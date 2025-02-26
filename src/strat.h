#ifndef STRAT_H
#define STRAT_H

#include "Arduino.h"
#include "motors.h"
#include "actionneur.h"

extern TaskHandle_t vavancerHandle;
extern TaskHandle_t vtournerHandle;

#define tourner(angle,direction,vitesse) \
  Parameters = {0, angle, direction, vitesse};\
  xTaskCreate(vtourner,"vtourner", 1000, &Parameters, 1, &vtournerHandle); \
  ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

#define avancer(distance,direction,vitesse) \
  Parameters = {distance, 0, direction, vitesse};\
  xTaskCreate(vavancer,"vavancer", 1000, &Parameters, 1, &vavancerHandle); \
  ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

void vstrat1(void *pvParameters);
void vstrat2(void *pvParameters);

#endif