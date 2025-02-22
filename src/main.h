#ifndef MAIN_H
#define MAIN_H

#include "Arduino.h"

#include "common.h"
#include "bluetooth.h"
#include "motors.h"
#include "commande_serie.h"

BluetoothSerial SerialBT;

TaskParams Parameters = {0, 0, 0, 0};

TaskHandle_t vavancerHandle = NULL;
TaskHandle_t vtournerHandle = NULL;

unsigned long startMillis;
int resolution = 8;
int bloque = 0;
bool modeBluetooth = false;//choix du mode bluetooth ou strategique


#define tourner(angle,direction,vitesse) \
  Parameters = {0, angle, direction, vitesse};\
  xTaskCreate(vtourner,"vtourner", 1000, &Parameters, 1, &vtournerHandle); \
  ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

#define avancer(distance,direction,vitesse) \
  Parameters = {distance, 0, direction, vitesse};\
  xTaskCreate(vavancer,"vavancer", 1000, &Parameters, 1, &vavancerHandle); \
  ulTaskNotifyTake(pdTRUE, portMAX_DELAY);


#endif