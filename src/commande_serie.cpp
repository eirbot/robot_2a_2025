#include "commande_serie.h"

void commande(){ // sert à entrer des commandes via le port serial 
    if(Serial.available()){
        String commande = Serial.readStringUntil('\n');
        String var = commande.substring(4);
        if(commande.substring(0,4)=="help"){
            Serial.println("TUTO utilisation bas niveau\n\n"
            "Mode teleguide a la manette bluetooth\n"
            "   jbluetooth\n\n"
            "Mode terminal bluetooth\n"
            "   tbluetooth\n\n"
            "Mode execution d'une strategie\n"
            "   strat [0,...]\n"
            "Stoper le robot\n"
            "   stop"
            );
        }
        else if(commande.substring(0,3)=="rep"){
            Serial.println(var.substring(0,3).toInt());
        }
        else if(commande.substring(0,10)=="jbluetooth"){            
            Serial.println("Mode joystick bluetooth active");
            ledcSetup(STEPD, 1000,resolution);
            ledcAttachPin(STEPD, MOTOR_D);

            ledcSetup(STEPG, 1000,resolution);
            ledcAttachPin(STEPG, MOTOR_G);
            xTaskCreate(vcontrole_bluetooth,"vcontrole_bluetooth", 1000, NULL, 1, NULL);
        }
        else if(commande.substring(0,10)=="tbluetooth" || initial_tbluetooth){      
            initial_tbluetooth=false;      
            Serial.println("Mode terminal bluetooth active");

            xTaskCreate(vterminal_bluetooth,"vterminal_bluetooth", 4000, NULL, 1, &vterminal_bluetoothHandle);

        }
        else if(commande.substring(0,5)=="strat"){
            if(commande.substring(6,7)=="1"){
                Serial.println(commande.substring(5).substring(0,3).toInt());
                pinMode(STEPD,OUTPUT);
                pinMode(STEPG,OUTPUT);
                xTaskCreate(vstrat1,"vstrat1", 4096, NULL, 1, &vstratHandle ); 
            }
            else if(commande.substring(6,7)=="2"){
                Serial.println(commande.substring(5).substring(0,3).toInt());
                pinMode(STEPD,OUTPUT);
                pinMode(STEPG,OUTPUT);
                xTaskCreate(vstrat2,"vstrat2", 4096, NULL, 1, &vstratHandle ); 
            }
            
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
            if(vterminal_bluetoothHandle != NULL){
                vTaskDelete(vterminal_bluetoothHandle);
                vterminal_bluetoothHandle = NULL;
            }
            
            pinMode(STEPD,OUTPUT);
            pinMode(STEPG,OUTPUT);
            digitalWrite(STEPG,LOW);
            digitalWrite(STEPD,LOW);
        }
        else if(commande.substring(0,4)=="goto"){
            pinMode(STEPD,OUTPUT);
            pinMode(STEPG,OUTPUT);
            GoToPosition serialGoto {0,0,0,1000,1000,0};

            Serial.println((float)commande.substring(5,9).toInt());
            Serial.println((float)commande.substring(10,14).toInt());
            Serial.println((float)commande.substring(15,18).toInt());

            serialGoto.Go((float)commande.substring(5,9).toInt(),(float)commande.substring(10,14).toInt(),(float)commande.substring(15,18).toInt());
        }
        else if(commande.substring(0,4)=="test"){
            mot.StartMotors();
        }
        else if(commande.substring(0,4)=="tes1"){
            pinMode(STEPD,OUTPUT);
            pinMode(STEPG,OUTPUT);

            TaskParams Params = {commande.substring(5,9).toInt(), 0, 0, 10};

            mot.EnvoyerDonnees(&Params);
        }

    }
    else{
        xTaskCreate(vterminal_bluetooth,"vterminal_bluetooth", 4000, NULL, 1, &vterminal_bluetoothHandle);
    }
  }