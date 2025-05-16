#include "GoToPosition.hpp"


GoToPosition::GoToPosition(const float &x_i,const float &y_i,const float &cangle_i, const float &x_f,const float &y_f,const float &cangle_f){
    x_initial = x_i;
    y_initial = y_i;
    cangle_initial = cangle_i;
    x_final = x_f;
    y_final = y_f;
    cangle_final = cangle_f;
}

void GoToPosition::CalculPolar() {
    float delta_x = x_final - x_initial;
    float delta_y = y_final - y_initial;

    r = sqrt(delta_x * delta_x + delta_y * delta_y);

    // Angle vers la cible en degrés
    float sigma = atan2(delta_x, delta_y) * RAD_TO_DEG;

    // Angle à tourner pour faire face à la cible
    pangle = sigma - cangle_initial;

    // Angle à tourner une fois arrivé pour atteindre la bonne orientation
    pangleFin = cangle_final - sigma;

    // Normalisation dans [-180 ; 180]
    if (pangle > 180) pangle -= 360;
    if (pangle < -180) pangle += 360;

    if (pangleFin > 180) pangleFin -= 360;
    if (pangleFin < -180) pangleFin += 360;

    if (abs(pangle) > 90){
        pangle += 180.0;
        pangleFin += 180.0;
        r = -r;

        // Normalisation dans [-180 ; 180]
        if (pangle > 180) pangle -= 360;
        if (pangle < -180) pangle += 360;

        if (pangleFin > 180) pangleFin -= 360;
        if (pangleFin < -180) pangleFin += 360;
    }

    Serial.print("cangle_initial: ");
    Serial.print(cangle_initial);
    Serial.print("  sigma: ");
    Serial.print(sigma);
    Serial.print("  pangle: ");
    Serial.print(pangle);
    Serial.print("  cangle_final: ");
    Serial.print(cangle_final);
    Serial.print("  pangleFin: ");
    Serial.println(pangleFin);
}

void GoToPosition::Go(float x_f,float y_f,float cangle_f) {

    TaskParams Params;
    x_final = x_f;
    y_final = y_f;
    cangle_final = cangle_f;
    
    CalculPolar();

    Params = {0, (int)(abs(pangle)), (pangle > 0) ? 1 : 0, 500};
    mot.EnvoyerDonnees(&Params);

    Params = {(int)r, 0, 0, 600};
    mot.EnvoyerDonnees(&Params);

    Params = {0, (int)(abs(pangleFin)), (pangleFin > 0) ? 1 : 0, 500};
    mot.EnvoyerDonnees(&Params);

    // On attend que le robot ait fini de bouger
    mot.WaitUntilDone();
    
    if (FLAG_STOP) {
        Serial.println("Le robot a été arrêté avant d'atteindre la position finale.");
        UpdateFinalPoseAfterStop(mot.GetDistanceDid());
        FLAG_STOP = false;

        if (Evitement()) {
            Serial.println("Reprise de la stratégie initiale après évitement.");
            Go(x_final, y_final, cangle_final);  // relance le déplacement principal
            return;
        }
    } else{
        Serial.println("Le robot a atteint la position finale.");

        x_initial = x_final;
        y_initial = y_final;
        cangle_initial = cangle_final;
    }

    Serial.println("Position interne mise à jour après déplacement.");
    Serial.print("x_initial: "); Serial.print(x_initial);
    Serial.print("  y_initial: "); Serial.print(y_initial);
    Serial.print("  cangle_initial: "); Serial.println(cangle_initial);
}

void GoToPosition::AllerEtSet(float x_f, float y_f, float cangle_f, float x_set, float y_set, float cangle_set) {
    // D'abord on va à la position demandée
    Go(x_f, y_f, cangle_f);

    // Puis on force la position interne du robot
    x_initial = x_set;
    y_initial = y_set;
    cangle_initial = cangle_set;

    Serial.println("Position interne mise à jour manuellement après déplacement.");
    Serial.print("x_set: "); Serial.print(x_set);
    Serial.print("  y_set: "); Serial.print(y_set);
    Serial.print("  cangle_set: "); Serial.println(cangle_set);
}

void GoToPosition::UpdateFinalPoseAfterStop(float distanceDid) {
    cangle_initial += pangle;
    
    float dx = distanceDid * sin(cangle_initial * DEG_TO_RAD);
    float dy = distanceDid * cos(cangle_initial * DEG_TO_RAD);

    x_initial += dx;
    y_initial += dy;
}

bool GoToPosition::Evitement() {
    Serial.println("Début de la stratégie d'évitement.");

    float dx = 0, dy = -200;  // reculer de 50 mm
    float x_backup = x_initial + dx * cos(cangle_initial) - dy * sin(cangle_initial);
    float y_backup = y_initial + dx * sin(cangle_initial) + dy * cos(cangle_initial);

    GoToPosition recul(x_initial, y_initial, cangle_initial, x_backup, y_backup, cangle_initial);
    recul.Go(x_backup, y_backup, cangle_initial);

    // Tester zone interdite
    if (IsInForbiddenZone(x_backup, y_backup)) {
        Serial.println("Zone interdite détectée, évitement par la gauche.");

        // Essai vers la gauche
        float x_left = x_backup + 0 * cos(cangle_initial) - 100 * sin(cangle_initial);
        float y_left = y_backup + 0 * sin(cangle_initial) + 100 * cos(cangle_initial);

        GoToPosition gauche(x_backup, y_backup, cangle_initial, x_left, y_left, cangle_initial);
        gauche.Go(x_left, y_left, cangle_initial);
    } else {
        Serial.println("Zone libre, évitement par la droite.");

        // Essai vers la droite
        float x_right = x_backup + 0 * cos(cangle_initial) + 100 * sin(cangle_initial);
        float y_right = y_backup + 0 * sin(cangle_initial) - 100 * cos(cangle_initial);

        GoToPosition droite(x_backup, y_backup, cangle_initial, x_right, y_right, cangle_initial);
        droite.Go(x_right, y_right, cangle_initial);
    }

    return true;
}

bool GoToPosition::IsInForbiddenZone(float x, float y) {
    // Exemple simple : zone interdite si dans rectangle (100,100) à (200,200)
    return (x >= 100 && x <= 200 && y >= 100 && y <= 200);
}
