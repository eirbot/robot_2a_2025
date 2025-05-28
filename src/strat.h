#ifndef STRAT_H
#define STRAT_H

#include "Arduino.h"
#include "ClassMotors.hpp"
#include "GoToPosition.hpp"
#include "main_robot.h"
#include "common.h"
#include "actionneur.h"

void DoStrat(void* param);
void StopMatch();
void retourBase();
void inverseur(float* x, float* angle);

void vstrat0();
void vstrat1();
void vstrat2();
void vstrat3();
void vstrat4();
void vstrat5();


#endif