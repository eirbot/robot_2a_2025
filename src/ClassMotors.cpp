#include "ClassMotors.hpp"

ClassMotors::ClassMotors(){
    xQueue = xQueueCreate(5, sizeof(TaskParams));
}

void ClassMotors::vMotors(void* pvParameters){
    ClassMotors* instance = (ClassMotors*)pvParameters;
    Serial.println("tache qui tourne");
    TaskParams taskParams;
    int step = 0;
    int steps = 0;
    while (1) {
        if(xQueueReceive(instance->xQueue, &taskParams, portMAX_DELAY)==pdPASS){
            Serial.println("tache lance");

            if(taskParams.angle==0){
                Serial.println("tache de ligne droite");
                steps = ((int)taskParams.distance / (3.14 * dRoues)) * stepPerRev;
                step = 0;

                if(taskParams.direction == 1){
                    digitalWrite(DIRD,LOW);
                    digitalWrite(DIRG,LOW);
                }
                else{
                    digitalWrite(DIRD,HIGH);
                    digitalWrite(DIRG,HIGH);
                }

                for(step=0; step<steps; step++){
                    digitalWrite(STEPD,HIGH);
                    digitalWrite(STEPG,HIGH);
                    esp_rom_delay_us(300);
                    digitalWrite(STEPD,LOW);
                    digitalWrite(STEPG,LOW);
                    esp_rom_delay_us(300);
                }
            }
            else if(taskParams.distance==0){
                Serial.println("tache de virage");
                int steps = ((int)std::abs(Parameters.angle) / 360.0) * (3.14 * ecartRoues) * stepPerRev / (3.14 * dRoues);
                step = 0;

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
                    esp_rom_delay_us(300);
                    digitalWrite(STEPD,LOW);
                    digitalWrite(STEPG,LOW);
                    esp_rom_delay_us(300);
                }
            }
            else{
                Serial.println("il faut au moins un paramètre à 0 entre angle et distance");
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