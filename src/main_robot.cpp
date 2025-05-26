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

GoToPosition serialGoto {0,0,0,1000,1000,0};

void debug(void* param) {

  while (true) {
    float x, y, angle;
    mot.GetPosition(x, y, angle);
    Serial.printf("%f;%f;%f\r\n", x, y, angle * RAD_TO_DEG);
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

void setup() {
  esp_task_wdt_init(10,true);
  ComWithRasp comRasp;

  // Config des vitesses max et accélérations
  moteurGauche.setMaxSpeed(SPEEDMAX);
  moteurGauche.setAcceleration(ACCELMAX);
  moteurDroit.setCurrentPosition(0);

  moteurDroit.setMaxSpeed(SPEEDMAX);
  moteurDroit.setAcceleration(ACCELMAX);
  moteurGauche.setCurrentPosition(0);

  xPositionMutex = xSemaphoreCreateMutex();
  if (xPositionMutex == NULL) {
      Serial.println("Erreur : mutex non créé");
  }


  Serial.begin(115200);
  startMillis = millis();

  SerialBT.begin("ESP32test");
  SerialBT.setTimeout(50);

  // xTaskCreate(readTofs,"readTofs", 5000, NULL, 1, NULL);

  xTaskCreate(vsetup_actionneurs,"vsetup_actionneurs", 1000, NULL, 1, NULL);
  xTaskCreate(debug,"debug", 1000, NULL, 1, NULL);

  mot.StartMotors();
  //comRasp.StartCom();

  if (!oled.begin()) {
    Serial.println("Erreur init OLED");
    while (true);
  }
  
  oled.displayPage(currentDisplayedPage);
  vTaskDelay(pdMS_TO_TICKS(3000));
  oled.afficherScore(88);
  vTaskDelay(pdMS_TO_TICKS(3000));
  oled.afficherDebug();
}

void loop() {
  commande();
  
  // Serial.println(frontClear_tof); // This will now work correctly
  vTaskDelay(pdMS_TO_TICKS(1000));
}

