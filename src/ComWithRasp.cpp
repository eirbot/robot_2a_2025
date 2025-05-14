#include "ComWithRasp.hpp"

ComWithRasp::ComWithRasp(){
    Serial.begin(115200);
}

void ComWithRasp::Receive(){
    while(1){
        if(Serial.available()){
            rcv = Serial.read();
            commande += rcv;
            if(rcv == '\n'){
                processLine();
                commande = "";
            }
            else{
                if(commande.length() > MAX_COMMAND_LENGTH){
                    commande = "";
                    Serial.println("Commande trop longue");
                    break;
                }
            }
        }
        vTaskDelay(10);
    }
}

void ComWithRasp::Send(){
    Serial.println("Envoie de la commande");
    Serial.println(commande);
}

void ComWithRasp::processLine(){
     // Découper la ligne en morceaux (commande + params)
  int firstSpace = commande.indexOf(' ');
  String command;
  std::vector<int> params;

  if (firstSpace == -1) {
    command = commande;
  } else {
    command = commande.substring(0, firstSpace);
    String rest = commande.substring(firstSpace + 1);

    // Découper les paramètres
    int start = 0;
    while (start < rest.length()) {
      int end = rest.indexOf(' ', start);
      if (end == -1) end = rest.length();

      String token = rest.substring(start, end);
      params.push_back(token.toInt());

      start = end + 1;
    }
  }

  processCommand(command, params);
}

void ComWithRasp::processCommand(const String& cmd, const std::vector<int>& params) {
    if (cmd == "G" && params.size() == 3){
        Serial.println("GoToPosition");
        serialGoto.Go((float)params[0], (float)params[1], (float)params[2]);
    } else if (cmd == "C"){
        Serial.println("Cannette");
        pousserCanettes();
    }
}

void ComWithRasp::StartCom(){
    xTaskCreate([](void *){ComWithRasp com; com.Receive();}, "ComWithRasp",4000, NULL, 1, NULL );
}