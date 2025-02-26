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



extern TaskHandle_t vstratHandle;

#endif