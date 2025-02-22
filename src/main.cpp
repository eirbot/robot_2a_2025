#include "main.h"

TaskHandle_t vstratHandle = NULL;

void vstrat(void *pvParameters){
  int distance = 100;
  int angle=90;
  
  Serial.println("commence");
  avancer(500,0,10);
  tourner(90,1,0); 
  avancer(400,0,10);
  tourner(90,0,0);
  avancer(650,0,10);
  tourner(90,0,0);
  avancer(1000,0,10);
  tourner(90,0,0);
  avancer(600,0,10);
  tourner(90,0,0);
  avancer(600,0,10);
  tourner(90,0,0);
  avancer(500,1,10);
  vTaskDelete(NULL);
}

void setup() {
  esp_task_wdt_init(10,true);

  
  if(modeBluetooth){
    ledcSetup(STEPD, 1000,resolution);
    ledcAttachPin(STEPD,1);

    ledcSetup(STEPG, 1000,resolution);
    ledcAttachPin(STEPG,2);
  }
  else{
    pinMode(STEPD,OUTPUT);
    pinMode(STEPG,OUTPUT);
  }

  pinMode(DIRD,OUTPUT);
  pinMode(DIRG,OUTPUT);


  Serial.begin(115200);
  startMillis = millis();

  SerialBT.begin("ESP32test");
  SerialBT.setTimeout(50);

   
  if(modeBluetooth){
    xTaskCreate(vcontrole_bluetooth,"vcontrole_bluetooth", 1000, NULL, 1, NULL); 
  }
  else{
    xTaskCreate(vstrat,"vstrat", 4096, NULL, 1, &vstratHandle ); 
  }
}

void loop() {
  commande();
  vTaskDelay(pdMS_TO_TICKS(1000));
}
