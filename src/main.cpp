//Pour le PlatformIO

#include "Arduino.h"
#include "bluetooth.h"
//Code de base


BluetoothSerial SerialBT;

#include "esp_task_wdt.h"

#define dRoues 100.0
#define stepPerRev 3200
#define ecartRoues 253.0


bool modeBluetooth = true;



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

typedef struct {
  int distance;
  int angle;
  int direction;
  int vitesse;
} TaskParams;

TaskParams Parameters = {0, 0, 0, 0};

TaskHandle_t vstratHandle = NULL;

TaskHandle_t vavancerHandle = NULL;




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

TaskHandle_t vtournerHandle = NULL;


void vtourner(void *Parameters_temp){
  TaskParams *Parameters = (TaskParams *)Parameters_temp;
  if(Parameters->direction == 0){
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
