#include "terminal_bluetooth.h"

void vterminal_bluetooth(void *pvParameters){
  int vit = 100;
  String id;
  String valueStr;
  int value;

  for(;;){
    
    while(SerialBT.available()){
      String receivedData = SerialBT.readStringUntil('\n');
      Serial.println(receivedData);
      int separatorIndex = receivedData.indexOf(' ');
  
      if (separatorIndex != -1) {
        id = receivedData.substring(0, separatorIndex);  // Extraire l'ID
        id.trim();  // Nettoyer les espaces autour de l'ID
        valueStr = receivedData.substring(separatorIndex + 1);  // Extraire la valeur
        valueStr.trim();  // Nettoyer les espaces autour de la valeur
        value = valueStr.toInt();  // Convertir en int
      }
      else {
        id = receivedData;
      }
      if (id.startsWith("help")) {
        SerialBT.println("ar");
        SerialBT.println("deploy *value* : value between 0 and 900");
        SerialBT.println("retract *value* : value between 0 and 900");
        SerialBT.println("fuck");
        SerialBT.println("servog *value* : value between 0 and 180, resets at 0");
        SerialBT.println("servod *value* : value between 0 and 180, resets at 60");
        SerialBT.println("servoh *value* : value between 0 and 180, resets at 160");
        SerialBT.println("reset");
        SerialBT.println("pousser");
        SerialBT.println("vit *value*");
        SerialBT.println("strat *value*: 1 pitit tour autour des planches, 2 strat de base à 18 points");
      }
      if (id.startsWith("ar")) {
          arreterCanettes();
      }
      else if (id.startsWith("deploy")) {
          avancerCanettes();
          delay(value);
          arreterCanettes();
      }
      else if (id.startsWith("retract")) {
          reculerCanettes();
          delay(value);
          arreterCanettes();
      }
      else if (id.startsWith("fuck")) {
          avancerCanettes();
          delay(900);
          reculerCanettes();
          delay(900);
          arreterCanettes();
      }
      else if (id.startsWith("servog")) {      
          gServof(value);
      }
      else if (id.startsWith("servod")) {      
          dServof(value);
      }
      else if (id.startsWith("servoh")) {      
          hServof(value);
      }
      else if (id.startsWith("servob")) {      
        bServof(value);
      }
      else if (id.startsWith("reset")) {      
          resetActionneurs();
      }
      else if (id.startsWith("pousser")) {      
          pousserCanettes();
      }
      else if (id.startsWith("vit")) {      
        vit = value;
      }
      else if (id.startsWith("vitessed")) {      
        
      }
      else if (id.startsWith("vitesseg")) {      
        
      }
      else if (id.startsWith("stopd")) {      
        
      }
      else if (id.startsWith("stopd")) {      
        
      }
      else if (id.startsWith("carre")) {    
  
        for(int i = 0; i<4; i++){
  
          // avancer(value, vit);
          // delay(800);
  
          // gauche(90, vit);
          // delay(800);
        }
      }
      else if (id.startsWith("a-r")) {      
          // avancer(value, vit);
          // delay(2000);
  
          // reculer(value, vit);
          // delay(2000);
      }
      else if (id.startsWith("gauchedroite")) {      
        // gauche(90, vit);
        // delay(2000);
  
        // droite(180, vit);
        // delay(2000);
  
        // gauche(90, vit);
        // delay(2000);
      }
      else if (id.startsWith("strat")) {    
        if (value == 0) {
          vstrat0();
        }        
        else if (value == 1) {
          vstrat1();
        }
        else {
          vstrat2();
        }  
      }
      else if (id.startsWith("quit")) {      
        vTaskDelete(vterminal_bluetoothHandle);
      }
      else if (id.startsWith("stop")) {      
        vTaskDelete(vterminal_bluetoothHandle);
      }
      else if(receivedData.substring(0,4)=="goto"){
        SerialBT.println("start goto");
        serialGoto.Go((float)receivedData.substring(5,9).toInt(),(float)receivedData.substring(10,14).toInt(),(float)receivedData.substring(15,19).toInt());
        // pinMode(STEPD,OUTPUT);
        // pinMode(STEPG,OUTPUT);
        // GoToPosition serialGoto {0,0,0,1000,1000,0};

        // SerialBT.println((float)receivedData.substring(5,9).toInt());
        // SerialBT.println((float)receivedData.substring(10,14).toInt());
        // SerialBT.println((float)receivedData.substring(15,18).toInt());

        // serialGoto.Go((float)receivedData.substring(5,9).toInt(),(float)receivedData.substring(10,14).toInt(),(float)receivedData.substring(15,18).toInt());
      }
    }
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}