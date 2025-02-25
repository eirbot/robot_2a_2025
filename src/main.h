#ifndef MAIN_H
#define MAIN_H

#include "Arduino.h"

#include "common.h"
#include "bluetooth.h"
#include "motors.h"
#include "commande_serie.h"
#include "actionneur.h"

extern BluetoothSerial SerialBT;

extern TaskParams Parameters;

extern int resolution;
extern int bloque;
extern bool modeBluetooth;

extern unsigned long startMillis;

#endif