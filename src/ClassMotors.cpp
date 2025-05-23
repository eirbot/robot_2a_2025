#include "ClassMotors.hpp"

ClassMotors::ClassMotors(){
    xQueue = xQueueCreate(30, sizeof(TaskParams));
    xQueueBuffer = xQueueCreate(30, sizeof(TaskParams));
}

void ClassMotors::vMotors(void* pvParameters){
    ClassMotors* instance = (ClassMotors*)pvParameters;
    TaskParams taskParams;
    int steps = 0;

    // Gestion des arrets
    const TickType_t maxIdleTime = pdMS_TO_TICKS(5000); // 5000 ms = 5 secondes
    TickType_t stopStartTime = 0;
    bool wasStopped = false;

    while (1) {
        if(xQueueReceive(instance->xQueue, &taskParams, portMAX_DELAY)==pdPASS){

            float speed = (taskParams.vitesse * stepPerRev)/((M_PI * dRoues));
            float accel = speed * 0.6;
            moteurGauche.setMaxSpeed(speed);
            moteurGauche.setAcceleration(accel);
            moteurDroit.setMaxSpeed(speed);
            moteurDroit.setAcceleration(accel);


            instance->currentStep = moteurGauche.currentPosition();

            if(taskParams.angle==0 && taskParams.distance==0){
                Serial.println("Tout les parametres a 0");
            }
            else if(taskParams.angle==0){ //Pour avancer
                steps = ((int)taskParams.distance / (M_PI * dRoues)) * stepPerRev;
                instance->stepDid = 0;

                Serial.print("Steps de la ligne droite:   ");
                Serial.println(steps);
                if(taskParams.direction == 1){// On recule, ou on t'encule
                    moteurGauche.move(-steps);
                    moteurDroit.move(-steps);
                }
                else{// On avance
                    moteurGauche.move(steps);
                    moteurDroit.move(steps);
                }

                if(steps > 0){
                    FLAG_CLEAR = &frontClear_tof;
                } else{
                    FLAG_CLEAR = &backClear_tof;
                }

                while (moteurGauche.distanceToGo() != 0 || moteurDroit.distanceToGo() != 0) { //Gooo                
                    // Si FLAG_STOP est actif
                    if (!*FLAG_CLEAR) {
                        if (!wasStopped) {
                             // Ralentissement progressif
                            StopStepper(moteurGauche, moteurDroit);

                            Serial.println("Robot arrêté. Ralentissement progressif.");

                            stopStartTime = xTaskGetTickCount(); // Première fois qu'on détecte l'arrêt
                            wasStopped = true;

                            instance->stepDid = moteurGauche.currentPosition() - instance->GetCurrentStep();  
                            instance->distanceDid = (instance->GetStepDid() * M_PI * dRoues) / stepPerRev;

                            float remainingSteps = steps - instance->stepDid;
                            moteurGauche.setAcceleration(accel);
                            moteurDroit.setAcceleration(accel);

                            moteurGauche.move(remainingSteps);
                            moteurDroit.move(remainingSteps);

                            Serial.print("Distance restante: ");
                            Serial.println(instance->distanceDid);
                            Serial.print("Step restant: ");
                            Serial.println(instance->stepDid);
                        }
                        else if ((xTaskGetTickCount() - stopStartTime) > maxIdleTime) {
                            Serial.println("Robot arrêté trop longtemps. Vidage de la queue.");
                            wasStopped = false; // Remet à zéro après vidage
                            FLAG_STOP = true;

                            instance->stepDid = moteurGauche.currentPosition() - instance->GetCurrentStep();  
                            instance->distanceDid = (instance->GetStepDid() * M_PI * dRoues) / stepPerRev;  

                            instance->TransferQueueBuffer();
                            break; // sort de la boucle de mouvement
                        }
                    } 
                    else {
                        if (wasStopped) {
                            wasStopped = false;
                        }

                        moteurGauche.run();
                        moteurDroit.run(); 
                        instance->UpdateOdometry();
                    }
                }               
            }
            else if(taskParams.distance==0){ //Pour tourner
                steps = ((int)std::abs(taskParams.angle) / 360.0) * (M_PI * ecartRoues) * stepPerRev / (M_PI * dRoues);
                instance->stepDid = steps;

                if(taskParams.direction == 0){//0 droite, 1 gauche
                    moteurGauche.move(steps);
                    moteurDroit.move(-steps);
                }
                else{
                    moteurGauche.move(-steps);
                    moteurDroit.move(steps);
                }

                while (moteurGauche.distanceToGo() != 0 || moteurDroit.distanceToGo() != 0) { //Goooo                    
                    moteurGauche.run();
                    moteurDroit.run();
                    instance->UpdateOdometry();
                }
            }
            else{
                Serial.println("il faut au moins un parametre a 0 entre angle et distance");
            }
            vTaskDelay(100);
        }
    }
}

void ClassMotors::StartMotors(){
    xTaskCreate(vMotors, "vMotors", 10000, this, 1, NULL);
}

void ClassMotors::EnvoyerDonnees(void* Params){
    TaskParams* ptaskParams = (TaskParams*)(Params); //Merci au patron de l'année derrnière en dépit de ses maigres performances concernant la coupe
    xQueueSend(xQueue, ptaskParams, portMAX_DELAY);
}

void ClassMotors::WaitUntilDone() {
    // Boucle bloquante tant que la file contient des messages
    while (uxQueueMessagesWaiting(xQueue) > 0) {
        vTaskDelay(pdMS_TO_TICKS(10)); // Petite pause pour ne pas bloquer l'ordonnanceur
    }
}

void ClassMotors::TransferQueueBuffer() {
    TaskParams tmp;
    while (xQueueReceive(xQueue, &tmp, 0) == pdTRUE) {
        xQueueSend(xQueueBuffer, &tmp, 0); // Sauvegarde dans le tampon
    }
}

void ClassMotors::RestoreQueueBuffer() {
    TaskParams tmp;
    while (xQueueReceive(xQueueBuffer, &tmp, 0) == pdTRUE) {
        xQueueSend(xQueue, &tmp, 0); // Recharge
    }
}

void StopStepper(AccelStepper& moteur1, AccelStepper& moteur2) {
    moteur1.setAcceleration(DECCEL); // Ralentissement
    moteur2.setAcceleration(DECCEL); // Ralentissement

    moteur1.stop(); // Arrête le moteur
    moteur2.stop(); // Arrête le moteur
    while (moteur1.isRunning() || moteur2.isRunning()) {
        // On attend que les moteurs s'arrêtent
        moteur1.run();
        moteur2.run();
    }
    vTaskDelay(100); // Pause pour éviter un mouvement trop rapide
}

void ClassMotors::GetPosition(float &x, float &y, float &angle) {
    if (xSemaphoreTake(xPositionMutex, portMAX_DELAY) == pdTRUE) {
        x = x_pos;
        y = y_pos;
        angle = orientation;
        xSemaphoreGive(xPositionMutex);
    }
}

void ClassMotors::SetPosition(float x, float y, float angle) {
    if (xSemaphoreTake(xPositionMutex, portMAX_DELAY) == pdTRUE) {
        x_pos = x;
        y_pos = y;
        orientation = angle;
        xSemaphoreGive(xPositionMutex);
    }
}

void ClassMotors::UpdateOdometry() {
    static long lastStepGauche = 0;
    static long lastStepDroit = 0;

    long currentStepGauche = moteurGauche.currentPosition();
    long currentStepDroit = moteurDroit.currentPosition();

    long deltaStepGauche = currentStepGauche - lastStepGauche;
    long deltaStepDroit  = currentStepDroit  - lastStepDroit;

    lastStepGauche = currentStepGauche;
    lastStepDroit  = currentStepDroit;

    float distanceParStep = (M_PI * dRoues) / stepPerRev;
    float s_L = deltaStepGauche * distanceParStep;
    float s_R = deltaStepDroit  * distanceParStep;

    float delta_s = (s_R + s_L) / 2.0;
    float delta_theta = (s_R - s_L) / ecartRoues; // en radians

    float x, y, angle;

    // Prendre les positions actuelles
    GetPosition(x, y, angle);

    // Mise à jour de l'orientation
    angle += delta_theta;

    // Normalisation
    if (angle > M_PI) angle -= 2 * M_PI;
    if (angle < -M_PI) angle += 2 * M_PI;

    // Mise à jour de la position
    x += delta_s * cos(angle);
    y += delta_s * sin(angle);

    // Écrire la nouvelle position
    SetPosition(x, y, angle);

}
