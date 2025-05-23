#ifndef COMMON_H
#define COMMON_H

#include "esp_task_wdt.h"
#include "ClassMotors.hpp"
#include "GoToPosition.hpp"
#include "AccelStepper.h"

// a determiner pour les pins des btn et switch
#define BTNG 36
#define BTND 39
#define SWITCH1 34 //Gauche
#define SWITCH2 35
#define SWITCH3 P10
#define SWITCH4 P11 //Droite

#define STEPD 17
#define DIRD 16 
#define STEPG 4
#define DIRG 2

#define SPEEDMAX 1000
#define ACCELMAX 3000

#define MOTOR_G 6
#define MOTOR_D 5

#define dRoues 100.0
#define stepPerRev 3200
#define ecartRoues 253.0

#define vitesse_nominale 100
#define stop_distance 200

typedef struct {
    int distance;
    int angle;
    int direction;
    int vitesse;
} TaskParams;

extern TaskHandle_t vavancerHandle;
extern TaskHandle_t vtournerHandle;

// Déclaration des deux moteurs (type DRIVER = step/dir)
extern AccelStepper moteurGauche; 
extern AccelStepper moteurDroit; 

extern TaskHandle_t vstratHandle;
extern TaskHandle_t vterminal_bluetoothHandle;

extern volatile bool* FLAG_CLEAR;
extern bool FLAG_STOP;

#endif