#ifndef CLASSMOTORS_H
#define CLASSMOTORS_H

#include "Arduino.h"
#include <cmath>

class ClassMotors{
public:
    ClassMotors();
    static void vMotors(void* pvParameters);

    void StartMotors();
    void EnvoyerDonnees(int value);
private:
    QueueHandle_t xQueue;
};




#endif