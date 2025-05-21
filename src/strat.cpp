#include "strat.h"

void retourBase(){
  Serial.println("Retour à la base");
  // Position initiale
  float x = 0, y = 0;
  float angle = 0;

  // Va devant la base
  x = -750; y = 1100; angle = 0;
  serialGoto.Go(x, y, angle);

  vTaskDelay(1000);

  // Retour a la base
  x = -750; y = 1450; angle = 0;
  serialGoto.Go(x, y, angle);
}

void vstrat0(){ // 1 Canette (bleue)
  Serial.println("Stratégie 0 en cours d'exécution...");
  // Position initiale
  float x = 0, y = 0;
  float angle = 0;

  // Va devant les canettes
  x = -480; y = 300; angle = 180;
  serialGoto.Go(x, y, angle);

  // Se met en position pour pousser les canettes
  x = -480; y = 0; angle = 180;
  serialGoto.Go(x, y, angle);

  // Action mécanique
  mot.WaitUntilDone();
  pousserCanettes();

  // Reculer de 500 mm vers le haut (Y+), même angle
  x = -480; y = 300; angle = 180;
  serialGoto.Go(x, y, angle);

  // Retour a la base
  retourBase();

  if(vterminal_bluetoothHandle!=NULL){
    xTaskNotifyGive(vterminal_bluetoothHandle);
  } 
}

void vstrat1(){ // 2 Canettes (bleue)
  Serial.println("Stratégie 1 en cours d'exécution...");
  // Position initiale
  float x = 0, y = 0;
  float angle = 0;

  // Va devant les canettes
  x = -480; y = 300; angle = 180;
  serialGoto.Go(x, y, angle);

  // Se met en position pour pousser les canettes
  x = -480; y = -60; angle = 180;
  serialGoto.Go(x, y, angle);

  // Action mécanique
  mot.WaitUntilDone();
  pousserCanettes();

  // Reculer de 500 mm vers le haut (Y+), même angle
  x = -480; y = 300; angle = 180;
  serialGoto.Go(x, y, angle);

  // Position intermediaire
  x = -900; y = 950; angle = 90;
  serialGoto.Go(x, y, angle);

  // Va devant les canettes
  x = -200; y = 870; angle = 180;
  serialGoto.Go(x, y, angle);

  // Se met en position pour pousser les canettes
  x = -200; y = 700; angle = 180;
  serialGoto.Go(x, y, angle);

    // On pousse les canettes
  x = -100; y = 150; angle = 180;
  serialGoto.Go(x, y, angle);

  // On pousse les canettes
  x = -100; y = 0; angle = 180;
  serialGoto.Go(x, y, angle);

  Serial.println("Pousser les canettes");
  // Action mécanique
  mot.WaitUntilDone();
  pousserCanettes();

  // Reculer de 500 mm vers le haut (Y+), même angle
  x = -100; y = 500; angle = 180;
  serialGoto.Go(x, y, angle);

  // Retour a la base
  retourBase();
}

void vstrat2(){ // 2 Cannettes + Banniere (blue)
  Serial.println("Stratégie 2 en cours d'exécution...");
  // Position initiale
  float x = 0, y = 0;
  float angle = 0;

  // init banniere
  // startBanniere();
  // vTaskDelay(100);

  // Postion pour bannière
  x = 0; y = -212; angle = 0;
  serialGoto.Go(x, y, angle);

  mot.WaitUntilDone();
  //Met en position la bannière
  DoBanniere();

  // Repart pour les canettes
  x = 0; y = 100; angle = 0;
  serialGoto.Go(x, y, angle);

  // Fait la stratégie 1
  vstrat1();
}