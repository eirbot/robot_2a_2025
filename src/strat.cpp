#include "strat.h"

void retourBase(){
  Serial.println("Retour à la base...");
  // Position initiale
  float x = 0, y = 0;
  float angle = 0;

  // Point intermédiaire si nécessaire
  mot.GetPosition(x, y, angle);
  if(y < 1000 || y > 0){
    x = 2000; y = 1000; angle = 0;
    inverseur(&x,&angle);
    serialGoto.Go(x, y, angle);
  }

  // Va devant la base
  x = 2500; y = 1325; angle = 0;
  inverseur(&x,&angle);
  serialGoto.Go(x, y, angle);

  mot.WaitUntilDone();
  vTaskDelay(pdMS_TO_TICKS(1000)); // Attente pour stabilisation


  // se placer pour aller chercher canettes de fin
  x = 2600; y = 1325; angle = 90;
  inverseur(&x,&angle);
  serialGoto.Go(x, y, angle);

  // aller chercher canettes de fin
  x = 2796; y = 1325; angle = 90;
  inverseur(&x,&angle);
  serialGoto.Go(x, y, angle);

  // Attraper les canettes pour le transport
  mot.WaitUntilDone();
  resetActionneurs();

  // s'éloigner du bord pour retournement
  x = 2600; y = 1325; angle = 90;
  inverseur(&x,&angle);
  serialGoto.Go(x, y, angle);

  // Retour a la base
  x = 2700; y = 1600; angle = 0;
  inverseur(&x,&angle);
  serialGoto.Go(x, y, angle);

  mot.WaitUntilDone();
  Serial.println("Retour à la base terminé.");
}

void StopMatch(){
  mot.Stop();        // Vide la queue et arrête les moteurs
  if (handleDoStrat != NULL) {
    vTaskDelete(handleDoStrat);
    handleDoStrat = NULL;
  }
  retourBase();             // Optionnel : revenir à la base
}

void inverseur(float* x, float* angle){
  if(jaune){
    *x= 3000.0- *x;
    *angle = -*angle;
  }
}

void DoStrat(void* param) {
  int strat = *((int*)param);

  while(1){
    Serial.println("Attente de la tirette...");
    while(!FLAG_TIRETTE) {
      vTaskDelay(pdMS_TO_TICKS(100));
    }

    Serial.println("Choix Equipe");
    while(FLAG_TIRETTE) {
      if(digitalRead(SWITCH1)){
        jaune = true;
        Serial.println("Jaune");
      }
      else{
        jaune = false;
        Serial.println("Bleu");
      }
      if(digitalRead(SWITCH3)){
        FLAG_TOF = true;
        Serial.println("TOF active");
      }
      else{
        FLAG_TOF = true;
        Serial.println("TOF active");
        // FLAG_TOF = false;
        // Serial.println("TOF desactivé");
      }
      vTaskDelay(pdMS_TO_TICKS(100));
    }

    if(jaune){
      X_POS_INIT= 1225;
      Y_POS_INIT= 220;
      ANGLE_INIT= 0;
    }
    else{
      X_POS_INIT= 1775;
      Y_POS_INIT= 220;
      ANGLE_INIT= 0;
    }
    mot.SetPosition(X_POS_INIT,Y_POS_INIT,ANGLE_INIT);
    serialGoto.x_initial = X_POS_INIT;
    serialGoto.y_initial = Y_POS_INIT;
    serialGoto.cangle_initial = ANGLE_INIT;

    Serial.printf("Strategie %d en cours...\n", strat);
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
      case 3:
        vstrat3();
        break;
      case 4:
        vstrat4();
        break;
      case 5:
        vstrat5();
        break;
      default:
        Serial.println("Strategie inconnue");
        retourBase();
        break;
    }
    Serial.println("Strategie terminee.");
  }
  vTaskDelete(NULL);
}

void vstrat0(){ // 1 Canette (bleue)
  
  // Position initiale
  float x = 0, y = 0;
  float angle = 0;

  // Va devant les canettes
  x = 2220; y = 400; angle = 180;
  inverseur(&x,&angle);
  serialGoto.Go(x, y, angle);

  // Se met en position pour pousser les canettes
  x = 2220; y = 300; angle = 180;
  inverseur(&x,&angle);
  serialGoto.Go(x, y, angle);

  // Action mécanique
  mot.WaitUntilDone();
  pousserCanettes();

  // Reculer de 500 mm vers le haut (Y+), même angle
  x = 2220; y = 400; angle = 90;
  inverseur(&x,&angle);
  serialGoto.Go(x, y, angle);

  // // Se met en position pour se caler
  // x = 2800; y = 400; angle = 90;
  // inverseur(&x,&angle);
  // serialGoto.Go(x, y, angle);

  // // Se caler
  // mot.WaitUntilDone();

  // x = 3000; y = 400; angle = 90;
  // inverseur(&x,&angle);
  // serialGoto.AllerEtSet(x, y, angle, (jaune) ? 150 : 2850, y, angle);

  // mot.WaitUntilDone();

  // // Reculer pour repartir
  // x = 2600; y = 400; angle = 90;
  // inverseur(&x,&angle);
  // serialGoto.Go(x, y, angle);

}

void vstrat1(){ // 2 Canettes (bleue)

  // Position initiale
  float x = 0, y = 0;
  float angle = 0;

  vstrat0();

  // Position intermediaire
  x = 2450; y = 1300; angle = -90;
  inverseur(&x,&angle);
  serialGoto.Go(x, y, angle);

  // Va devant les canettes
  x = 1900; y = 1300; angle = 180;
  inverseur(&x,&angle);
  serialGoto.Go(x, y, angle);

  // Attraper les canettes pour le transport
  mot.WaitUntilDone();
  resetActionneurs();

  // Se met en position pour pousser les canettes
  x = 1900; y = 1100; angle = 180;
  inverseur(&x,&angle);
  serialGoto.Go(x, y, angle);

    // On pousse les canettes
  x = 1775; y = 350; angle = 180;
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
  x = 1775; y = 155; angle = 0;
  inverseur(&x,&angle);
  serialGoto.Go(x, y, angle);

  mot.WaitUntilDone();
  //Met en position la bannière
  DoBanniere();

  // Repart pour les canettes
  x = 1775; y = 550; angle = 0;
  inverseur(&x,&angle);
  serialGoto.Go(x, y, angle);

  // Position initiale pour la strat 1
  x = 2220; y = 550; angle = 180;
  inverseur(&x,&angle);
  serialGoto.Go(x, y, angle);

  // Fait la stratégie 1
  vstrat1();
}

void vstrat3(){ // Banniere

  // Position initiale
  float x = 0, y = 0;
  float angle = 0;

  // Postion pour bannière
  x = 1775; y = 155; angle = 0;
  inverseur(&x,&angle);
  serialGoto.Go(x, y, angle);

  mot.WaitUntilDone();
  //Met en position la bannière
  DoBanniere();
}

void vstrat4(){ // strat utilisation de la capacité à tirer les canettes

  // Position initiale
  float x = 0, y = 0;
  float angle = 0;

  // Postion pour bannière
  x = 1775; y = 155; angle = 0;
  inverseur(&x,&angle);
  serialGoto.Go(x, y, angle);

  mot.WaitUntilDone();
  //Met en position la bannière
  DoBanniere();

   // s'écarter de la bannière
  x = 1775; y = 200; angle = 0;
  inverseur(&x,&angle);
  serialGoto.Go(x, y, angle);

  // canettes centre
  x = 1900; y = 825; angle = 0;
  inverseur(&x,&angle);
  serialGoto.Go(x, y, angle);

  // Attraper les canettes pour le transport
  mot.WaitUntilDone();
  resetActionneurs();

  // bien plaquer les canettes
  x = 1900; y = 900; angle = 91;
  inverseur(&x,&angle);
  serialGoto.Go(x, y, angle);

  mot.WaitUntilDone();
  baisserBras();

  //finir demi tour
  x = 1900; y = 880; angle = 180;
  inverseur(&x,&angle);
  serialGoto.Go(x, y, angle);

  mot.WaitUntilDone();
  monterBras();

  // retour base pour placer l'etage
  x = 1750; y = 400; angle = 180;
  inverseur(&x,&angle);
  serialGoto.Go(x, y, angle);

  // Action mécanique
  mot.WaitUntilDone();
  pousserCanettes();

  // s'éloigner de l'etage
  x = 1750; y = 800; angle = 180;
  inverseur(&x,&angle);
  serialGoto.Go(x, y, angle);

  // placement pour entamer la marche vers les canettes du depart
  x = 2230; y = 800; angle = 180;
  inverseur(&x,&angle);
  serialGoto.Go(x, y, angle);

  // Attraper les canettes pour le transport
  mot.WaitUntilDone();
  resetActionneurs();

  // entamer la marche vers les canettes du depart
  x = 2220; y = 350; angle = 180;
  inverseur(&x,&angle);
  serialGoto.Go(x, y, angle);

  // // avancer les canettes
  // x = 2220; y = 400; angle = 180;
  // inverseur(&x,&angle);
  // serialGoto.Go(x, y, angle);

  // Action mécanique
  mot.WaitUntilDone();
  pousserCanettes();

  // se decoler des canettes
  x = 2230; y = 500; angle = 180;
  inverseur(&x,&angle);
  serialGoto.Go(x, y, angle);

  // se placer pour aller chercher canettes suivante
  x = 2600; y = 400; angle = 90;
  inverseur(&x,&angle);
  serialGoto.Go(x, y, angle);

  // aller chercher canettes suivante
  x = 2796; y = 400; angle = 90;
  inverseur(&x,&angle);
  serialGoto.Go(x, y, angle);

  // Attraper les canettes pour le transport
  mot.WaitUntilDone();
  resetActionneurs();

  // // bien plaquer les canettes
  // x = 2843; y = 400; angle = 90;
  // inverseur(&x,&angle);
  // serialGoto.Go(x, y, angle);

  // s'éloigner du bord pour retournement
  x = 2600; y = 400; angle = 90;
  inverseur(&x,&angle);
  serialGoto.Go(x, y, angle);

  // retour zone de pose
  x = 1776; y = 552; angle = 180;
  inverseur(&x,&angle);
  serialGoto.Go(x, y, angle);

  // Action mécanique
  mot.WaitUntilDone();
  pousserCanettes();
}

void vstrat5(){ // Aller Retour

  // Position initiale
  float x = 0, y = 0;
  float angle = 0;

  // avant
  x = 1775; y = 1720; angle = 0;
  inverseur(&x,&angle);
  serialGoto.Go(x, y, angle);

  // arriere
  x = 1775; y = 220; angle = 0;
  inverseur(&x,&angle);
  serialGoto.Go(x, y, angle);
}