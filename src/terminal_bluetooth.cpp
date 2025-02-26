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
      if (id.startsWith("debug")) {
        
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
          hServof(160);
          gServof(0);
          dServof(60);
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
        xTaskCreate(vstrat2,"vstrat2", 4096, NULL, 1, NULL);
      }
    }
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}