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

Page* CurrentDisplayPage = &Menu_principal;

int FLAG_NEXT = false;
int FLAG_SELECT = false;
void testing(int id){
    switch (id)
    {
    case 0:
        /* code */
        break;
    case 1:
        /* code */
        break;
    case 2:
        /* code */
        break;
    case 3:
        /* code */
        break;
    case 4:
        /* code */
        break;
    case 5:
        /* code */
        break;
    case 10:
        /* code */
        break;
    case 11:
        /* code */
        break;
    case 12:
        /* code */
        break;
    case 13:
        /* code */
        break;
    default:
        break;
    }
}
void updateValue(int id){
    switch (id)
    {
    case 0:
        /* code */
        break;
    case 1:
        /* code */
        break;
    case 2:
        /* code */
        break;

    case 10:
        /* code */
        break;
    case 11:
        /* code */
        break;
    case 12:
        /* code */
        break;
    case 13:
        /* code */
        break;
    case 15:
        /* code */
        break;
    case 20:
        if(jaune){
          CurrentDisplayPage[CurrentDisplayPage->_currentElement]._name = "JAUNE";
        }
        else{
          CurrentDisplayPage[CurrentDisplayPage->_currentElement]._name = "BLEU";
        }
        break;
    default:
        break;
    }
}

void ButtonInteraction(){
  if(FLAG_NEXT){
    CurrentDisplayPage->NextElement();
    FLAG_NEXT = false;
  }
  else if(FLAG_SELECT){
    CurrentDisplayPage->doElement(CurrentDisplayPage);
    FLAG_SELECT = false;
  }
}


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
  oled.displayPage(CurrentDisplayPage);
}

void loop() {

  
}
