#include "strat.h"


TaskHandle_t vavancerHandle = NULL;
TaskHandle_t vtournerHandle = NULL;

void vstrat1(void *pvParameters){
  int distance = 100;
  int angle=90;

  avancer(500,0,10);
  tourner(90,1,0); 
  avancer(400,0,10);
  tourner(90,0,0);
  avancer(650,0,10);
  tourner(90,0,0);
  avancer(1000,0,10);
  tourner(90,0,0);
  avancer(600,0,10);
  tourner(90,0,0);
  avancer(600,0,10);
  tourner(90,0,0);
  avancer(500,1,10);
  vTaskDelete(NULL);
}

void vstrat2(void *pvParameters){
  int vit = 20;
  avancer(500, 0, vit);

  tourner(90, 0, vit);

  avancer(450, 0, vit);

  tourner(90, 0, vit);

  avancer(330, 0, vit);

  pousserCanettes();

  avancer(500, 1, vit);

  tourner(165, 1, vit);

  avancer(1300, 0, vit);
  vTaskDelete(NULL);
}