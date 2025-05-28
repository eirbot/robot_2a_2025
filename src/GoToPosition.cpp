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
}

void GoToPosition::Go(float x_f,float y_f,float cangle_f) {
    static int retryCount = 0;
    
    TaskParams Params;
    x_final = x_f;
    y_final = y_f;
    cangle_final = cangle_f;
    
    CalculPolar();

    Params = {0, (int)(abs(pangle)), (pangle > 0) ? 1 : 0, (int)(SPEEDMAX * 0.7)};
    mot.EnvoyerDonnees(&Params);

    Params = {(int)r, 0, 0, SPEEDMAX};
    mot.EnvoyerDonnees(&Params);

    Params = {0, (int)(abs(pangleFin)), (pangleFin > 0) ? 1 : 0, (int)(SPEEDMAX * 0.7)};
    mot.EnvoyerDonnees(&Params);

    mot.WaitUntilDone();
    
    if (FLAG_STOP) {
        UpdateFinalPoseAfterStop(mot.GetDistanceDid());

        float x, y, angle;
        mot.GetPosition(x, y, angle);
        FLAG_STOP = false;

        if (retryCount >= 3) {
            retryCount = 0;
            return;
        }

        retryCount++;
        if (Evitement()) {
            mot.RestoreQueueBuffer();  // <-- À insérer ici
            Go(x_final, y_final, cangle_final);
        }

    } else {
        retryCount = 0;
        float x, y, angle;
        mot.GetPosition(x, y, angle);

        x_initial = x;
        y_initial = y;
        cangle_initial = cangle_final;

        // Serial.printf("x: %f, y: %f, angle: %f\n", x_initial, y_initial, cangle_initial);
    }
}

void GoToPosition::AllerEtSet(float x_f, float y_f, float cangle_f, float x_set, float y_set, float cangle_set) {
    Go(x_f, y_f, cangle_f);

    x_initial = x_set;
    y_initial = y_set;
    cangle_initial = cangle_set;

    // Mettre à jour la position finale après l'arrêt
    mot.SetPosition(x_initial, y_initial, cangle_initial * DEG_TO_RAD);
    mot.UpdateOdometry();
}

void GoToPosition::UpdateFinalPoseAfterStop(float distanceDid) {
    cangle_initial += pangle;
    
    float dx = distanceDid * sin(cangle_initial * DEG_TO_RAD);
    float dy = distanceDid * cos(cangle_initial * DEG_TO_RAD);

    x_initial += dx;
    y_initial += dy;
}

bool GoToPosition::Evitement() {
    float theta = cangle_initial * DEG_TO_RAD;

    // Recul de 200 mm
    float dx, dy;
    if(r > 0){
        dx = 0, dy = -250;
    } else {
        dx = 0, dy = 250;
    }
    float x_backup = x_initial + dx * cos(theta) + dy * sin(theta);
    float y_backup = y_initial - dx * sin(theta) + dy * cos(theta);

    GoToPosition recul(x_initial, y_initial, cangle_initial, x_backup, y_backup, cangle_initial);
    recul.Go(x_backup, y_backup, cangle_initial);

    // Décalage gauche (theta - 90°)
    dx = 350, dy = 0;
    float x_left = x_backup + dx * cos(theta) + dy * sin(theta);
    float y_left = y_backup - dx * sin(theta) + dy * cos(theta);

    if (!IsInForbiddenZone(x_left, y_left)) {
        GoToPosition gauche(x_backup, y_backup, cangle_initial, x_left, y_left, cangle_initial - 90);
        gauche.Go(x_left, y_left, cangle_initial);

        x_initial = x_left;
        y_initial = y_left;

        if (r > 0) {
            dx = 0, dy = 250;
        } else {
            dx = 0, dy = -250;
        }
        x_left = x_left + dx * cos(theta) + dy * sin(theta);
        y_left = y_left - dx * sin(theta) + dy * cos(theta);

        gauche.Go(x_left, y_left, cangle_initial);

        x_initial = x_left;
        y_initial = y_left;
        return true;
    }

    // Décalage droite (theta + 90°)
    dx = -250, dy = 0;
    float x_right = x_backup + dx * cos(theta) + dy * sin(theta);
    float y_right = y_backup - dx * sin(theta) + dy * cos(theta);

    if (!IsInForbiddenZone(x_right, y_right)) {

        GoToPosition droite(x_backup, y_backup, cangle_initial, x_right, y_right, cangle_initial);
        droite.Go(x_right, y_right, cangle_initial);

        x_initial = x_right;
        y_initial = y_right;

        if (r > 0) {
            dx = 0, dy = 250;
        } else {
            dx = 0, dy = -250;
        }
        x_right = x_right + dx * cos(theta) + dy * sin(theta);
        y_right = y_right - dx * sin(theta) + dy * cos(theta);
        droite.Go(x_right, y_right, cangle_initial);

        x_initial = x_right;
        y_initial = y_right;

        return true;
    }
    return false;
}


// Liste des zones interdites
std::vector<Zone> forbiddenZonesBlue = {
    {0, 0, 3000, 200}, // bord bas
    {0, 0, 200, 2000}, // bord gauche
    {2800, 0, 3000, 2000}, //bord droit
    {0, 1800, 3000, 2000}, //bord haut
    {650, 1800, 2350, 2000}, // Zone monter plateforme

    // Zone canettes Bleue
    {2000, 0, 2450, 150},
    {1550, 0, 2000, 450},
    {0, 0, 450, 150},
    {2550, 650, 3000, 1100},
    {1050, 1550, 3000, 2000},
    
    // Zone canettes Jaune
    {550, 0, 1000, 150},
    {1000, 0, 1450, 450},
    {2550, 0, 3000, 150},
    {2550, 650, 3000, 1100},
    {0, 1550, 1950, 2000}
    
};

// Liste des zones interdites
std::vector<Zone> forbiddenZonesYellow = {
    {0, 0, 3000, 200}, // bord bas
    {0, 0, 200, 2000}, // bord gauche
    {2800, 0, 3000, 2000}, //bord droit
    {0, 1800, 3000, 2000}, //bord haut
    {650, 1800, 2350, 2000}, // Zone monter plateforme

    // Zone canettes Bleue
    {2000, 0, 2450, 150},
    {1550, 0, 2000, 450},
    {0, 0, 450, 150},
    {2550, 650, 3000, 1100},
    {1050, 1550, 3000, 2000},

    // Zone canettes Jaune
    {550, 0, 1000, 150},
    {1000, 0, 1450, 450},
    {2550, 0, 3000, 150},
    {2550, 650, 3000, 1100},
    {0, 1550, 1950, 2000}
};

bool GoToPosition::IsInForbiddenZone(float x, float y) {
    if (jaune) { 
        for (const auto& zone : forbiddenZonesYellow) {
            if (x >= zone.x1 && x <= zone.x2 && y >= zone.y1 && y <= zone.y2)
                return true;
        }
    } else {
        for (const auto& zone : forbiddenZonesBlue) {
            if (x >= zone.x1 && x <= zone.x2 && y >= zone.y1 && y <= zone.y2)
                return true;
        }
    }
    return false;
}
