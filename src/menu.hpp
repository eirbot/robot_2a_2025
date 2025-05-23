#include <Arduino.h>
#include <vector>




class Page {  
public:
    Page(char* name, std::vector<Page*> elementsOfPage);
    Page(char* name, std::vector<Page*> elementsOfPage, Page* parentPage, char type, int id);
    void Next();
    void doAction();
    int getSize() { return _elementsOfPage.size(); }
    char* _name;
    int _currentElement = 0;
    char _type;
    int _id;
    std::vector<Page*> _elementsOfPage;
};
