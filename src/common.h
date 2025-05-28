#ifndef COMMON_H
#define COMMON_H


#define TEMPS_MATCH_ROBOT 85000

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

#define SPEEDMAX 500
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
#include "menu.hpp"

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


extern Page Test_total;
extern Page Test_Pince;
extern Page Test_Servo;
extern Page Test_Pousseur;
 
extern Page Test_actionneurs;

extern Page test_baniere;
extern Page Test_vox;

extern Page Test;

extern Page PositionX;
extern Page PositionY;
extern Page PositionAngle;

extern Page Position;

extern Page Strat_1;
extern Page Strat_2;
extern Page Strat_3;
extern Page Strat_4;

extern Page Strat;

extern Page Tof_1;
extern Page Tof_2;
extern Page Tof_3;
extern Page Tof_4;
extern Page Tof_5;
extern Page Tof_6;

extern Page Tof;

extern Page Menu_principal;

#endif