#include "common.h"

TaskHandle_t vavancerHandle = NULL;
TaskHandle_t vtournerHandle = NULL;

AccelStepper moteurGauche(AccelStepper::DRIVER, STEPG, DIRG); // STEP, DIR
AccelStepper moteurDroit(AccelStepper::DRIVER, STEPD, DIRD);  // STEP, DIR

ClassMotors mot;