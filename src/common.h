#ifndef COMMON_H
#define COMMON_H

#include "esp_task_wdt.h"

#define STEPD 18 
#define DIRD 22  
#define DIRG 21
#define STEPG 19


#define dRoues 100.0
#define stepPerRev 3200
#define ecartRoues 253.0

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


extern TaskHandle_t vstratHandle;

#endif