//Pour le PlatformIO

#include "Arduino.h"

//Code de base

#include "BluetoothSerial.h"


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


  ledcSetup(ENA, 1000,resolution);
  ledcAttachPin(ENA,1);
  pinMode(IN1,OUTPUT);

  ledcSetup(ENB, 1000,resolution);
  ledcAttachPin(ENB,2);
  pinMode(IN3,OUTPUT);

  xTaskCreate(
    vcontrole_bluetooth, /* Task function. */
    "vcontrole_bluetooth", /* name of task. */
    1000, /* Stack size of task */
    NULL, /* parameter of the task */
    1, /* priority of the task */
    NULL); /* Task handle to keep track of created task */

}




void loop() {


  
}
