#include "main.h"

TaskHandle_t vstratHandle = NULL;

int resolution = 8;
int bloque = 0;

bool modeBluetooth = false;//choix du mode bluetooth ou strategique
unsigned long startMillis;

BluetoothSerial SerialBT;
TaskParams Parameters = {0, 0, 0, 0};

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

  xTaskCreate(vsetup_actionneurs,"vsetup_actionneurs", 1000, NULL, 1, NULL);
  
}

void loop() {
  commande();
  vTaskDelay(pdMS_TO_TICKS(1000));
}
