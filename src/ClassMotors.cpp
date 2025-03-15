#include "ClassMotors.hpp"

ClassMotors::ClassMotors(){
    xQueue = xQueueCreate(5, sizeof(int));
}

void ClassMotors::vMotors(void* pvParameters){
    ClassMotors* instance = (ClassMotors*)pvParameters;
    Serial.println("tache lance");
    int valeurRecu;
    int step = 0;
    int steps = 0;
    while (1) {
        if(xQueueReceive(instance->xQueue, &valeurRecu, portMAX_DELAY)==pdPASS){
            Serial.println("tache execute");
            steps = ((int)valeurRecu / (3.14 * dRoues)) * stepPerRev;
            step = 0;

            for(step=0; step<steps; step++){
                digitalWrite(STEPD,HIGH);
                digitalWrite(STEPG,HIGH);
                esp_rom_delay_us(300);
                digitalWrite(STEPD,LOW);
                digitalWrite(STEPG,LOW);
                esp_rom_delay_us(300);
            }
        }
    }
}

void ClassMotors::StartMotors(){
    xTaskCreate(vMotors, "vMotors", 5000, this, 1, NULL);
}

void ClassMotors::EnvoyerDonnees(void* Params){
    TaskParams* tasParams = (TaskParams*)(Params); //Merci au patron de l'année derrnière en dépit de ses maigres performances concernant la coupe
    xQueueSend(xQueue, &tasParams->distance, portMAX_DELAY);
}