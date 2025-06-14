#ifndef ACTIONNEUR_H
#define ACTIONNEUR_H

#include <Arduino.h>
#include <ESP32Servo.h>
#include <HardwareSerial.h>
#include <driver/mcpwm.h>
#include "soc/mcpwm_periph.h"
#include "BluetoothSerial.h"
#include "common.h"
#include "PCF8575.h"

extern PCF8575 pcf8575;

#define motorIN1 P3
#define motorIN2 P2

#define BANNIERE 33

#define tirette 32

#define bgServoPin 25
#define bdServoPin 26
#define hgServoPin 27
#define hdServoPin 14

// Function declarations
void pousserCanettes();
void avancerCanettes();
void reculerCanettes();
void arreterCanettes();
void baisserBras();
void monterBras();
void demiBras();
void aimantCanettes();
void lacherCanettes();

void DoBanniere();

void gServof(int angle);
void dServof(int angle);
void hServof(int angle);
void bServof(int angle);
void vsetup_actionneurs(void *pvParameters);
void resetActionneurs();
// bool checkSwitches(int switchNumber);

#endif

