#include "ClassMotors.hpp"

ClassMotors::ClassMotors(){
    xQueue = xQueueCreate(30, sizeof(TaskParams));
}

void ClassMotors::vMotors(void* pvParameters){
    ClassMotors* instance = (ClassMotors*)pvParameters;
    TaskParams taskParams;
    int steps = 0;

    while (1) {
        if(xQueueReceive(instance->xQueue, &taskParams, portMAX_DELAY)==pdPASS){

            float speed = (taskParams.vitesse * stepPerRev)/((3.14 * dRoues));
            moteurGauche.setMaxSpeed(speed);
            moteurDroit.setMaxSpeed(speed);

            if(taskParams.angle==0 && taskParams.distance==0){
                Serial.println("Tout les parametres a 0");
            }
            else if(taskParams.angle==0){ //Pour avancer
                steps = ((int)taskParams.distance / (3.14 * dRoues)) * stepPerRev;

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

                while (moteurGauche.distanceToGo() != 0 || moteurDroit.distanceToGo() != 0) { //Goooo
                    moteurGauche.run();
                    moteurDroit.run();
                }
                
            }
            else if(taskParams.distance==0){ //Pour tourner
                steps = ((int)std::abs(taskParams.angle) / 360.0) * (3.14 * ecartRoues) * stepPerRev / (3.14 * dRoues);

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
                }
            }
            else{
                Serial.println("il faut au moins un parametre a 0 entre angle et distance");
            }
        }
    }
}

void ClassMotors::StartMotors(){
    xTaskCreate(vMotors, "vMotors", 5000, this, 1, NULL);
}

void ClassMotors::EnvoyerDonnees(void* Params){
    TaskParams* ptaskParams = (TaskParams*)(Params); //Merci au patron de l'année derrnière en dépit de ses maigres performances concernant la coupe
    xQueueSend(xQueue, ptaskParams, portMAX_DELAY);
}