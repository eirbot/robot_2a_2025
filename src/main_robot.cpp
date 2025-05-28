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


Page Test_total("Test Total", 't', 0);
Page Test_Pince("Test Pince", 't', 1);
Page Test_Servo("Test Servo", 't', 2);
Page Test_Pousseur("Test Pousseur", 't', 3);
std::vector<Page> elementsOfTest = {Test_total, Test_Pince, Test_Servo, Test_Pousseur};
Page Test_actionneurs("Test Actionneurs", elementsOfTest, 's', 0);

Page test_baniere("Test Baniere", 't', 4);
Page Test_vox("Test Vox", 't', 5);
std::vector<Page> elementsOfTest2 = {Test_actionneurs, test_baniere, Test_vox};
Page Test("Test", elementsOfTest2, 's', 0);

Page PositionX("Position X", 'c', 0);
Page PositionY("Position Y", 'c', 1);
Page PositionAngle("Position Angle", 'c', 2);

Page Position("Position", {PositionX, PositionY, PositionAngle}, 's', 0);

Page Strat_1("Stratégie 1", 't', 0);
Page Strat_2("Stratégie 2", 't', 1);
Page Strat_3("Stratégie 3", 't', 2);
Page Strat_4("Stratégie 4", 't', 3);
std::vector<Page> elementsOfStrat = {Strat_1, Strat_2, Strat_3, Strat_4};
Page Strat("Stratégies", elementsOfStrat, 's', 0);

Page Tof_1("TOF_1", 'c', 0);
Page Tof_2("TOF_2", 'c', 1);
Page Tof_3("TOF_3", 'c', 2);
Page Tof_4("TOF_4", 'c', 3);
Page Tof_5("TOF_5", 'c', 4);
Page Tof_6("TOF_6", 'c', 5);
std::vector<Page> elementsOfTof = {Tof_1, Tof_2, Tof_3, Tof_4, Tof_5,Tof_6};
Page Tof("TOF", elementsOfTof, 's', 0);

std::vector<Page> elementsOfPage = {Test, Position, Strat, Tof};
Page Menu_principal("Menu Principal", elementsOfPage, 's', 0);

Page CurrentDisplayPage = Menu_principal;



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
  int *strat = new int(3);

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

  xTaskCreate(readTofs,"readTofs", 5000, NULL, 1, NULL);
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
  oled.displayPage(CurrentDisplayPage);
  vTaskDelay(pdMS_TO_TICKS(10));

  
}
