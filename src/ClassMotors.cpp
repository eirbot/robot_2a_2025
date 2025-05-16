#include "ClassMotors.hpp"

ClassMotors::ClassMotors(){
    xQueue = xQueueCreate(30, sizeof(TaskParams));
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
            moteurGauche.setMaxSpeed(speed);
            moteurDroit.setMaxSpeed(speed);

            instance->currentStep = moteurGauche.currentPosition();

            if(taskParams.angle==0 && taskParams.distance==0){
                Serial.println("Tout les parametres a 0");
            }
            else if(taskParams.angle==0){ //Pour avancer
                steps = ((int)taskParams.distance / (M_PI * dRoues)) * stepPerRev;
                instance->stepRemain = steps;

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
                            RampDownAccelStepper(moteurGauche, moteurDroit);
                            Serial.println("Robot arrêté. Ralentissement progressif.");

                            stopStartTime = xTaskGetTickCount(); // Première fois qu'on détecte l'arrêt
                            wasStopped = true;

                            instance->stepRemain = instance->GetCurrentStep() - moteurGauche.currentPosition();  
                            instance->distanceRemain = (instance->GetStepRemain() * M_PI * dRoues) / stepPerRev;

                            Serial.print("Distance restante: ");
                            Serial.println(instance->distanceRemain);
                            Serial.print("Step restant: ");
                            Serial.println(instance->stepRemain);
                        }
                        else if ((xTaskGetTickCount() - stopStartTime) > maxIdleTime) {
                            Serial.println("Robot arrêté trop longtemps. Vidage de la queue.");
                            instance->ViderQueue();
                            wasStopped = false; // Remet à zéro après vidage
                            FLAG_STOP = true;

                            instance->stepRemain = instance->GetCurrentStep() - moteurGauche.currentPosition();  
                            instance->distanceRemain = (instance->GetStepRemain() * M_PI * dRoues) / stepPerRev;  

                            break; // sort de la boucle de mouvement
                        }
                    } 
                    else {
                        if (wasStopped) {
                            float ratio = std::abs(instance->distanceRemain) / DIST_MAX;
                            if (ratio > 1.0) ratio = 1.0;
                            if (ratio < 0.0) ratio = 0.0;

                            float dynamicSpeed = V_MIN + ratio * (speed - V_MIN);
                            if (steps < 0) {
                                dynamicSpeed = -dynamicSpeed; // Inverser la vitesse si le moteur est en marche arrière
                            }

                            if (instance->stepRemain < STEP_ACCEL) {
                                RampUpAccelStepper(moteurGauche, moteurDroit, dynamicSpeed, instance->stepRemain/2);
                            }
                            else {
                                RampUpAccelStepper(moteurGauche, moteurDroit, dynamicSpeed);
                            }                    
                            wasStopped = false;

                            Serial.println("Robot en mouvement. Accélération dynamique.");
                        }

                        moteurGauche.run();
                        moteurDroit.run(); 
                    }
                }               
            }
            else if(taskParams.distance==0){ //Pour tourner
                steps = ((int)std::abs(taskParams.angle) / 360.0) * (M_PI * ecartRoues) * stepPerRev / (M_PI * dRoues);
                instance->stepRemain = steps;

                if(taskParams.direction == 0){//0 droite, 1 gauche
                    moteurGauche.move(steps);
                    moteurDroit.move(-steps);
                }
                else{
                    moteurGauche.move(-steps);
                    moteurDroit.move(steps);
                }

                while (moteurGauche.distanceToGo() != 0 || moteurDroit.distanceToGo() != 0) { //Goooo                    
                    // Si FLAG_STOP est actif
                    if (!*FLAG_CLEAR) {
                        if (!wasStopped) {
                            stopStartTime = xTaskGetTickCount(); // Première fois qu'on détecte l'arrêt
                            wasStopped = true;

                            // Ralentissement progressif
                            RampDownAccelStepper(moteurGauche, moteurDroit);
                            
                            instance->stepRemain = instance->GetCurrentStep() - moteurGauche.currentPosition();
                            instance->angleRemain = (instance->GetStepRemain() * 360.0) / (M_PI * ecartRoues * stepPerRev) * (M_PI * dRoues);
                        } 
                        else if ((xTaskGetTickCount() - stopStartTime) > maxIdleTime) {
                            Serial.println("Robot arrêté trop longtemps. Vidage de la queue.");
                            instance->ViderQueue();
                            wasStopped = false; // Remet à zéro après vidage
                            FLAG_STOP = true;

                            // Calcul de l'angle restant
                            instance->stepRemain = instance->GetCurrentStep() - moteurGauche.currentPosition();
                            instance->angleRemain = (instance->GetStepRemain() * 360.0) / (M_PI * ecartRoues * stepPerRev) * (M_PI * dRoues);

                            break; // sort de la boucle de mouvement
                        }
                    } 
                    else {
                        if (wasStopped) {
                            float angleRatio = instance->angleRemain / ANGLE_MAX;
                            if (angleRatio > 1.0f) angleRatio = 1.0f;
                            if (angleRatio < 0.0f) angleRatio = 0.0f;

                            float dynamicAngleSpeed = V_ROT_MIN + angleRatio * (speed - V_ROT_MIN);

                            RampUpAccelStepper(moteurGauche, moteurDroit, dynamicAngleSpeed);

                            wasStopped = false;
                        }

                        moteurGauche.run();
                        moteurDroit.run();
                        }
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

void ClassMotors::ViderQueue() {
    TaskParams tmp;
    while (xQueueReceive(xQueue, &tmp, 0) == pdTRUE);
}

void RampDownAccelStepper(AccelStepper& moteur1, AccelStepper& moteur2) {
    float currentSpeed = moteur1.speed();
    Serial.print("Vitesse actuelle: ");
    Serial.println(currentSpeed);
    float rampStep = currentSpeed / STEP_DECCEL; // Nombre de pas de ralentissement
    for (int i = 0; i < STEP_DECCEL; ++i) {
        currentSpeed -= rampStep;
        moteur1.setSpeed(currentSpeed);
        moteur2.setSpeed(currentSpeed);

        moteur1.runSpeed(); // Utilise runSpeed au lieu de run pour un contrôle direct
        moteur2.runSpeed(); // Utilise runSpeed au lieu de run pour un contrôle direct
    }
    vTaskDelay(100); // Pause pour éviter un mouvement trop rapide
}

void RampUpAccelStepper(AccelStepper& moteur1, AccelStepper& moteur2, float targetSpeed, float stepAccel) {
    float rampStep = targetSpeed / stepAccel; // STEP_ACCEL : nombre de pas d'accélération
    float currentSpeed = 0;

    for (int i = 0; i < stepAccel; ++i) {
        currentSpeed += rampStep;
        moteur1.setSpeed(currentSpeed);
        moteur2.setSpeed(currentSpeed);
        
        moteur1.runSpeed();  // Mouvement instantané à la vitesse fixée
        moteur2.runSpeed();  // Mouvement instantané à la vitesse fixée
    }
    Serial.print("Vitesse cible: ");
    Serial.println(targetSpeed);
    Serial.print("Vitesse actuelle: ");
    Serial.println(currentSpeed);
    // On s'assure d'avoir bien atteint la vitesse cible
    moteur1.setSpeed(targetSpeed);
    moteur2.setSpeed(targetSpeed);
}
