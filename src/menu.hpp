#ifndef MENU_H
#define MENU_H
#include <Arduino.h>
#include <vector>




class Page {  
public:

    Page(const char* name, char type, int id);
    Page(const char* name, std::vector<Page *> elementsOfPage, char type, int id);
    
    const char * _name;
    char _type; // 't' = test, 's' = folder, 'c' = constant
    int _id;
    int _currentElement;
    std::vector<Page*> _elementsOfPage;

    void NextElement();
    void doElement(Page * currentDisplayPage);
    void addParent(Page * parentPage);
};


#endif