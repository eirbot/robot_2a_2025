#include "motors.h"

//un tour = 3200 step

void vavancer(void *Parameters_temp){
    TaskParams *Parameters = (TaskParams *)Parameters_temp;
    if(Parameters->direction == 1){
      digitalWrite(DIRD,LOW);
      digitalWrite(DIRG,LOW);
    }
    else{
      digitalWrite(DIRD,HIGH);
      digitalWrite(DIRG,HIGH);
    }
    int steps = ((int)Parameters->distance / (3.14 * dRoues)) * stepPerRev;
    for(int k=0 ; k < steps; k++){
      digitalWrite(STEPD,HIGH);
      digitalWrite(STEPG,HIGH);
      esp_rom_delay_us(300);
      digitalWrite(STEPD,LOW);
      digitalWrite(STEPG,LOW);
      esp_rom_delay_us(300);
    }
    Serial.println("fin de avancer");
    vTaskDelay(1);
    xTaskNotifyGive(vstratHandle);
    vTaskDelete(NULL);
}

void vtourner(void *Parameters_temp){
    TaskParams *Parameters = (TaskParams *)Parameters_temp;
    if(Parameters->direction == 0){//0 droite, 1 gauche
        digitalWrite(DIRD,LOW);
        digitalWrite(DIRG,HIGH);
    }
    else{
        digitalWrite(DIRD,HIGH);
        digitalWrite(DIRG,LOW);
    }
    int steps = ((int)Parameters->angle / 360.0) * (3.14 * ecartRoues) * stepPerRev / (3.14 * dRoues);
    for(int k=0 ; k < steps; k++){
        digitalWrite(STEPD,HIGH);
        digitalWrite(STEPG,HIGH);
        esp_rom_delay_us(300);
        digitalWrite(STEPD,LOW);
        digitalWrite(STEPG,LOW);
        esp_rom_delay_us(300);
    }
    vTaskDelay(1);
    xTaskNotifyGive(vstratHandle);
    vTaskDelete(NULL);
}