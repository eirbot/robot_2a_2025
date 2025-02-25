#include "actionneur.h"


#define dRoues 100.0
#define ecartRoues 253.0
#define stepPerRev 3200

Servo gServo;
Servo dServo;
Servo hServo;

#define motorIN1 13
#define motorIN2 12

void vsetup_actionneurs(void *pvParameters){

  pinMode(motorIN1, OUTPUT);
  pinMode(motorIN2, OUTPUT);

  gServo.setPeriodHertz(50); // standard 50 hz servo
	gServo.attach(14, 1000, 2000); // Attach the servo after it has been detatched

  dServo.setPeriodHertz(50); // standard 50 hz servo
	dServo.attach(27, 1000, 2000); // Attach the servo after it has been detatched

  hServo.setPeriodHertz(50); // standard 50 hz servo
	hServo.attach(25, 1000, 2000); // Attach the servo after it has been detatched

  hServo.write(160);
  gServo.write(0);
  dServo.write(60);

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