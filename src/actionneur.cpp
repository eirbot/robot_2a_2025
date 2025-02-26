#include "actionneur.h"

Servo gServo;
Servo dServo;
Servo hServo;
Servo bServo;

void vsetup_actionneurs(void *pvParameters){

  pinMode(motorIN1, OUTPUT);
  pinMode(motorIN2, OUTPUT);

  gServo.setPeriodHertz(50); // standard 50 hz servo
	gServo.attach(14, 1000, 2000); // Attach the servo after it has been detatched

  dServo.setPeriodHertz(50); // standard 50 hz servo
	dServo.attach(27, 1000, 2000); // Attach the servo after it has been detatched

  hServo.setPeriodHertz(50); // standard 50 hz servo
	hServo.attach(25, 1000, 2000); // Attach the servo after it has been detatched

  bServo.setPeriodHertz(50); // standard 50 hz servo
  bServo.attach(32, 1000, 2000); // Attach the servo after it has been detatched

  hServo.write(160);
  gServo.write(0);
  dServo.write(60);
  bServo.write(110);

  vTaskDelete(NULL);
}

void pousserCanettes(){
  hServo.write(0);
  delay(500);
  avancerCanettes();
  delay(900);
  reculerCanettes();
  delay(900);
  arreterCanettes();
  gServo.write(60);
  dServo.write(0);
  hServo.write(160);
  delay(800);
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
  gServo.write(angle);
}

void dServof(int angle){
  dServo.write(angle);
}

void hServof(int angle){
  hServo.write(angle);
}

void bServof(int angle){
  bServo.write(angle);
}