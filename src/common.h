#ifndef COMMON_H
#define COMMON_H


#define TEMPS_MATCH_ROBOT 85000

#define BTNG 36
#define BTND 39
#define SWITCH1 34 //Gauche
#define SWITCH2 35
#define SWITCH3 12  
#define SWITCH4 13 //Droite

#define STEPD 17
#define DIRD 16 
#define STEPG 4
#define DIRG 2

#define SPEEDMAX 800
#define ACCELMAX 3000


#define MOTOR_G 6
#define MOTOR_D 5

#define dRoues 100.0
#define stepPerRev 3200
#define ecartRoues 253.0

#define vitesse_nominale 100
#define STOP_DISTANCE 200

extern float X_POS_INIT;
extern float Y_POS_INIT;
extern float ANGLE_INIT;

#include "esp_task_wdt.h"
#include "ClassMotors.hpp"
#include "GoToPosition.hpp"
#include "AccelStepper.h"

typedef struct {
    int distance;
    int angle;
    int direction;
    int vitesse;
} TaskParams;

// Déclaration des deux moteurs (type DRIVER = step/dir)
extern AccelStepper moteurGauche; 
extern AccelStepper moteurDroit; 

extern TaskHandle_t vstratHandle;
extern TaskHandle_t vterminal_bluetoothHandle;
extern TaskHandle_t vMotorsHandle;
extern TaskHandle_t handleDoStrat;


extern SemaphoreHandle_t xPositionMutex;

extern volatile bool* FLAG_CLEAR;
extern bool FLAG_STOP;
extern bool FLAG_TIRETTE;
extern bool FLAG_DEBUG;
extern bool FLAG_TOF;

extern bool jaune;



#endif