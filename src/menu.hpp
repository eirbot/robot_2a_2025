#ifndef MENU_H
#define MENU_H
#include <Arduino.h>
#include <vector>



class Page {  
public:
    Page();
    Page(char* name, char type, int id);
    Page(char* name, char type, int id, Page parentPage);
    Page(char* name, std::vector<Page> elementsOfPage, char type, int id);
    Page(char* name, std::vector<Page> elementsOfPage, char type, int id, Page parentPage);
    
    char * _name;
    char _type; // 't' = test, 's' = folder, 'c' = constant
    int _id;
    int _currentElement;
    std::vector<Page> _elementsOfPage;

    void NextElement();
    void doElement(Page& currentDisplayPage);
    void hardwareInterface(Page& CurrentDisplayPage);
};


bool DISPLAY_FLAG_SELECT;
bool DISPLAY_FLAG_NEXT;

#endif