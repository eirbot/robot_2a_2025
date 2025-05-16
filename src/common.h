#ifndef COMMON_H
#define COMMON_H

#include "esp_task_wdt.h"
#include "ClassMotors.hpp"
#include "GoToPosition.hpp"
#include "AccelStepper.h"

// a determiner pour les pins des btn et switch
#define BTNG 34
#define BTND 35
#define SWITCH1 39 //Gauche
#define SWITCH2 36
#define SWITCH3 37
#define SWITCH4 38 //Droite

#define STEPD 18 
#define DIRD 23  
#define DIRG 5
#define STEPG 19

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