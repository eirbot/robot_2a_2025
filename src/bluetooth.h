#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <Arduino.h>
#include "BluetoothSerial.h"
#include "common.h"

extern BluetoothSerial SerialBT;


void moteur_droit(int vitesse,int sens);
void moteur_gauche(int vitesse,int sens);
void vcontrole_bluetooth(void *pvParameters);

#endif
