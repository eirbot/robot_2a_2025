#include "strat.h"

void retourBase(){
  Serial.println("Retour à la base...");
  // Position initiale
  float x = 0, y = 0;
  float angle = 0;

  // Va devant la base
  x = 2700; y = 1200; angle = 0;
  serialGoto.Go(x, y, angle);

  vTaskDelay(1000);

  // Retour a la base
  x = 2700; y = 1600; angle = 0;
  serialGoto.Go(x, y, angle);
}

void inverseur(float* x, float* angle){
  if(jaune){
    *x= 3000.0- *x;
    *angle = -*angle;
  }
}

void DoStrat(void* param) {
  int strat = *((int*)param);
  Serial.printf("Stratégie %d en cours...\n", strat);

  if(digitalRead(SWITCH1)){
    jaune = true;
  }
  else{
    jaune = false;
  }

  if(jaune){
    float X_POS_INIT= 1225;
    float Y_POS_INIT= 200;
    float ANGLE_INIT= 0;
  }
  else{
    float X_POS_INIT= 1775;
    float Y_POS_INIT= 200;
    float ANGLE_INIT= 0;
  }
  mot.SetPosition(X_POS_INIT,Y_POS_INIT,ANGLE_INIT);

  switch (strat) {
    case 0:
      vstrat0();
      break;
    case 1:
      vstrat1();
      break;
    case 2:
      vstrat2();
      break;
    default:
      Serial.println("Stratégie inconnue");
      retourBase();
      break;
  }

  Serial.println("Stratégie terminée.");
  vTaskDelete(NULL);
}

void vstrat0(){ // 1 Canette (bleue)
  
  // Position initiale
  float x = 0, y = 0;
  float angle = 0;

  // Va devant les canettes
  x = 2250; y = 400; angle = 180;
  inverseur(&x,&angle);
  serialGoto.Go(x, y, angle);

  // Se met en position pour pousser les canettes
  x = 2250; y = 250; angle = 180;
  inverseur(&x,&angle);
  serialGoto.Go(x, y, angle);

  // Action mécanique
  mot.WaitUntilDone();
  pousserCanettes();

  // Reculer de 500 mm vers le haut (Y+), même angle
  x = 2250; y = 400; angle = 180;
  inverseur(&x,&angle);
  serialGoto.Go(x, y, angle);

  // Se met en position pour se caler
  x = 3000; y = 400; angle = 90;
  inverseur(&x,&angle);
  serialGoto.AllerEtSet(x, y, angle, x-100, y, angle);

  // Reculer pour repartir
  x = 2700; y = 400; angle = 90;
  inverseur(&x,&angle);
  serialGoto.Go(x, y, angle);

}

void vstrat1(){ // 2 Canettes (bleue)

  // Position initiale
  float x = 0, y = 0;
  float angle = 0;

  vstrat0();

  // Position intermediaire
  x = 2450; y = 1200; angle = -90;
  inverseur(&x,&angle);
  serialGoto.Go(x, y, angle);

  // Va devant les canettes
  x = 1900; y = 1100; angle = 180;
  inverseur(&x,&angle);
  serialGoto.Go(x, y, angle);

  // Se met en position pour pousser les canettes
  x = 1900; y = 900; angle = 180;
  inverseur(&x,&angle);
  serialGoto.Go(x, y, angle);

    // On pousse les canettes
  x = 1775; y = 400; angle = 180;
  inverseur(&x,&angle);
  serialGoto.Go(x, y, angle);

  // On pousse les canettes
  x = 1775; y = 200; angle = 180;
  inverseur(&x,&angle);
  serialGoto.Go(x, y, angle);

  // Action mécanique
  mot.WaitUntilDone();
  pousserCanettes();

  // Reculer de 500 mm vers le haut (Y+), même angle
  x = 1775; y = 700; angle = 180;
  inverseur(&x,&angle);
  serialGoto.Go(x, y, angle);

}

void vstrat2(){ // 2 Cannettes + Banniere (blue)

  // Position initiale
  float x = 0, y = 0;
  float angle = 0;

  // Postion pour bannière
  x = 1775; y = 0; angle = 0;
  inverseur(&x,&angle);
  serialGoto.Go(x, y, angle);

  mot.WaitUntilDone();
  //Met en position la bannière
  DoBanniere();

  // Repart pour les canettes
  x = 1775; y = 300; angle = 0;
  inverseur(&x,&angle);
  serialGoto.Go(x, y, angle);

  // Fait la stratégie 1
  vstrat1();
}