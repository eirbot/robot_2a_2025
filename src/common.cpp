#include "common.h"

TaskHandle_t vMotorsHandle;
TaskHandle_t handleDoStrat = NULL;


AccelStepper moteurGauche(AccelStepper::DRIVER, STEPG, DIRG); // STEP, DIR
AccelStepper moteurDroit(AccelStepper::DRIVER, STEPD, DIRD);  // STEP, DIR

ClassMotors mot;

volatile bool* FLAG_CLEAR = NULL; // Valeur initiale (1 = continue, 0 = stop)
bool FLAG_STOP = false; // Valeur initiale (1 = stop, 0 = continue)
bool FLAG_TIRETTE = false;
bool FLAG_DEBUG = true; // Valeur initiale (1 = debug, 0 = normal)
bool FLAG_TOF = false; // Valeur initiale (1 = TOF actif, 0 = inactif)

SemaphoreHandle_t xPositionMutex;

bool jaune=true;

//position bleu par default
float X_POS_INIT= 1775;
float Y_POS_INIT= 220;
float ANGLE_INIT= 0;



Page Test_Total("Total", 't', 0);
Page Test_Pince("Pince", 't', 1);
Page Test_Servo("Servo", 't', 2);
Page Test_Pousseur("Pousseur", 't', 3);
std::vector<Page*> elementsOfActionneur = {&Test_Total, &Test_Pince, &Test_Servo, &Test_Pousseur}; //, Test_Servo, Test_Pousseur};
Page Test_Actionneur_Avant("Actionneur Avant", elementsOfActionneur, 's',0);

Page Test_Baniere("Baniere", 't', 4);
Page Test_Vox("Vox", 't', 5);
std::vector<Page*> elementOfTest = {&Test_Baniere, &Test_Actionneur_Avant, &Test_Vox};
Page Test("Test", elementOfTest, 's', 0);

Page PositionX("Position X", 'c', 0);
Page PositionY("Position Y", 'c', 1);
Page PositionAngle("Position Angle", 'c', 2);
std::vector<Page*> elementOfPosition = {&PositionX, &PositionY, &PositionAngle};
Page Position("Position", elementOfPosition, 's', 0);

Page Strategy1("Strat 1", 't', 10);
Page Strategy2("Strat 2", 't', 11);
std::vector<Page*> elementOfStrategy = {&Strategy1, &Strategy2};
Page Strategie("Strategie", elementOfStrategy,'s',0);

Page Tof1("Tof1", 'c', 10);
Page Tof2("Tof2", 'c', 11);
Page Tof3("Tof3", 'c', 12);
Page Tof4("Tof4", 'c', 13);
Page Tof5("Tof5", 'c', 14);
Page Tof6("Tof6", 'c', 15);
Page Tof7("Tof7", 'c', 16);
std::vector<Page*> elementOfTof = {&Tof1,&Tof2,&Tof3,&Tof4,&Tof5,&Tof6,&Tof7};
Page Tof("Tofs", elementOfTof, 's', 0);


std::vector<Page*> elementOfMenu = {&Test, &Position, &Strategie, &Tof};
Page Menu("Menu",elementOfMenu, 's', 0);

