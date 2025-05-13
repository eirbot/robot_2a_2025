#include "strat.h"

void vstrat0(){ // 1 Canette (bleue)
  Serial.println("Stratégie 0 en cours d'exécution...");
  // Position initiale
  float x = 0, y = 0;
  float angle = 0;

  // Va devant les canettes
  x = -480;
  y = 300;
  angle = 180;

  serialGoto.Go(x, y, angle, 0);

  // Se met en position pour pousser les canettes
  x = -480;
  y = 0;
  angle = 180;

  serialGoto.Go(x, y, angle, 0);

  // Action mécanique
  serialGoto.waitPos();
  pousserCanettes();

  // Reculer de 500 mm vers le haut (Y+), même angle
  x = -480;
  y = 300;
  angle = 180;

  serialGoto.Go(x, y, angle, 1);

  // Retour a la base
  x = -950;
  y = 1450;
  angle = 0;
  serialGoto.Go(x, y, angle, 0);


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
  x = -480;
  y = 300;
  angle = 180;

  serialGoto.Go(x, y, angle, 0);

  // Se met en position pour pousser les canettes
  x = -480;
  y = 0;
  angle = 180;

  serialGoto.Go(x, y, angle, 0);

  // Action mécanique
  serialGoto.waitPos();
  pousserCanettes();

  // Reculer de 500 mm vers le haut (Y+), même angle
  x = -480;
  y = 300;
  angle = 180;

  serialGoto.Go(x, y, angle, 1);

  // Position intermediaire
  x = -950;
  y = 1000;
  angle = 90;

  serialGoto.Go(x, y, angle, 1);

  // Se met en position pour pousser les canettes
  x = -100;
  y = 800;
  angle = 180;

  serialGoto.Go(x, y, angle, 0);

  // On pousse les canettes
  x = 0;
  y = 0;
  angle = 180;
  serialGoto.Go(x, y, angle, 0);

  // Action mécanique
  serialGoto.waitPos();
  pousserCanettes();

  // Reculer de 500 mm vers le haut (Y+), même angle
  x = 0;
  y = 500;
  angle = 0;

  serialGoto.Go(x, y, angle, 1);

  // Retour a la base
  x = -950;
  y = 1450;
  angle = 0;

  serialGoto.Go(x, y, angle, 0);
}

void vstrat2(){ // 2 Cannettes + Banniere (blue)

}