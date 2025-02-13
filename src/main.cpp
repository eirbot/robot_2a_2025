//Pour le PlatformIO

#include "Arduino.h"

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


int ENA = 18;
int IN1 = 22;
int IN3 = 21;
int ENB = 19;

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
  digitalWrite(IN1, sens);
}

void moteur_gauche(int vitesse,int sens){
  ledcWriteTone(2, vitesse);
  ledcWrite(2, 127);
  digitalWrite(IN3, sens);
}

void vavancer(void *pvParameters){
  digitalWrite(IN1,HIGH);
  digitalWrite(IN3,HIGH);
  int steps = (*(int *)pvParameters / (3.14 * dRoues)) * stepPerRev;
  for(int k=0 ; k < steps; k++){
    digitalWrite(ENA,HIGH);
    digitalWrite(ENB,HIGH);
    delayMicroseconds(100);
    digitalWrite(ENA,LOW);
    digitalWrite(ENB,LOW);
    delayMicroseconds(1000);
  }
  vTaskDelete(NULL);
}

void vtourner(void *pvParameters){
  digitalWrite(IN1,HIGH);
  digitalWrite(IN3,LOW);
  int steps = (*(int*)pvParameters / 360.0) * (3.14 * ecartRoues) * stepPerRev / (3.14 * dRoues);
  for(int k=0 ; k < steps; k++){
    digitalWrite(ENA,HIGH);
    digitalWrite(ENB,HIGH);
    delay(1);
    digitalWrite(ENA,LOW);
    digitalWrite(ENB,LOW);
    delay(1);
  }
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

void setup() {
  Serial.begin(115200);
  startMillis = millis();

  SerialBT.begin("ESP32test");
  SerialBT.setTimeout(50);

  // ledcSetup(ENA, 1000,resolution);
  // ledcAttachPin(ENA,1);
  pinMode(ENA,OUTPUT);
  pinMode(IN1,OUTPUT);

  // ledcSetup(ENB, 1000,resolution);
  // ledcAttachPin(ENB,2);
  pinMode(ENB,OUTPUT);
  pinMode(IN3,OUTPUT);

  // xTaskCreate(
  //   vcontrole_bluetooth, /* Task function. */
  //   "vcontrole_bluetooth", /* name of task. */
  //   1000, /* Stack size of task */
  //   NULL, /* parameter of the task */
  //   1, /* priority of the task */
  //   NULL/* Task handle to keep track of created task */
  // ); 
  
  
  delay(2000);
  
}


//un tour = 3200 step

void loop() {
  int distance = 1000;
  int angle=90;
  // xTaskCreate(
  //   vavancer, /* Task function. */
  //   "vavancer", /* name of task. */
  //   1000, /* Stack size of task */
  //   &distance, /* parameter of the task */
  //   1, /* priority of the task */
  //   NULL/* Task handle to keep track of created task */
  // ); 


  xTaskCreate(
    vtourner, /* Task function. */
    "vtourner", /* name of task. */
    1000, /* Stack size of task */
    &angle, /* parameter of the task */
    1, /* priority of the task */
    NULL/* Task handle to keep track of created task */
  ); 
  delay(1000000);
  
}
