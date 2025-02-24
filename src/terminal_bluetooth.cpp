#include "terminal_bluetooth.h"
#include "actionneur.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif
BluetoothSerial SerialBT;

void loop() {
    if(SerialBT.available()){
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
          gServo.write(value);
      }
      else if (id.startsWith("servod")) {      
          dServo.write(value);
      }
      else if (id.startsWith("servoh")) {      
          hServo.write(value);
      }
      else if (id.startsWith("reset")) {      
          hServo.write(400);
          gServo.write(0);
          dServo.write(60);
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
  
          avancer(value, vit);
          delay(800);
  
          gauche(90, vit);
          delay(800);
        }
      }
      else if (id.startsWith("a-r")) {      
          avancer(value, vit);
          delay(2000);
  
          reculer(value, vit);
          delay(2000);
      }
      else if (id.startsWith("gauchedroite")) {      
        gauche(90, vit);
        delay(2000);
  
        droite(180, vit);
        delay(2000);
  
        gauche(90, vit);
        delay(2000);
      }
      else if (id.startsWith("strat")) {      
        avancer(500, vit);
        delay(800);
  
        droite(90, vit);
        delay(800);
  
        avancer(450, vit);
        delay(800);
  
        droite(90, vit);
        delay(800);
  
        avancer(330, vit);
        delay(800);
  
        pousserCanettes();
        delay(800);
  
        reculer(500, vit);
        delay(800);
  
        gauche(165, vit);
        delay(800);
  
        // avancer(1300, vit);
        // delay(800);
      }
    }
    stop();
  }