#include "commande_serie.h"

void commande(){ // sert à entrer des commandes via le port serial 
    if(Serial.available()){
        String commande = Serial.readStringUntil('\n');
        String var = commande.substring(4);
        if(commande.substring(0,4)=="help"){
            Serial.println("TUTO utilisation bas niveau\n\n"
            "Mode teleguide a la manette bluetooth\n"
            "   bluetooth\n\n"
            "Mode execution d'une strategie\n"
            "   strat [0,...]\n"
            );
        }
        else if(commande.substring(0,3)=="rep"){
            Serial.println(var.substring(0,3).toInt());
        }
        else if(commande.substring(0,9)=="bluetooth"){
            Serial.println("Mode bluetooth active");
        }
        else if(commande.substring(0,5)=="strat"){
            Serial.println(commande.substring(5).substring(0,3).toInt());
        }
    }
  }