#include "actionneur.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif
BluetoothSerial SerialBT;

/*
HardwareSerial to_moteur1(1);

// Stepper address ----------------------//
static uint8_t ADDR_STP_D = 0xE0;
static uint8_t ADDR_STP_G = 0xE1;
static uint8_t ADDR_STP_2 = 0xE2;
static uint8_t ADDR_STP_3 = 0xE3;
static uint8_t ADDR_STP_4 = 0xE4;
// Function constants -------------------//
static uint8_t set_speed_position = 0xFD;   //needs data & data_plus

static uint8_t set_speed = 0xF6;            //needs data
static uint8_t set_en_pin = 0xF3;           //needs data (00 or 01)
static uint8_t set_0_mode = 0x90;           //needs data (00 or 01)
static uint8_t set_0 = 0x91;                //needs data (00 or 01)
static uint8_t set_0_speed = 0x92;          //needs data 
static uint8_t set_0_dir = 0x93;            //needs data (00 or 01)
static uint8_t return_to_0 = 0x94;          //needs data (00 only )
static uint8_t sys_calibrate = 0x80;        //needs data (00 only)
static uint8_t sys_motor_type = 0x81;       //needs data (00 or 01 only)
static uint8_t sys_current = 0x83;          //needs data (00 to 0c)
static uint8_t sys_step_div = 0x84;         //needs data (from 1 to 256)
static uint8_t sys_set_en_pin = 0x85;       //needs data (00 or 01)
static uint8_t sys_set_dir = 0x86;          //needs data (00 or 01)



static uint8_t stop = 0xF7;                 //no data requiered
static uint8_t read_encoder = 0x30;         //no data requiered
static uint8_t read_pulse = 0x33;           //no data requiered
static uint8_t read_angle = 0x36;           //no data requiered
static uint8_t read_error = 0x39;           //no data requiered
static uint8_t read_en_status = 0x3A;       //no data requiered
static uint8_t read_shaft_status = 0x3E;    //no data requiered
static uint8_t sys_stall = 0x3E;    //no data requiered
// --------------------------------------//
uint64_t COMMAND;
uint8_t *DATA;
uint32_t *DATA_PLUS;
*/

#define dRoues 100.0
#define ecartRoues 253.0
#define stepPerRev 3200

Servo gServo;
Servo dServo;
Servo hServo;

// Broches du L298N
// #define motorPWM1 2
// #define motorPWM2 4
#define motorIN1 13
#define motorIN2 12
// #define motorIN3 19
// #define motorIN4 21

// Define the pins for the stepper motors
#define STEP_PIN_1 2
#define DIR_PIN_1 4
#define ENABLE_PIN_1 15

#define STEP_PIN_2 18
#define DIR_PIN_2 19
#define ENABLE_PIN_2 5

#define LEDC_TIMER_13_BIT  13
#define LEDC_BASE_FREQ     5000

// Define the LEDC channels
#define LEDC_CHANNEL_0     0
#define LEDC_CHANNEL_1     1

int a;
String id;
String valueStr;
int value;
int vit = 3200;
float temps10Tours = 25.8;
float vitesse_mmps = 3.14*ecartRoues*10/temps10Tours;

/*
uint8_t Speed(uint8_t Vrpm, uint8_t Mstep) {
    return uint8_t(Vrpm * (Mstep * 200) / 30000);
}

uint8_t Check(uint8_t addr, uint8_t func) {
    return (addr + func) & 0xFF;
}

int delaiDist(int distance_mm){
  float total = ((float)distance_mm)/vitesse_mmps;
  return (int) (total*1000);
}

int distVirage(int angle){
  return (int)(3.14*ecartRoues*(float)angle/360.0);
}

uint16_t Prefix(uint8_t addr, uint func) {
    return uint16_t(addr)<<8 | uint16_t(func);
}

void Set_params_no_data(uint8_t addr, uint8_t func){
    to_moteur1.write(addr);
    to_moteur1.write(func);
    to_moteur1.write(Check(addr, func));  
}
void Set_params_data(uint8_t addr, uint8_t func, uint8_t data){
    Serial.write(addr);
    Serial.write(func);
    Serial.write(data);
    Serial.write(Check(addr, func));  
}

void Set_speed(uint8_t addr, uint8_t speed){
    to_moteur1.write(addr);
    to_moteur1.write(set_speed);
    to_moteur1.write(speed);
    to_moteur1.write(Check(addr, set_speed));   
}

void Set_speed_position(uint8_t addr, uint8_t speed, uint8_t pos){
    to_moteur1.write(addr);
    to_moteur1.write(set_speed_position);
    to_moteur1.write(speed);
    to_moteur1.write(Check(addr, set_speed_position));      
}

uint8_t Percent_to_speed(float speed_percentage){
    if(speed_percentage > 0){return uint8_t(speed_percentage*1.27);}
    else{return int(-1*speed_percentage*1.27) + 128;}
}
*/
void setup() {
  // pinMode(motorPWM1, OUTPUT);
  // pinMode(motorPWM2, OUTPUT);
  pinMode(motorIN1, OUTPUT);
  pinMode(motorIN2, OUTPUT);
  // pinMode(motorIN3, OUTPUT);
  // pinMode(motorIN4, OUTPUT);

  // Configure the LEDC PWM channels
  ledcSetup(LEDC_CHANNEL_0, LEDC_BASE_FREQ, LEDC_TIMER_13_BIT);
  ledcSetup(LEDC_CHANNEL_1, LEDC_BASE_FREQ, LEDC_TIMER_13_BIT);

  // Attach the LEDC channels to the GPIO pins
  ledcAttachPin(STEP_PIN_1, LEDC_CHANNEL_0);
  ledcAttachPin(STEP_PIN_2, LEDC_CHANNEL_1);

  // Configure the direction and enable pins
  pinMode(DIR_PIN_1, OUTPUT);
  pinMode(ENABLE_PIN_1, OUTPUT);
  pinMode(DIR_PIN_2, OUTPUT);
  pinMode(ENABLE_PIN_2, OUTPUT);

  // Enable the stepper motors
  freeWheel(1);

  // Set the direction of the stepper motors
  setDir(1);

  SerialBT.begin("TOBOR");
  // to_moteur1.begin(115200,SERIAL_8N1,16,17); //Problèmes potentiels sur le format SERIAL_8N1 <= non lol
  Serial.begin(9600);

  gServo.setPeriodHertz(50); // standard 50 hz servo
	gServo.attach(14, 1000, 2000); // Attach the servo after it has been detatched

  dServo.setPeriodHertz(50); // standard 50 hz servo
	dServo.attach(27, 1000, 2000); // Attach the servo after it has been detatched

  hServo.setPeriodHertz(50); // standard 50 hz servo
	hServo.attach(26, 1000, 2000); // Attach the servo after it has been detatched

  hServo.write(400);
  gServo.write(0);
  dServo.write(60);

  // Set_params_no_data(ADDR_STP_D, stop);
  // Set_params_no_data(ADDR_STP_G, stop);

  Serial.println();
  Serial.println();
  Serial.println();
}

void loop() {
  if(SerialBT.available()){
    String receivedData = SerialBT.readStringUntil('\n');
    Serial.println(receivedData);
    int separatorIndex = receivedData.indexOf(' ');

    if (separatorIndex != -1) {
      id = receivedData.substring(0, separatorIndex);  // Extraire l'ID
      id.trim();  // Nettoyer les espaces autour de l'ID
      valueStr = receivedData.substring(separatorIndex + 1);  // Extraire la valeur
      valueStr.trim();  // Nettoyer les espaces autour de la valeur
      value = valueStr.toInt();  // Convertir en int
    }
    else {
      id = receivedData;
    }
    if (id.startsWith("debug")) {
      
    }
    if (id.startsWith("ar")) {
        arreterCanettes();
    }
    else if (id.startsWith("deploy")) {
        avancerCanettes();
        delay(value);
        arreterCanettes();
    }
    else if (id.startsWith("retract")) {
        reculerCanettes();
        delay(value);
        arreterCanettes();
    }
    else if (id.startsWith("fuck")) {
        avancerCanettes();
        delay(900);
        reculerCanettes();
        delay(900);
        arreterCanettes();
    }
    else if (id.startsWith("servog")) {      
        gServo.write(value);
    }
    else if (id.startsWith("servod")) {      
        dServo.write(value);
    }
    else if (id.startsWith("servoh")) {      
        hServo.write(value);
    }
    else if (id.startsWith("reset")) {      
        hServo.write(400);
        gServo.write(0);
        dServo.write(60);
    }
    else if (id.startsWith("pousser")) {      
        pousserCanettes();
    }
    else if (id.startsWith("vit")) {      
      vit = value;
    }
    else if (id.startsWith("vitessed")) {      
      
    }
    else if (id.startsWith("vitesseg")) {      
      
    }
    else if (id.startsWith("stopd")) {      
      
    }
    else if (id.startsWith("stopd")) {      
      
    }
    else if (id.startsWith("carre")) {    

      for(int i = 0; i<4; i++){

        avancer(value, vit);
        delay(800);

        gauche(90, vit);
        delay(800);
      }
    }
    else if (id.startsWith("a-r")) {      
        avancer(value, vit);
        delay(2000);

        reculer(value, vit);
        delay(2000);
    }
    else if (id.startsWith("gauchedroite")) {      
      gauche(90, vit);
      delay(2000);

      droite(180, vit);
      delay(2000);

      gauche(90, vit);
      delay(2000);
    }
    else if (id.startsWith("strat")) {      
      avancer(500, vit);
      delay(800);

      droite(90, vit);
      delay(800);

      avancer(450, vit);
      delay(800);

      droite(90, vit);
      delay(800);

      avancer(330, vit);
      delay(800);

      pousserCanettes();
      delay(800);

      reculer(500, vit);
      delay(800);

      gauche(165, vit);
      delay(800);

      // avancer(1300, vit);
      // delay(800);
    }
  }
  stop();
}

void setSpeed(int motor, int frequency) {
  if (motor == 1) {
    ledcWriteTone(LEDC_CHANNEL_0, frequency);
  } else if (motor == 2) {
    ledcWriteTone(LEDC_CHANNEL_1, frequency);
  }
}

void avancer(int distance_mm, int speed) {
  setDir(1);
  setSpeed(1, speed); // Set speed for motor 1 using frequency
  setSpeed(2, speed); // Set speed for motor 2 using frequency
  int steps = (distance_mm / (3.14 * dRoues)) * stepPerRev;
  delay(steps * 1000 / speed); // Adjust delay based on steps
  setSpeed(1, 0); // Stop motor 1
  setSpeed(2, 0); // Stop motor 2
}

void reculer(int distance_mm, int speed) {
  setDir(0);
  setSpeed(1, speed); // Set speed for motor 1 using frequency
  setSpeed(2, speed); // Set speed for motor 2 using frequency
  int steps = (distance_mm / (3.14 * dRoues)) * stepPerRev;
  delay(steps * 1000 / speed); // Adjust delay based on steps
  setSpeed(1, 0); // Stop motor 1
  setSpeed(2, 0); // Stop motor 2
}

void gauche(int angle, int speed) {
  digitalWrite(DIR_PIN_1, LOW);
  digitalWrite(DIR_PIN_2, LOW);
  setSpeed(1, speed); // Set speed for motor 1 using frequency
  setSpeed(2, speed); // Set speed for motor 2 using frequency
  int steps = (angle / 360.0) * (3.14 * ecartRoues) * stepPerRev / (3.14 * dRoues);
  delay(steps * 1000 / speed); // Adjust delay based on steps
  setSpeed(1, 0); // Stop motor 1
  setSpeed(2, 0); // Stop motor 2
}

void droite(int angle, int speed) {
  digitalWrite(DIR_PIN_1, HIGH);
  digitalWrite(DIR_PIN_2, HIGH);
  setSpeed(1, speed); // Set speed for motor 1 using frequency
  setSpeed(2, speed); // Set speed for motor 2 using frequency
  int steps = (angle / 360.0) * (3.14 * ecartRoues) * stepPerRev / (3.14 * dRoues);
  delay(steps * 1000 / speed); // Adjust delay based on steps
  setSpeed(1, 0); // Stop motor 1
  setSpeed(2, 0); // Stop motor 2
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
  hServo.write(400);
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

void stop() {
  setSpeed(1, 0); // Stop motor 1
  setSpeed(2, 0); // Stop motor 2
}

void freeWheel(char onOff){
  if(onOff){
    digitalWrite(ENABLE_PIN_1, HIGH); 
    digitalWrite(ENABLE_PIN_2, HIGH); 
  }
  else{
    digitalWrite(ENABLE_PIN_1, LOW); 
    digitalWrite(ENABLE_PIN_2, LOW);
  }
}

void setDir(char dir){
  if(dir){
    digitalWrite(DIR_PIN_1, LOW);
    digitalWrite(DIR_PIN_2, HIGH);
  }
  else{
    digitalWrite(DIR_PIN_1, HIGH);
    digitalWrite(DIR_PIN_2, LOW);
  }
}