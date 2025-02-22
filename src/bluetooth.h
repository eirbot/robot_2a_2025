#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <Arduino.h>
#include "BluetoothSerial.h"

extern BluetoothSerial SerialBT;

#define STEPD 18  // anciennement STEPD
#define DIRD 22  // anciennement DIRD
//int IN2 = 22;
#define DIRG 21  // anciennement DIRG
//int IN4 = 17;
#define STEPG 19  // anciennement STEPG

void moteur_droit(int vitesse,int sens);
void moteur_gauche(int vitesse,int sens);
void vcontrole_bluetooth(void *pvParameters);

#endif
