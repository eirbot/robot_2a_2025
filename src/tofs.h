#ifndef TOFS_H
#define TOFS_H

#include "Arduino.h"
#include "common.h"
//#include "Adafruit_VL53L0X.h" //la bibliothèque d'antonin ne permet pas facilement de changer l'adresse du tof 
//je vais utiliser <VL53L0X.h> meme si l'api est plus compliqué
#include <Wire.h>
#include <VL53L0X.h>
#include <SparkFun_I2C_Mux_Arduino_Library.h>
#include "actionneur.h"

#define NB_TOFS 3

extern volatile bool frontClear_tof; // Ensure this is declared as extern
extern volatile bool backClear_tof;  // Ensure this is declared as extern

static QueueHandle_t tofQueue;

void readTofs(void *Parameters_temp);

#endif