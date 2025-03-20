#include "ClassMotors.hpp"

ClassMotors::ClassMotors(){
    xQueue = xQueueCreate(30, sizeof(TaskParams));
}

void ClassMotors::vMotors(void* pvParameters){
    ClassMotors* instance = (ClassMotors*)pvParameters;
    TaskParams taskParams;
    int step = 0;
    int steps = 0;
    float delayMicrosVitesse = 300.0;
    while (1) {
        if(xQueueReceive(instance->xQueue, &taskParams, portMAX_DELAY)==pdPASS){

            pinMode(STEPD,OUTPUT);
            pinMode(STEPG,OUTPUT);

            if(taskParams.angle==0 && taskParams.distance==0){
                Serial.println("Tout les parametres a 0");
            }
            else if(taskParams.angle==0){ //Pour avancer
                steps = ((int)taskParams.distance / (3.14 * dRoues)) * stepPerRev;
                step = 0;
                delayMicrosVitesse=850.0;
                Serial.print("Steps de la ligne droite:   ");
                Serial.println(steps);
                if(taskParams.direction == 1){
                    digitalWrite(DIRD,LOW);
                    digitalWrite(DIRG,LOW);
                }
                else{
                    digitalWrite(DIRD,HIGH);
                    digitalWrite(DIRG,HIGH);
                }

                for(step=0; step<steps; step++){


                    if(steps-step<=2800){
                        delayMicrosVitesse += 0.25;
                    }
                    else if(step<2800){
                        delayMicrosVitesse -= 0.25;
                    }
                    

                    // Serial.print("delayMicrosVitesse:   ");
                    // Serial.println(delayMicrosVitesse);
                    digitalWrite(STEPD,HIGH);
                    digitalWrite(STEPG,HIGH);
                    esp_rom_delay_us((int)delayMicrosVitesse);
                    digitalWrite(STEPD,LOW);
                    digitalWrite(STEPG,LOW);
                    esp_rom_delay_us((int)delayMicrosVitesse);
                }
            }
            else if(taskParams.distance==0){ //Pour tourner
                steps = ((int)std::abs(taskParams.angle) / 360.0) * (3.14 * ecartRoues) * stepPerRev / (3.14 * dRoues);
                step = 0;

                delayMicrosVitesse = 300.0;

                if(taskParams.direction == 0){//0 droite, 1 gauche
                    digitalWrite(DIRD,LOW);
                    digitalWrite(DIRG,HIGH);
                }
                else{
                    digitalWrite(DIRD,HIGH);
                    digitalWrite(DIRG,LOW);
                }

                for(step=0 ; step < steps; step++){
                    
                    digitalWrite(STEPD,HIGH);
                    digitalWrite(STEPG,HIGH);
                    esp_rom_delay_us((int)delayMicrosVitesse);
                    digitalWrite(STEPD,LOW);
                    digitalWrite(STEPG,LOW);
                    esp_rom_delay_us((int)delayMicrosVitesse);
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