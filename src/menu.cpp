#include "menu.hpp"
 

void testing(int id) {
    Serial.print("Testing ID: ");
    Serial.println(id);
}
Page * currentDisplayedPage = nullptr;
Page::Page(char* name, std::vector<Page*> elementsOfPage)
    : _name(name), _elementsOfPage(elementsOfPage) {}

Page::Page(char* name, std::vector<Page*> elementsOfPage, Page* parentPage, char type, int id){
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
