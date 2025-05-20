#ifndef COMMANDE_SERIE_H
#define COMMANDE_SERIE_H

#include "Arduino.h"
#include "strat.h"
#include "main_robot.h"
#include "GoToPosition.hpp"
#include "terminal_bluetooth.h"
#include "ClassMotors.hpp"
#include "common.h"

extern int resolution;
extern int bloque;

void commande();

#endif