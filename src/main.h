#ifndef MAIN_H
#define MAIN_H

#include "Arduino.h"
#include "common.h"
#include "commande_serie.h"
#include "actionneur.h"
#include "tofs.h"

extern BluetoothSerial SerialBT;

extern TaskParams Parameters;

extern int resolution;
extern int bloque;
extern bool modeBluetooth;

extern unsigned long startMillis;

extern bool initial_tbluetooth;

extern GoToPosition serialGoto;

#endif
