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
            "Stoper le robot\n"
            "   stop"
            );
        }
        else if(commande.substring(0,3)=="rep"){
            Serial.println(var.substring(0,3).toInt());
        }
        else if(commande.substring(0,9)=="bluetooth"){            
            Serial.println("Mode bluetooth active");
            ledcSetup(STEPD, 1000,resolution);
            ledcAttachPin(STEPD,1);
            ledcSetup(STEPG, 1000,resolution);
            ledcAttachPin(STEPG,2);
            xTaskCreate(vcontrole_bluetooth,"vcontrole_bluetooth", 1000, NULL, 1, NULL);
        }
        else if(commande.substring(0,5)=="strat"){
            Serial.println(commande.substring(5).substring(0,3).toInt());
            pinMode(STEPD,OUTPUT);
            pinMode(STEPG,OUTPUT);
            xTaskCreate(vstrat,"vstrat", 4096, NULL, 1, &vstratHandle ); 
        }
        else if(commande.substring(0,5)=="stop"){
            if(vavancerHandle != NULL){
                vTaskDelete(vavancerHandle);
                vavancerHandle = NULL;
            }
            if(vtournerHandle != NULL){
                vTaskDelete(vtournerHandle);
                vtournerHandle = NULL;
            }
            if(vstratHandle != NULL){
                vTaskDelete(vstratHandle);   
                vstratHandle = NULL;
            }
            pinMode(STEPD,OUTPUT);
            pinMode(STEPG,OUTPUT);
            digitalWrite(STEPG,LOW);
            digitalWrite(STEPD,LOW);
        }
        else if(commande.substring(0,5)=="goto"){
            GoToPosition serialGoto {0,0,0,1000,1000,0};
            serialGoto.Go();
        }
    }
  }