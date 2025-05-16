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

class ClassMotors{
public:
    ClassMotors();
    static void vMotors(void* pvParameters);

    void WaitUntilDone();
    void StartMotors();
    void EnvoyerDonnees(void * Params);
    void TransferQueueBuffer();
    void RestoreQueueBuffer();

    long GetStepDid() const { return stepDid; }
    long GetCurrentStep() const { return currentStep; }
    float GetDistanceDid() const { return distanceDid; }

private:
    QueueHandle_t xQueue;
    QueueHandle_t xQueueBuffer; // File tampon

    long stepDid;
    long currentStep;
    float distanceDid;
};

void RampDownAccelStepper(AccelStepper& moteur1, AccelStepper& moteur2);
void RampUpAccelStepper(AccelStepper& moteur1, AccelStepper& moteur2, float targetSpeed, float stepAccel = STEP_ACCEL);

extern ClassMotors mot;

#endif