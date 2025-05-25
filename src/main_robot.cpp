#include "main_robot.h"

TaskHandle_t vstratHandle = NULL;
TaskHandle_t vterminal_bluetoothHandle = NULL;

int resolution = 8;
int bloque = 0;

bool modeBluetooth = false;//choix du mode bluetooth ou strategique
unsigned long startMillis;

bool initial_tbluetooth=true;

BluetoothSerial SerialBT;
OLEDInterface oled;
TaskParams Parameters = {0, 0, 0, 0};

GoToPosition serialGoto {X_POS_INIT,Y_POS_INIT,ANGLE_INIT,1000,1000,0};

void debug(void* param) {

  while (true) {
    if(digitalRead(SWITCH2)){
      FLAG_DEBUG = true;

      float x, y, angle;
      mot.GetPosition(x, y, angle);
      Serial.printf("%f;%f;%f\r\n", x, y, angle * RAD_TO_DEG);
    }
    else{
      FLAG_DEBUG = false;
    }
    vTaskDelay(pdMS_TO_TICKS(10));
  }
  vTaskDelete(NULL);
}

void setup() {
  esp_task_wdt_init(10,true);
  ComWithRasp comRasp;
  int *strat = new int(2);

  // Config des vitesses max et accélérations
  moteurGauche.setMaxSpeed(SPEEDMAX);
  moteurGauche.setAcceleration(ACCELMAX);
  moteurDroit.setCurrentPosition(0);

  moteurDroit.setMaxSpeed(SPEEDMAX);
  moteurDroit.setAcceleration(ACCELMAX);
  moteurGauche.setCurrentPosition(0);

  xPositionMutex = xSemaphoreCreateMutex();
  if (xPositionMutex == NULL) {
      // Serial.println("Erreur : mutex non créé");
  }

  Serial.begin(230400);
  startMillis = millis();

  SerialBT.begin("ESP32test");
  SerialBT.setTimeout(50);

  // xTaskCreate(readTofs,"readTofs", 5000, NULL, 1, NULL);
  xTaskCreate(vsetup_actionneurs,"vsetup_actionneurs", 1000, NULL, 1, NULL);
  xTaskCreatePinnedToCore(DoStrat, "Strat", 10000, strat, 1, &handleDoStrat, 1);

  mot.StartMotors();
  //comRasp.StartCom();
  xTaskCreate(debug,"debug", 8192, NULL, 1, NULL);
  
  if (!oled.begin()) {
    // Serial.println("Erreur init OLED");
    while (true);
  }
}

void loop() {
  //commande();
  TickType_t GoBase = 0;

  FLAG_TIRETTE = false;
  oled.afficherMenuPrincipal();
  vTaskDelay(pdMS_TO_TICKS(1000));

  Serial.println("Waiting for tirette In...");
  while(digitalRead(tirette)){
    vTaskDelay(pdMS_TO_TICKS(100));
  }

  oled.afficherScore(88);
  FLAG_TIRETTE = true;

  Serial.println("Waiting for tirette Out...");
  while(!digitalRead(tirette)){
    vTaskDelay(pdMS_TO_TICKS(100));
  }
  FLAG_TIRETTE = false;

  oled.afficherDebug();
  Serial.println("Tirette Out");
  GoBase = xTaskGetTickCount();

  Serial.println("On attend la fin du match");
  while(xTaskGetTickCount() - GoBase < TEMPS_MATCH_ROBOT){
    vTaskDelay(pdMS_TO_TICKS(100));
  }
  Serial.println("Fin du match");
  StopMatch();
  // Serial.println(frontClear_tof); // This will now work correctly
  
}