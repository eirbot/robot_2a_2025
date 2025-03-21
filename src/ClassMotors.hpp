#ifndef CLASSMOTORS_H
#define CLASSMOTORS_H

#include "Arduino.h"
#include <cmath>

#include "common.h"
#include "motors.h"

class ClassMotors{
public:
    ClassMotors();
    static void vMotors(void* pvParameters);

    void StartMotors();
    void EnvoyerDonnees(void * Params);
private:
    QueueHandle_t xQueue;
};


extern ClassMotors mot;

#endif