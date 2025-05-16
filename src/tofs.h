#ifndef TOFS_H
#define TOFS_H

#include "Arduino.h"
#include "common.h"
#include "Adafruit_VL53L0X.h"

extern volatile bool frontClear_tof; // Ensure this is declared as extern
extern volatile bool backClear_tof;  // Ensure this is declared as extern

static QueueHandle_t tofQueue;

void readTofs(void *Parameters_temp);

#endif