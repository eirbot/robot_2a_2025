#ifndef CLASSMOTORS_H
#define CLASSMOTORS_H

#include "Arduino.h"
#include <cmath>

#include "common.h"
#include "AccelStepper.h"

#define STEP_DECCEL 100000.0
#define STEP_ACCEL 10000.0

#define DIST_MAX 500.0
#define V_MIN 100.0
#define V_MAX 10000.0

#define ANGLE_MAX 180.0f 
#define V_ROT_MIN 100    
#define V_ROT_MAX 800    

class ClassMotors{
public:
    ClassMotors();
    static void vMotors(void* pvParameters);

    void WaitUntilDone();
    void StartMotors();
    void EnvoyerDonnees(void * Params);
    void ViderQueue();

    long GetStepRemain() const { return stepRemain; }
    long GetCurrentStep() const { return currentStep; }
    float GetAngleRemain() const { return angleRemain; }
    float GetDistanceRemain() const { return distanceRemain; }

private:
    QueueHandle_t xQueue;
    long stepRemain;
    long currentStep;
    float angleRemain;
    float distanceRemain;
};

void RampDownAccelStepper(AccelStepper& moteur1, AccelStepper& moteur2);
void RampUpAccelStepper(AccelStepper& moteur1, AccelStepper& moteur2, float targetSpeed, float stepAccel = STEP_ACCEL);

extern ClassMotors mot;

#endif