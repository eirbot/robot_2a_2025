#ifndef COM_WITH_RASP_HPP
#define COM_WITH_RASP_HPP

#include <Arduino.h>
#include <vector>
#include "common.h"
#include "ClassMotors.hpp"
#include "GoToPosition.hpp"
#include "actionneur.h"
#include "main.h"

class ComWithRasp {
public:
    ComWithRasp();
    void StartCom();         // Lance la tâche FreeRTOS
    void Send();             // Envoie la commande (debug/test)
    
private:
    void Receive();          // Lecture UART dans une boucle
    void processLine();      // Découpe et traite la commande
    void processCommand(const String& cmd, const std::vector<int>& params);

    String commande;
    char rcv;
    static constexpr int MAX_COMMAND_LENGTH = 64;
};


#endif