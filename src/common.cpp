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
float Y_POS_INIT= 100;
float ANGLE_INIT= 0;

Page Test_total("Test Total", 't', 0);
Page Test_Pince("Test Pince", 't', 1);
Page Test_Servo("Test Servo", 't', 2);
Page Test_Pousseur("Test Pousseur", 't', 3);
std::vector<Page> elementsOfTest = {Test_total, Test_Pince, Test_Servo, Test_Pousseur};
Page Test_actionneurs("Test Actionneurs", elementsOfTest, 's', 0);

Page test_baniere("Test Baniere", 't', 4);
Page Test_vox("Test Vox", 't', 5);
std::vector<Page> elementsOfTest2 = {Test_actionneurs, test_baniere, Test_vox};
Page Test("Test", elementsOfTest2, 's', 0);

Page PositionX("Position X", 'c', 0);
Page PositionY("Position Y", 'c', 1);
Page PositionAngle("Position Angle", 'c', 2);

Page Position("Position", {PositionX, PositionY, PositionAngle}, 's', 0);

Page Strat_1("Stratégie 1", 't', 0);
Page Strat_2("Stratégie 2", 't', 1);
Page Strat_3("Stratégie 3", 't', 2);
Page Strat_4("Stratégie 4", 't', 3);
std::vector<Page> elementsOfStrat = {Strat_1, Strat_2, Strat_3, Strat_4};
Page Strat("Stratégies", elementsOfStrat, 's', 0);

Page Tof_1("TOF_1", 'c', 0);
Page Tof_2("TOF_2", 'c', 1);
Page Tof_3("TOF_3", 'c', 2);
Page Tof_4("TOF_4", 'c', 3);
Page Tof_5("TOF_5", 'c', 4);
Page Tof_6("TOF_6", 'c', 5);
std::vector<Page> elementsOfTof = {Tof_1, Tof_2, Tof_3, Tof_4, Tof_5,Tof_6};
Page Tof("TOF", elementsOfTof, 's', 0);

std::vector<Page> elementsOfPage = {Test, Position, Strat, Tof};
Page Menu_principal("Menu Principal", elementsOfPage, 's', 0);