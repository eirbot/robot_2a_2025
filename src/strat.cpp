#include "strat.h"




void vstrat(void *pvParameters){
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
    vTaskDelete(NULL);
  }