#include "strat.h"




void vstrat1(void *pvParameters){
  int distance = 100;
  int angle=90;

  avancer(500,0,10);
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
  tourner(90,1,0); 
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
  avancer(400,0,10);
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
  tourner(90,0,0);
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
  avancer(650,0,10);
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
  tourner(90,0,0);
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
  avancer(1000,0,10);
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
  tourner(90,0,0);
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
  avancer(600,0,10);
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
  tourner(90,0,0);
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
  avancer(600,0,10);
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
  tourner(90,0,0);
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
  avancer(500,1,10);
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

  if(vterminal_bluetoothHandle!=NULL){
    xTaskNotifyGive(vterminal_bluetoothHandle);
  } 

  vTaskDelete(vstratHandle);
}

void vstrat2(void *pvParameters){
  int vit = 20;
  avancer(500, 0, vit);
  ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
  tourner(90, 0, vit);
  ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
  avancer(450, 0, vit);
  ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
  tourner(90, 0, vit);
  ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
  avancer(330, 0, vit);
  ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
  pousserCanettes();
  avancer(500, 1, vit);
  ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
  tourner(165, 1, vit);
  ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
  avancer(1300, 0, vit);
  ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

  if(vterminal_bluetoothHandle!=NULL){
    xTaskNotifyGive(vterminal_bluetoothHandle);
  } 

  vTaskDelete(vstratHandle);
}