#ifndef ACTIONNEUR_H
#define ACTIONNEUR_H

#include <Arduino.h>
#include <ESP32Servo.h>
#include <HardwareSerial.h>
#include <driver/mcpwm.h>
#include "soc/mcpwm_periph.h"
#include "BluetoothSerial.h"

#define motorIN1 13
#define motorIN2 12

#define gServoPin 14
#define dServoPin 27
#define hServoPin 25
#define bServoPin 32

// Function declarations
void pousserCanettes();
void avancerCanettes();
void reculerCanettes();
void arreterCanettes();
void gServof(int angle);
void dServof(int angle);
void hServof(int angle);
void bServof(int angle);
void vsetup_actionneurs(void *pvParameters);

#endif

