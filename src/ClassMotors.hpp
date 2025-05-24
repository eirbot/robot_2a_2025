#ifndef CLASSMOTORS_H
#define CLASSMOTORS_H

#include "Arduino.h"
#include <cmath>

#include "common.h"
#include "AccelStepper.h"

#define DECCEL 9000.0

class ClassMotors{
public:
    ClassMotors();
    static void vMotors(void* pvParameters);

    void WaitUntilDone();
    void StartMotors();
    void EnvoyerDonnees(void * Params);
    void TransferQueueBuffer();
    void RestoreQueueBuffer();
    void Stop();
    void RestartMotors();

    long GetStepDid() const { return stepDid; }
    long GetCurrentStep() const { return currentStep; }
    float GetDistanceDid() const { return distanceDid; }
    void UpdateOdometry();

    void GetPosition(float &x, float &y, float &angle);
    void SetPosition(float x, float y, float angle);

private:
    QueueHandle_t xQueue;
    QueueHandle_t xQueueBuffer; // File tampon

    long stepDid;
    long currentStep;
    float distanceDid;

    float x_pos = X_POS_INIT;
    float y_pos = Y_POS_INIT;
    float orientation = ANGLE_INIT; // En radians
};

void StopStepper(AccelStepper& moteur1, AccelStepper& moteur2);

extern ClassMotors mot;

#endif