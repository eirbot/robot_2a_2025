#include "menu.hpp"
 

void testing(int id) {
    Serial.print("Testing ID: ");
    Serial.println(id);
}


Page::Page(std::string name, char type, int id)
{
    _name = name;
    _type = type;
    _id = id;
    _currentElement = 0;
}

Page::Page(std::string name, std::vector<Page*> elementsOfPage, Page* parentPage, char type, int id){
    _name = name;
    _elementsOfPage = elementsOfPage;
    _elementsOfPage.push_back(parentPage);
    _type = type;
    _id = id;
}

void Page::Next() {
    if (_currentElement == _elementsOfPage.size() - 1) {
        _currentElement = 0;
    } else {
        _currentElement++;
    }    
}

void Page::doAction() {
    if (_elementsOfPage[_currentElement]->_type == 't') {       //test
        testing(_elementsOfPage[_currentElement]->_id);
    }
    else if (_elementsOfPage[_currentElement]->_type == 's') {  //folder
        currentDisplayedPage = _elementsOfPage[_currentElement];
    }
    else if (_elementsOfPage[_currentElement]->_type == 'c') {  //constant        
    }
}




Page Test_total("TestTotal", 't', 0);
Page Test_pince("TestPince", 't', 1);
Page Test_servo("TestServo", 't', 2);
Page Test_pouseurs("TestPousseur", 't', 3);
std::vector<Page*> testActionneurAvant = {&Test_total, &Test_total, &Test_servo, &Test_pouseurs};


Page Test_baniere("TestBaniere", 't', 4);
Page Test_vox("TestVox", 't', 5);
std::vector<Page*> testActionneur = {&Test_baniere, &Test_vox};


Page Position_x("PositionX", 'c', 0);
Page Position_y("PositionY", 'c', 1);
std::vector<Page*> position = {&Position_x, &Position_y};


Page Strategy_1("Strategy_1", 't', 6);
Page Strategy_2("Strategy_2", 't', 7);
Page Strategy_3("Strategy_3", 't', 8);
Page Strategy_4("Strategy_4", 't', 9);
Page Strategy_5("Strategy_5", 't', 10);
Page Strategy_6("Strategy_6", 't', 11);
std::vector<Page*> strategy = {&Strategy_1, &Strategy_2, &Strategy_3, &Strategy_4, &Strategy_5, &Strategy_6};


Page Tof_1("Tof_1", 'c', 2);
Page Tof_2("Tof_2", 'c', 3);
Page Tof_3("Tof_3", 'c', 4);
Page Tof_4("Tof_4", 'c', 5);
Page Tof_5("Tof_5", 'c', 6);
Page Tof_6("Tof_6", 'c', 7);
std::vector<Page*> tof = {&Tof_1, &Tof_2, &Tof_3, &Tof_4, &Tof_5, &Tof_6};


Page DisplayMenu("Menu", 's', 0);
Page* ptr_Menu = &DisplayMenu;

Page DisplayTestAct("Actionneur", testActionneur, ptr_Menu, 's', 0);
Page* ptr_TestAct = &DisplayTestAct;

Page DisplayTestActAvant("ActAvant", testActionneurAvant, ptr_TestAct, 's', 0);

Page DisplayPosition("Position", position, ptr_Menu, 's', 0);

Page DisplayStrategy("Strategy", strategy, ptr_Menu, 's', 0);

Page DisplayTof("Tof", tof, ptr_Menu, 's', 0);

Page * currentDisplayedPage = &DisplayMenu;