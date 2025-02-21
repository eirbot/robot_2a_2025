//Pour le PlatformIO

#include "Arduino.h"

//Code de base

#include "BluetoothSerial.h"

#include "esp_task_wdt.h"

#define dRoues 100.0
#define stepPerRev 3200
#define ecartRoues 253.0

#define STEPD 18  // anciennement STEPD
#define DIRD 22  // anciennement DIRD
//int IN2 = 22;
#define DIRG 21  // anciennement DIRG
//int IN4 = 17;
#define STEPG 19  // anciennement STEPG

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


BluetoothSerial SerialBT;

unsigned long startMillis;


int resolution = 8;
int f_initial = 0;
int bloque = 0;

int reception;


int8_t vitesse_droite_recep=0;
int8_t vitesse_gauche_recep=0;

int vitesse_droite=0;
int vitesse_gauche=0;

typedef struct {
  int distance;
  int angle;
  int direction;
  int vitesse;
} TaskParams;

TaskParams Parameters = {0, 0, 0, 0};

TaskHandle_t vstratHandle = NULL;


void moteur_droit(int vitesse,int sens){
  ledcWriteTone(1, vitesse);
  ledcWrite(1, 127);
  //analogWrite(STEPD, vitesse);
  digitalWrite(DIRD, sens);
  //digitalWrite(IN2, (1-sens));
}

void moteur_gauche(int vitesse,int sens){
  ledcWriteTone(2, vitesse);
  ledcWrite(2, 127);
  //analogWrite(STEPG, vitesse);
  digitalWrite(DIRG, sens);
  //digitalWrite(IN4, (1-sens));
}

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




void vcontrole_bluetooth(void *pvParameters)
{
    for( ;; )
    {
      while(SerialBT.available()){
        vitesse_gauche_recep = SerialBT.read();
        vitesse_droite_recep = SerialBT.read();
  
        vitesse_droite = (int)vitesse_droite_recep*50;
        vitesse_gauche = (int)vitesse_gauche_recep*50;
        
  
        if(vitesse_droite>0){
          moteur_droit(vitesse_droite,1);
        }
        else if(vitesse_droite<0){
          moteur_droit(-vitesse_droite,0);
        }
        else{
          moteur_droit(0,0);
        }
        if(vitesse_gauche>0){
          moteur_gauche(vitesse_gauche,0); 
        }
        else if(vitesse_gauche<0){
          moteur_gauche(-vitesse_gauche,1);
        }
        else{
          moteur_droit(0,0);
        }     
      }
    }
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

  Serial.begin(115200);
  startMillis = millis();

  SerialBT.begin("ESP32test");
  SerialBT.setTimeout(50);


  // ledcSetup(STEPD, 1000,resolution);
  // ledcAttachPin(STEPD,1);
  pinMode(STEPD,OUTPUT);

  pinMode(DIRD,OUTPUT);
  //pinMode(IN2,OUTPUT);


  // ledcSetup(STEPG, 1000,resolution);
  // ledcAttachPin(STEPG,2);
  pinMode(STEPG,OUTPUT);
  pinMode(DIRG,OUTPUT);

  // xTaskCreate(
  //   vcontrole_bluetooth, /* Task function. */
  //   "vcontrole_bluetooth", /* name of task. */
  //   1000, /* Stack size of task */
  //   NULL, /* parameter of the task */
  //   1, /* priority of the task */
  //   NULL/* Task handle to keep track of created task */
  // ); 
  
  
  delay(2000);
  int distance = 1000;
  int angle=90;
  
  xTaskCreate(
    vstrat, /* Task function. */
    "vstrat", /* name of task. */
    4096, /* Stack size of task */
    &angle, /* parameter of the task */
    1, /* priority of the task */
    &vstratHandle/* Task handle to keep track of created task */
  ); 

  // xTaskCreate(
  //   vtourner, /* Task function. */
  //   "vtourner", /* name of task. */
  //   1000, /* Stack size of task */
  //   &angle, /* parameter of the task */
  //   1, /* priority of the task */
  //   &vtournerHandle/* Task handle to keep track of created task */
  // ); 
  // Serial.println("commence");
  // ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
  // Serial.println("fini");

}


//un tour = 3200 step

void loop() {
  
  // xTaskCreate(
  //   vavancer, /* Task function. */
  //   "vavancer", /* name of task. */
  //   1000, /* Stack size of task */
  //   &distance, /* parameter of the task */
  //   1, /* priority of the task */
  //   NULL/* Task handle to keep track of created task */
  // ); 

}
