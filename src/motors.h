#ifndef MOTORS_H
#define MOTORS_H


#include "Arduino.h"
#include <cmath>

#include "common.h"
#include "tofs.h"

typedef struct {
    int distance;
    int angle;
    int direction;
    int vitesse;
} TaskParams;

extern TaskParams Parameters;

void vavancer(void *Parameters_temp);
void vtourner(void *Parameters_temp);

#endif