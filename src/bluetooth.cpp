#include "bluetooth.h"

void moteur_droit(int vitesse,int sens){
    ledcWriteTone(1, vitesse);
    ledcWrite(1, 127);
    //analogWrite(STEPD, vitesse);
    digitalWrite(DIRD, sens);
    //digitalWrite(IN2, (1-sens));
}

void moteur_gauche(int vitesse,int sens){
    ledcWriteTone(2, vitesse);
    ledcWrite(2, 127);
    //analogWrite(STEPG, vitesse);
    digitalWrite(DIRG, sens);
    //digitalWrite(IN4, (1-sens));
}

void vcontrole_bluetooth(void *pvParameters){

    int vitesse_droite=0;
    int vitesse_gauche=0;
    
    int8_t vitesse_droite_recep=0;
    int8_t vitesse_gauche_recep=0;

    for( ;; )
    {
      while(SerialBT.available()){
        
        vitesse_gauche_recep = SerialBT.read();
        vitesse_droite_recep = SerialBT.read();
  
        vitesse_droite = (int)vitesse_droite_recep*50;
        vitesse_gauche = (int)vitesse_gauche_recep*50;

        Serial.println(vitesse_droite);
        
  
        if(vitesse_droite>0){
          moteur_droit(vitesse_droite,1);
        }
        else if(vitesse_droite<0){
          moteur_droit(-vitesse_droite,0);
        }
        else{
          moteur_droit(0,0);
        }
        if(vitesse_gauche>0){
          moteur_gauche(vitesse_gauche,1); 
        }
        else if(vitesse_gauche<0){
          moteur_gauche(-vitesse_gauche,0);
        }
        else{
          moteur_droit(0,0);
        }     
      }
    }
}
