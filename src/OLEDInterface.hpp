#pragma once

#include "Arduino.h"
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSans24pt7b.h>
#include "main_robot.h"
#include "common.h"
#include "menu.hpp"

class OLEDInterface {
public:
    OLEDInterface();

    bool begin();

    void afficherMenuPrincipal();
    void afficherErreurCapteur(const char* message);
    void afficherDebug();
    void afficherScore(int score);
    void displayPage(Page * page);

private:
    Adafruit_SSD1306 _display;

    void tailleTexte(int taille);
    void afficherTexte(const char* message, int ligne, int colonne);
    void afficherTexte(int message, int ligne, int colonne);
    void afficherTexte(float message, int ligne, int colonne);
    void clearLine(int ligne, int colonne);
    void refresh();
};
