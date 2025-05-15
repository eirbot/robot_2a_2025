#include "ComWithRasp.hpp"

ComWithRasp::ComWithRasp() {
    Serial.begin(115200);
}

void ComWithRasp::StartCom() {
    // Crée une tâche FreeRTOS qui appelle this->Receive()
    xTaskCreate([](void* obj) {
        static_cast<ComWithRasp*>(obj)->Receive();
    }, "ComWithRasp", 4000, this, 1, NULL);
}

void ComWithRasp::Receive() {
    while (1) {
        if (Serial.available()) {
            rcv = Serial.read();
            commande += rcv;

            if (rcv == '\n') {
                processLine();
                commande = "";
            } else if (commande.length() > MAX_COMMAND_LENGTH) {
                commande = "";
                Serial.println("Commande trop longue");
            }
        }
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

void ComWithRasp::Send() {
    Serial.println("Envoi de la commande");
    Serial.println(commande);
}

void ComWithRasp::processLine() {
    int firstSpace = commande.indexOf(' ');
    String command;
    std::vector<int> params;

    if (firstSpace == -1) {
        command = commande;
    } else {
        command = commande.substring(0, firstSpace);
        String rest = commande.substring(firstSpace + 1);

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
    } else if (cmd == "C") {
        Serial.println("Cannette");
        pousserCanettes();
    } else {
        Serial.println("Commande inconnue");
    }
}
