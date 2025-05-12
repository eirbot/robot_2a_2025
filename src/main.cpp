#include "main.h"

TaskHandle_t vstratHandle = NULL;
TaskHandle_t vterminal_bluetoothHandle = NULL;

int resolution = 8;
int bloque = 0;

bool modeBluetooth = false;//choix du mode bluetooth ou strategique
unsigned long startMillis;

bool initial_tbluetooth=true;

BluetoothSerial SerialBT;
TaskParams Parameters = {0, 0, 0, 0};

GoToPosition serialGoto {0,0,0,1000,1000,0};

void setup() {
  esp_task_wdt_init(10,true);

  pinMode(DIRD,OUTPUT);
  pinMode(DIRG,OUTPUT);
  pinMode(STEPD,OUTPUT);
  pinMode(STEPG,OUTPUT);

  // Config des vitesses max et accélérations
  moteurGauche.setMaxSpeed(SPEEDMAX);
  moteurGauche.setAcceleration(ACCELMAX);

  moteurDroit.setMaxSpeed(SPEEDMAX);
  moteurDroit.setAcceleration(ACCELMAX);

  Serial.begin(115200);
  startMillis = millis();

  SerialBT.begin("ESP32test");
  SerialBT.setTimeout(50);

  // xTaskCreate(readTofs,"readTofs", 5000, NULL, 1, NULL);

  xTaskCreate(vsetup_actionneurs,"vsetup_actionneurs", 1000, NULL, 1, NULL);

  mot.StartMotors();
  
}

void loop() {
  commande();
  // Serial.println(frontClear_tof); // This will now work correctly
  vTaskDelay(pdMS_TO_TICKS(1000));
}