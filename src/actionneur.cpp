#include "actionneur.h"
#include "PCF8575.h"

Servo bgServo;
Servo bdServo;
Servo hdServo;
Servo hgServo;

PCF8575 pcf8575(0x20);

void vsetup_actionneurs(void *pvParameters){

  pinMode(SWITCH1, INPUT);
  pinMode(SWITCH2, INPUT);
  pcf8575.pinMode(SWITCH3, INPUT);  
  pcf8575.pinMode(SWITCH4, INPUT); 

  pcf8575.pinMode(motorIN1, OUTPUT);
  pcf8575.pinMode(motorIN2, OUTPUT);
  pinMode(BANNIERE, OUTPUT);
  pinMode(tirette, INPUT_PULLUP);

  bgServo.setPeriodHertz(50); // standard 50 hz servo
	bgServo.attach(bgServoPin, 1000, 2000); // Attach the servo after it has been detatched

  bdServo.setPeriodHertz(50); // standard 50 hz servo
	bdServo.attach(bdServoPin, 1000, 2000); // Attach the servo after it has been detatched

  hdServo.setPeriodHertz(50); // standard 50 hz servo
	hdServo.attach(hdServoPin, 1000, 2000); // Attach the servo after it has been detatched

  hgServo.setPeriodHertz(50); // standard 50 hz servo
  hgServo.attach(hgServoPin, 1000, 2000); // Attach the servo after it has been detatched

  resetActionneurs();

  vTaskDelete(NULL);
}

void pousserCanettes(){
  hgServo.write(160);
  hdServo.write(0);
  bgServo.write(0);
  bdServo.write(170);
  vTaskDelay(800);
  avancerCanettes();
  vTaskDelay(900);
  reculerCanettes();
  vTaskDelay(900);
  arreterCanettes();
  hgServo.write(0);
  hdServo.write(160);
  bgServo.write(170);
  bdServo.write(0);
  vTaskDelay(800);
}

void DoBanniere(){
  TickType_t TimeOutBan = 0;
  digitalWrite(BANNIERE, HIGH);
  vTaskDelay(100);
  digitalWrite(BANNIERE, LOW);
  vTaskDelay(100);

  pinMode(BANNIERE, INPUT); // Set the pin to input mode to disable the pull-up resistor

  TimeOutBan = xTaskGetTickCount();
  while(!digitalRead(BANNIERE)){
    vTaskDelay(100);
    if((xTaskGetTickCount() - TimeOutBan) > 5000){
      break; // Stop after 5 seconds
    }
  } 
  pinMode(BANNIERE, OUTPUT);
}

void resetActionneurs(){
  hgServo.write(0);
  bgServo.write(0);
  bdServo.write(170);
  hdServo.write(160);
}

void avancerCanettes() {
  pcf8575.digitalWrite(motorIN1, LOW); 
  pcf8575.digitalWrite(motorIN2, HIGH); 
}

void reculerCanettes() {
  pcf8575.digitalWrite(motorIN1, HIGH); 
  pcf8575.digitalWrite(motorIN2, LOW); 
}

void arreterCanettes() {
  pcf8575.digitalWrite(motorIN1, LOW); 
  pcf8575.digitalWrite(motorIN2, LOW); 
}

void gServof(int angle){
  bgServo.write(angle);
}

void dServof(int angle){
  bdServo.write(angle);
}

void hServof(int angle){
  hgServo.write(angle);
}

void bServof(int angle){
  hdServo.write(angle);
}