//Pour le PlatformIO

#include "Arduino.h"

#include "common.h"
#include "bluetooth.h"
#include "motors.h"
//Code de base


BluetoothSerial SerialBT;


bool modeBluetooth = false;

TaskHandle_t vstratHandle = NULL;

#define tourner(angle,direction,vitesse) \
  Parameters = {0, angle, direction, vitesse};\
  xTaskCreate(vtourner,"vtourner", 1000, &Parameters, 1, &vtournerHandle); \
  ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

#define avancer(distance,direction,vitesse) \
  Parameters = {distance, 0, direction, vitesse};\
  xTaskCreate(vavancer,"vavancer", 1000, &Parameters, 1, &vavancerHandle); \
  ulTaskNotifyTake(pdTRUE, portMAX_DELAY);


#if !defined(CONFIG_BT_STEPDBLED) || !defined(CONFIG_BLUEDROID_STEPDBLED)
//#error Bluetooth is not STEPDbled! Please run `make menuconfig` to and STEPDble it
#endif



unsigned long startMillis;


int resolution = 8;
int f_initial = 0;
int bloque = 0;

int reception;



TaskParams Parameters = {0, 0, 0, 0};



TaskHandle_t vavancerHandle = NULL;






TaskHandle_t vtournerHandle = NULL;





void vstrat(void *pvParameters){
  int distance = 100;
  int angle=90;
  
  Serial.println("commence");
  avancer(500,0,10);
  tourner(90,1,0); //0 droite, 1 gauche
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
  
  
  //delay(2000);
  int distance = 1000;
  int angle=90;
  
  if(modeBluetooth){
    xTaskCreate(
      vcontrole_bluetooth, /* Task function. */
      "vcontrole_bluetooth", /* name of task. */
      1000, /* Stack size of task */
      NULL, /* parameter of the task */
      1, /* priority of the task */
      NULL/* Task handle to keep track of created task */
    ); 
  }
  else{
    xTaskCreate(
      vstrat, /* Task function. */
      "vstrat", /* name of task. */
      4096, /* Stack size of task */
      &angle, /* parameter of the task */
      1, /* priority of the task */
      &vstratHandle/* Task handle to keep track of created task */
    ); 
  }


}


//un tour = 3200 step

void loop() {

}
