#include "strat.h"




void vstrat(void *pvParameters){
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