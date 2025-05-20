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
    float sigma = atan2(delta_x, delta_y) * RAD_TO_DEG;

    pangle = sigma - cangle_initial;
    pangleFin = cangle_final - sigma;

    if (pangle > 180) pangle -= 360;
    if (pangle < -180) pangle += 360;
    if (pangleFin > 180) pangleFin -= 360;
    if (pangleFin < -180) pangleFin += 360;

    if (abs(pangle) > 90){
        pangle += 180.0;
        pangleFin += 180.0;
        r = -r;

        if (pangle > 180) pangle -= 360;
        if (pangle < -180) pangle += 360;
        if (pangleFin > 180) pangleFin -= 360;
        if (pangleFin < -180) pangleFin += 360;
    }

    Serial.print("cangle_initial: "); Serial.print(cangle_initial);
    Serial.print("  sigma: "); Serial.print(sigma);
    Serial.print("  pangle: "); Serial.print(pangle);
    Serial.print("  cangle_final: "); Serial.print(cangle_final);
    Serial.print("  pangleFin: "); Serial.println(pangleFin);
}

void GoToPosition::Go(float x_f,float y_f,float cangle_f) {
    static int retryCount = 0;
    
    TaskParams Params;
    x_final = x_f;
    y_final = y_f;
    cangle_final = cangle_f;
    
    CalculPolar();

    Params = {0, (int)(abs(pangle)), (pangle > 0) ? 1 : 0, 500};
    mot.EnvoyerDonnees(&Params);

    Params = {(int)r, 0, 0, 1000};
    mot.EnvoyerDonnees(&Params);

    Params = {0, (int)(abs(pangleFin)), (pangleFin > 0) ? 1 : 0, 500};
    mot.EnvoyerDonnees(&Params);

    mot.WaitUntilDone();
    
    if (FLAG_STOP) {
        Serial.println("Le robot a ete arrete avant d'atteindre la position finale.");
        UpdateFinalPoseAfterStop(mot.GetDistanceDid());
        FLAG_STOP = false;

        if (retryCount >= 3) {
            Serial.println("Trop de tentatives d'evitement, abandon de la stratégie.");
            retryCount = 0;
            return;
        }

        retryCount++;
        if (Evitement()) {
            Serial.println("Reprise de la strategie initiale apres evitement.");
            Go(x_final, y_final, cangle_final);
            return;
        }
    } else {
        Serial.println("Le robot a atteint la position finale.");
        retryCount = 0;
        x_initial = x_final;
        y_initial = y_final;
        cangle_initial = cangle_final;
    }

    Serial.println("Position interne mise à jour apres déplacement.");
    Serial.print("x_initial: "); Serial.print(x_initial);
    Serial.print("  y_initial: "); Serial.print(y_initial);
    Serial.print("  cangle_initial: "); Serial.println(cangle_initial);
}

void GoToPosition::AllerEtSet(float x_f, float y_f, float cangle_f, float x_set, float y_set, float cangle_set) {
    Go(x_f, y_f, cangle_f);

    x_initial = x_set;
    y_initial = y_set;
    cangle_initial = cangle_set;

    Serial.println("Position interne mise à jour manuellement apres deplacement.");
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
    Serial.println("Debut de la strategie d'evitement.");

    float theta = cangle_initial * DEG_TO_RAD;

    // Recul de 200 mm
    float dx = 0, dy = -200;
    float x_backup = x_initial + dx * cos(theta) + dy * sin(theta);
    float y_backup = y_initial - dx * sin(theta) + dy * cos(theta);

    GoToPosition recul(x_initial, y_initial, cangle_initial, x_backup, y_backup, cangle_initial);
    recul.Go(x_backup, y_backup, cangle_initial);

    // Décalage gauche (theta - 90°)
    dx = 250, dy = 0;
    float x_left = x_backup + dx * cos(theta) + dy * sin(theta);
    float y_left = y_backup - dx * sin(theta) + dy * cos(theta);

    Serial.print("x_left: "); Serial.print(x_left);
    Serial.print("  y_left: "); Serial.println(y_left);

    if (!IsInForbiddenZone(x_left, y_left)) {
        Serial.println("Evitement par la gauche.");

        GoToPosition gauche(x_backup, y_backup, cangle_initial, x_left, y_left, cangle_initial - 90);
        gauche.Go(x_left, y_left, cangle_initial);

        x_initial = x_left;
        y_initial = y_left;

        Serial.print("x_left: "); Serial.print(x_left);
        Serial.print("  y_left: "); Serial.println(y_left);

        dx = 0, dy = 200;
        x_left = x_left + dx * cos(theta) + dy * sin(theta);
        y_left = y_left - dx * sin(theta) + dy * cos(theta);

        Serial.print("x_left: "); Serial.print(x_left);
        Serial.print("  y_left: "); Serial.println(y_left);
        gauche.Go(x_left, y_left, cangle_initial);

        x_initial = x_left;
        y_initial = y_left;
        return true;
    }

    // Décalage droite (theta + 90°)
    Serial.println("Gauche impossible, tentative par la droite.");
    dx = -250, dy = 0;
    float x_right = x_backup + dx * cos(theta) + dy * sin(theta);
    float y_right = y_backup - dx * sin(theta) + dy * cos(theta);

    if (!IsInForbiddenZone(x_right, y_right)) {
        Serial.println("Évitement par la droite.");

        GoToPosition droite(x_backup, y_backup, cangle_initial, x_right, y_right, cangle_initial);
        droite.Go(x_right, y_right, cangle_initial);

        x_initial = x_right;
        y_initial = y_right;

        dx = 0, dy = 200;
        x_right = x_right + dx * cos(theta) + dy * sin(theta);
        y_right = y_right - dx * sin(theta) + dy * cos(theta);
        droite.Go(x_right, y_right, cangle_initial);

        x_initial = x_right;
        y_initial = y_right;

        return true;
    }

    Serial.println("Aucune voie d’évitement disponible.");
    return false;
}


// Liste des zones interdites
std::vector<Zone> forbiddenZones = {
    {100, 100, 200, 200},
    {300, 300, 400, 400}
};

bool GoToPosition::IsInForbiddenZone(float x, float y) {
    for (const auto& zone : forbiddenZones) {
        if (x >= zone.x1 && x <= zone.x2 && y >= zone.y1 && y <= zone.y2)
            return true;
    }
    return false;
}
