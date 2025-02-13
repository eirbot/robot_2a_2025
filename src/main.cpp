//Pour le PlatformIO

#include "Arduino.h"
#include "main.h"

//Code de base

#include "BluetoothSerial.h"

#define dRoues 100.0
#define stepPerRev 3200
#define ecartRoues 253.0

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

unsigned long startMillis;


#define STEPD 18  // anciennement ENA
#define DIRD 22  // anciennement IN1
//int IN2 = 22;
#define DIRG 21  // anciennement IN3
//int IN4 = 17;
#define STEPG 19  // anciennement ENB

int resolution = 8;
int f_initial = 0;
int bloque = 0;

int reception;


int8_t vitesse_droite_recep=0;
int8_t vitesse_gauche_recep=0;

int vitesse_droite=0;
int vitesse_gauche=0;


void moteur_droit(int vitesse,int sens){
  ledcWriteTone(1, vitesse);
  ledcWrite(1, 127);
  //analogWrite(ENA, vitesse);
  digitalWrite(DIRD, sens);
  //digitalWrite(IN2, (1-sens));
}

void moteur_gauche(int vitesse,int sens){
  ledcWriteTone(2, vitesse);
  ledcWrite(2, 127);
  //analogWrite(ENB, vitesse);
  digitalWrite(DIRG, sens);
  //digitalWrite(IN4, (1-sens));
}

TaskHandle_t vavancerHandle = NULL;  // Handle global pour suivre vavancer

void vavancer(void *Parameters_temp){
  TaskParams *Parameters = (TaskParams *)Parameters_temp;
  if(Parameters->direction == 0){
    digitalWrite(DIRD,LOW);
    digitalWrite(DIRG,LOW);
  }
  else{
    digitalWrite(DIRD,HIGH);
    digitalWrite(DIRG,HIGH);
  }
  int steps = (*(int *)Parameters->distance / (3.14 * dRoues)) * stepPerRev;
  for(int k=0 ; k < steps; k++){
    digitalWrite(STEPD,HIGH);
    digitalWrite(STEPG,HIGH);
    delayMicroseconds(500/Parameters->vitesse);
    digitalWrite(STEPD,LOW);
    digitalWrite(STEPG,LOW);
    delayMicroseconds(500/Parameters->vitesse);
  }

  // Notification que la tâche est terminée
  xTaskNotifyGive(vavancerHandle);

  vTaskDelete(NULL);
}

void avancer(int distance, char direction, int vitesse){
  TaskParams Parameters = {distance, 0, direction, vitesse};
  xTaskCreate(
    vavancer, /* Task function. */
    "vavancer", /* name of task. */
    1000, /* Stack size of task */
    &Parameters, /* parameter of the task */
    1, /* priority of the task */
    &vavancerHandle/* Task handle to keep track of created task */
  ); 
}

TaskHandle_t vtournerHandle = NULL;  // Handle global pour suivre vavancer

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
  int steps = (*(int*)Parameters->angle / 360.0) * (3.14 * ecartRoues) * stepPerRev / (3.14 * dRoues);
  for(int k=0 ; k < steps; k++){
    digitalWrite(STEPD,HIGH);
    digitalWrite(STEPG,HIGH);
    delayMicroseconds(500/Parameters->vitesse);
    digitalWrite(STEPD,LOW);
    digitalWrite(STEPG,LOW);
    delayMicroseconds(500/Parameters->vitesse);
  }

  // Notification que la tâche est terminée
  xTaskNotifyGive(vtournerHandle);
  
  vTaskDelete(NULL);
}

void tourner(int angle, char direction, int vitesse){
  TaskParams Parameters = {0, angle, direction, vitesse};
  xTaskCreate(
    vtourner, /* Task function. */
    "vtourner", /* name of task. */
    1000, /* Stack size of task */
    &Parameters,  /* parameter of the task */
    1, /* priority of the task */
    &vtournerHandle/* Task handle to keep track of created task */
  ); 
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

void vStrat(void *pvParameters)
{
  while(1){
    avancer(1000, 1, 1);
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    avancer(1000, 0, 1);
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
  }
}



void setup() {
  Serial.begin(115200);
  startMillis = millis();

  SerialBT.begin("ESP32test");
  SerialBT.setTimeout(50);


  // ledcSetup(ENA, 1000,resolution);
  // ledcAttachPin(ENA,1);
  pinMode(STEPD,OUTPUT);

  pinMode(DIRD,OUTPUT);
  //pinMode(IN2,OUTPUT);


  // ledcSetup(ENB, 1000,resolution);
  // ledcAttachPin(ENB,2);
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
  
  xTaskCreate(
    vStrat, /* Task function. */
    "vStrat", /* name of task. */
    1000, /* Stack size of task */
    NULL, /* parameter of the task */
    3, /* priority of the task */
    NULL/* Task handle to keep track of created task */
  ); 
  
  delay(2000);
  
}


//un tour = 3200 step

void loop() {
  // int distance = 1000;
  // int angle=90;

  // xTaskCreate(
  //   vavancer, /* Task function. */
  //   "vavancer", /* name of task. */
  //   1000, /* Stack size of task */
  //   &distance, /* parameter of the task */
  //   1, /* priority of the task */
  //   NULL/* Task handle to keep track of created task */
  // ); 


  // xTaskCreate(
  //   vtourner, /* Task function. */
  //   "vtourner", /* name of task. */
  //   1000, /* Stack size of task */
  //   &angle, /* parameter of the task */
  //   1, /* priority of the task */
  //   NULL/* Task handle to keep track of created task */
  // ); 
  // delay(1000000);

  
}
