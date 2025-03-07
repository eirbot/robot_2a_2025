#ifndef MENU_H
#define MENU_H
#include <Arduino.h>
#include <vector>




class Page {  
public:
    Page(std::string name, char type, int id);
    Page(std::string name, std::vector<Page*> elementsOfPage, Page* parentPage, char type, int id);
    void Next();
    void doAction();
    void Display();
    int getSize() { return _elementsOfPage.size(); }
    std::string _name;
    int _currentElement = 0;
    char _type;
    int _id;
    std::vector<Page*> _elementsOfPage;
};

extern Page* ptr_Menu;
extern Page* ptr_TestAct;
extern Page* currentDisplayedPage;

#endif