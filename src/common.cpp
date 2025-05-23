#include "common.h"

TaskHandle_t vavancerHandle = NULL;
TaskHandle_t vtournerHandle = NULL;

AccelStepper moteurGauche(AccelStepper::DRIVER, STEPG, DIRG); // STEP, DIR
AccelStepper moteurDroit(AccelStepper::DRIVER, STEPD, DIRD);  // STEP, DIR

ClassMotors mot;

volatile bool* FLAG_CLEAR = NULL; // Valeur initiale (1 = continue, 0 = stop)
bool FLAG_STOP = false; // Valeur initiale (1 = stop, 0 = continue)

SemaphoreHandle_t xPositionMutex;