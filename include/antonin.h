#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>
#include <ESP32Servo.h>
#include <HardwareSerial.h>
#include <driver/mcpwm.h>
#include "soc/mcpwm_periph.h"
#include "BluetoothSerial.h"

// Function declarations
void setSpeed(int motor, int frequency);
void avancer(int distance_mm, int speed);
void reculer(int distance_mm, int speed);
void gauche(int angle, int speed);
void droite(int angle, int speed);
void pousserCanettes();
void avancerCanettes();
void reculerCanettes();
void arreterCanettes();
void stop();
void freeWheel(char onOff);
void setDir(char dir);

#endif // MAIN_H

