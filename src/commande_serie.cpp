#include "commande_serie.h"

void commande(){ // sert à entrer des commandes via le port serial 
    if(Serial.available()){
        String commande = Serial.readStringUntil('\n');
        String var = commande.substring(4);
        if(commande.substring(0,4)=="help"){
            Serial.println("TUTO utilisation bas niveau\n\n"
            "Mode terminal bluetooth\n"
            "   tbluetooth\n\n"
            "Mode execution d'une strategie\n"
            "   strat [0,...]\n"
            "Envoie une pos au robot pour s'y rendre\n"
            "   goto [x] [y] [angle]\n"
            "Mode carre\n"
            "   carre\n"
            );
        }
        else if(commande.substring(0,3)=="rep"){
            Serial.println(var.substring(0,3).toInt());
        }
        else if(commande.substring(0,10)=="tbluetooth" || initial_tbluetooth){      
            initial_tbluetooth=false;      
            Serial.println("Mode terminal bluetooth active");

            xTaskCreate(vterminal_bluetooth,"vterminal_bluetooth", 4000, NULL, 1, &vterminal_bluetoothHandle);

        }
        else if(commande.substring(0,5)=="strat"){
            if(commande.substring(6,7)=="0"){
                Serial.println(commande.substring(5).substring(0,3).toInt());
                vstrat0();
            }
            if(commande.substring(6,7)=="1"){
                Serial.println(commande.substring(5).substring(0,3).toInt());
                vstrat1();
            }
            if(commande.substring(6,7)=="2"){
                Serial.println(commande.substring(5).substring(0,3).toInt());
                vstrat2(); 
            }
        }
        else if(commande.substring(0,4)=="goto"){
            serialGoto.Go((float)commande.substring(5,9).toInt(),(float)commande.substring(10,14).toInt(),(float)commande.substring(15,19).toInt());
        }
        else if(commande.substring(0,5)=="carre"){
            while(1){                
                serialGoto.Go(0,1000,90);
                serialGoto.Go(1000,1000,180);
                serialGoto.Go(1000,0,270);
                serialGoto.Go(0,0,0);
            }
        }
        else if(commande.substring(0,4)=="flag"){
            Serial.println("Flag");
            if(commande.substring(5,7)=="On"){
                //FLAG_STOP = 1;
            }
            else if(commande.substring(5,8)=="Off"){
                //FLAG_STOP = 0;
            }
        }
        else{
            xTaskCreate(vterminal_bluetooth,"vterminal_bluetooth", 4000, NULL, 1, &vterminal_bluetoothHandle);
        }
    }
}