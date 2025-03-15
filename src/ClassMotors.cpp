#include "ClassMotors.hpp"

ClassMotors::ClassMotors(){
    xQueue = xQueueCreate(1, sizeof(int));
}

void ClassMotors::vMotors(void* pvParameters){
    ClassMotors* instance = (ClassMotors*)pvParameters;
    Serial.println("tache lance");
    int valeurRecu;
    while (1) {
        if(xQueueReceive(instance->xQueue, &valeurRecu, portMAX_DELAY)==pdPASS){
            Serial.println("tache execute");
        }
        vTaskDelay(1000);
    }
}

void ClassMotors::StartMotors(){
    xTaskCreate(vMotors, "vMotors", 5000, this, 1, NULL);
}

void ClassMotors::EnvoyerDonnees(int value){
    xQueueSend(xQueue, &value, portMAX_DELAY);
}