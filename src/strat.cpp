#include "strat.h"

void retourBase(){
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

void vstrat0(){ // 1 Canette (bleue)
  
  // Position initiale
  float x = 0, y = 0;
  float angle = 0;

  // Va devant les canettes
  x = 2250; y = 400; angle = 180;
  serialGoto.Go(x, y, angle);

  // Se met en position pour pousser les canettes
  x = 2250; y = 250; angle = 180;
  serialGoto.Go(x, y, angle);

  // Action mécanique
  mot.WaitUntilDone();
  pousserCanettes();

  // Reculer de 500 mm vers le haut (Y+), même angle
  x = 2250; y = 400; angle = 180;
  serialGoto.Go(x, y, angle);

  // Se met en position pour se caler
  x = 3000; y = 400; angle = 90;
  serialGoto.AllerEtSet(x, y, angle, x-100, y, angle);

  // Reculer pour repartir
  x = 2700; y = 400; angle = 90;
  serialGoto.Go(x, y, angle);

}

void vstrat1(){ // 2 Canettes (bleue)

  // Position initiale
  float x = 0, y = 0;
  float angle = 0;

  vstrat0();

  // Position intermediaire
  x = 2450; y = 1200; angle = -90;
  serialGoto.Go(x, y, angle);

  // Va devant les canettes
  x = 1900; y = 1100; angle = 180;
  serialGoto.Go(x, y, angle);

  // Se met en position pour pousser les canettes
  x = 1900; y = 900; angle = 180;
  serialGoto.Go(x, y, angle);

    // On pousse les canettes
  x = 1775; y = 400; angle = 180;
  serialGoto.Go(x, y, angle);

  // On pousse les canettes
  x = 1775; y = 200; angle = 180;
  serialGoto.Go(x, y, angle);

  // Action mécanique
  mot.WaitUntilDone();
  pousserCanettes();

  // Reculer de 500 mm vers le haut (Y+), même angle
  x = 1775; y = 700; angle = 180;
  serialGoto.Go(x, y, angle);

}

void vstrat2(){ // 2 Cannettes + Banniere (blue)

  // Position initiale
  float x = 0, y = 0;
  float angle = 0;

  // Postion pour bannière
  x = 1775; y = 0; angle = 0;
  serialGoto.Go(x, y, angle);

  mot.WaitUntilDone();
  //Met en position la bannière
  DoBanniere();

  // Repart pour les canettes
  x = 1775; y = 300; angle = 0;
  serialGoto.Go(x, y, angle);

  // Fait la stratégie 1
  vstrat1();
}