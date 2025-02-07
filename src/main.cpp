//Pour le PlatformIO

#include "Arduino.h"

//Code de base

#include "BluetoothSerial.h"


#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

unsigned long startMillis;

/*
int ENA = 19;
int IN1 = 21;
int IN2 = 22;
int IN3 = 18;
int IN4 = 17;
int ENB = 16;
*/

int ENA = 18;
int IN1 = 22;
//int IN2 = 22;
int IN3 = 21;
//int IN4 = 17;
int ENB = 19;

int resolution = 8;
int f_initial = 0;
int bloque = 0;

int reception;
int vitesse_droite_faible=0;
int vitesse_gauche_faible=0;
int vitesse_droite_fort=0;
int vitesse_gauche_fort=0;

int vitesse_gauche_fort_precedent=0;

int8_t vitesse_droite_recep=0;
int8_t vitesse_gauche_recep=0;

int vitesse_droite=0;
int vitesse_gauche=0;

unsigned long interval_m =10000;

void moteur_droit(int vitesse,int sens){  //probleme ici
  ledcWriteTone(1, vitesse);
  ledcWrite(1, 127);
  //analogWrite(ENA, vitesse);
  digitalWrite(IN1, sens);
  //digitalWrite(IN2, (1-sens));
}

void moteur_gauche(int vitesse,int sens){
  ledcWriteTone(2, vitesse);
  ledcWrite(2, 127);
  //analogWrite(ENB, vitesse);
  digitalWrite(IN3, sens);
  //digitalWrite(IN4, (1-sens));
}


void setup() {
  Serial.begin(115200);
  startMillis = millis();

  SerialBT.begin("ESP32test");
  SerialBT.setTimeout(50);


  ledcSetup(ENA, 1000,resolution);
  ledcAttachPin(ENA,1);
  //pinMode(ENA, OUTPUT);
  pinMode(IN1,OUTPUT);
  //pinMode(IN2,OUTPUT);

  ledcSetup(ENB, 1000,resolution);
  ledcAttachPin(ENB,2);
  //pinMode(ENB, OUTPUT);
  pinMode(IN3,OUTPUT);
  //pinMode(IN4,OUTPUT);
}

void loop() {

  if(millis()-interval_m>=0){

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
    
    
    

    interval_m=millis();
  }
  
  
}
