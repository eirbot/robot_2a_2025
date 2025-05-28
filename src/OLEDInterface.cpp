#include "OLEDInterface.hpp"

#define OLED_WIDTH      128
#define OLED_HEIGHT     64
#define OLED_RESET_PIN  -1
#define OLED_I2C_ADDR   0x3C

OLEDInterface::OLEDInterface()
    : _display(OLED_WIDTH, OLED_HEIGHT, &Wire, OLED_RESET_PIN) {}

bool OLEDInterface::begin() {
    if (!_display.begin(SSD1306_SWITCHCAPVCC, OLED_I2C_ADDR)) {
        return false;
    }
    _display.clearDisplay();
    _display.setTextSize(1);
    _display.setTextColor(WHITE);
    _display.setFont(&FreeSans9pt7b);
    _display.display();
    return true;
}

void OLEDInterface::refresh() {
    _display.clearDisplay();
    _display.display();
}

void OLEDInterface::clearLine(int ligne, int colonne) {
    _display.fillRect(colonne, ligne - 12, 115, 13, BLACK);
}

void OLEDInterface::afficherTexte(const char* message, int ligne, int colonne) {
    clearLine(ligne, colonne);
    _display.setCursor(colonne, ligne);
    _display.println(message);
    _display.display();
}

void OLEDInterface::afficherTexte(int message, int ligne, int colonne) {
    clearLine(ligne, colonne);
    _display.setCursor(colonne, ligne);
    _display.println(message);
    _display.display();
}

void OLEDInterface::afficherTexte(float message, int ligne, int colonne) {
    clearLine(ligne, colonne);
    _display.setCursor(colonne, ligne);
    _display.println(message);
    _display.display();
}

void OLEDInterface::tailleTexte(int taille) {
    switch (taille) {
        case 1:
            _display.setTextSize(1);
            _display.setFont();
            break;
        case 2:
            _display.setTextSize(1);
            _display.setFont(&FreeSans9pt7b);
            break;
        case 3:
            _display.setTextSize(2);
            _display.setFont();
            break;
        case 4:
            _display.setTextSize(2);
            _display.setFont(&FreeSans9pt7b);
            break;
        case 5:
            _display.setTextSize(5);
            _display.setFont(&FreeSans9pt7b);
        default:
            _display.setTextSize(1);
    }
}

// === Menus / Écrans publics ===

void OLEDInterface::afficherMenuPrincipal() {
    refresh();

    tailleTexte(2);
    afficherTexte("=== Menu ===", 15, 0);
    afficherTexte("1. Start", 30, 0);
    afficherTexte("2. Reglages", 45, 0);
    afficherTexte("3. Quitter", 60, 0);
}

void OLEDInterface::afficherErreurCapteur(const char* message) {
    refresh();

    tailleTexte(2);
    afficherTexte("ERREUR CAPTEUR", 20, 0);
    afficherTexte(message, 35, 0);
}

void OLEDInterface::afficherDebug() {
    refresh();

    tailleTexte(2);
    afficherTexte("Debug mode:", 15, 0);

    tailleTexte(1);
    _display.setCursor(0, 25);
    _display.print("x: ");
    _display.println(serialGoto.x_initial);

    _display.setCursor(0, 35);
    _display.print("y: ");
    _display.println(serialGoto.y_initial);

    _display.setCursor(0, 45);
    _display.print("angle: ");
    _display.println(serialGoto.cangle_initial);

    _display.display();
}

void OLEDInterface::afficherScore(int score) {
    refresh();
    tailleTexte(5);
    _display.setCursor(10, 60);
    _display.println(score);

    _display.display();
}

#define MAX_ELEMENTS_PER_PAGE 5 // a voir avec l'ecran

void OLEDInterface::displayPage(Page page) {
    refresh();
    tailleTexte(1);
    afficherTexte(page._name, 0, 0);

    int startLine = 12;
    int lineHeight = 12;

    for (size_t i = 0; i < page._elementsOfPage.size() && i < MAX_ELEMENTS_PER_PAGE; ++i) {
        const Page& element = page._elementsOfPage[i];
        afficherTexte(element._name, startLine + i * lineHeight, 0);
    }

    _display.display();
}