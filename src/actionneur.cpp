#include "actionneur.h"

Servo bgServo;
Servo bdServo;
Servo hdServo;
Servo hgServo;

void vsetup_actionneurs(void *pvParameters){

  pinMode(motorIN1, OUTPUT);
  pinMode(motorIN2, OUTPUT);
  pinMode(BANNIERE, OUTPUT);

  bgServo.setPeriodHertz(50); // standard 50 hz servo
	bgServo.attach(14, 1000, 2000); // Attach the servo after it has been detatched

  bdServo.setPeriodHertz(50); // standard 50 hz servo
	bdServo.attach(27, 1000, 2000); // Attach the servo after it has been detatched

  hdServo.setPeriodHertz(50); // standard 50 hz servo
	hdServo.attach(25, 1000, 2000); // Attach the servo after it has been detatched

  hgServo.setPeriodHertz(50); // standard 50 hz servo
  hgServo.attach(32, 1000, 2000); // Attach the servo after it has been detatched

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
  bgServo.write(170);
  bdServo.write(0);
  hgServo.write(0);
  hdServo.write(160);
  vTaskDelay(800);
}
void startBanniere(){
  digitalWrite(BANNIERE, HIGH);
  vTaskDelay(100);
  digitalWrite(BANNIERE, LOW);
}

void DoBanniere(){
  digitalWrite(BANNIERE, HIGH);
  vTaskDelay(100);
  digitalWrite(BANNIERE, LOW);
  vTaskDelay(100);

  pinMode(BANNIERE, INPUT); // Set the pin to input mode to disable the pull-up resistor
  while(!digitalRead(BANNIERE)){
    vTaskDelay(100);
  } 
}

void resetActionneurs(){
  hgServo.write(0);
  bgServo.write(0);
  bdServo.write(170);
  hdServo.write(160);
}

void avancerCanettes() {
  digitalWrite(motorIN1, LOW); 
  digitalWrite(motorIN2, HIGH); 
}

void reculerCanettes() {
  digitalWrite(motorIN1, HIGH); 
  digitalWrite(motorIN2, LOW); 
}

void arreterCanettes() {
  digitalWrite(motorIN1, LOW); 
  digitalWrite(motorIN2, LOW); 
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