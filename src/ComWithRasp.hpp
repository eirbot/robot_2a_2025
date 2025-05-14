#ifndef COM_WITH_RASP_H
#define COM_WITH_RASP_H

#include "Arduino.h"
#include "common.h"
#include "main.h"
#include "ClassMotors.hpp"
#include "GoToPosition.hpp"
#include "vector"

#define MAX_COMMAND_LENGTH 20

class ComWithRasp{
    public:
        ComWithRasp();
        void StartCom();

        void Receive();
        void Send();

        void processLine();
        void processCommand(const String& cmd, const std::vector<int>& params);
    private:
        char rcv;
        String commande; ;
};

#endif