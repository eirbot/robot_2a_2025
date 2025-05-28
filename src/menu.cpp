#include "menu.hpp"
 

void testing(int id) {
    Serial.print("Testing ID: ");
    Serial.println(id);
}

void updateValue(int id) {
    Serial.print("Updating value for ID: ");
    Serial.println(id);
}

Page::Page(char* name, char type, int id) {
    _name = name;
    _type = type;
    _id = id;
    _currentElement = 0;
    _elementsOfPage.clear();
}

Page::Page(char* name, char type, int id, Page parentPage) {
    _name = name;
    _type = type;
    _id = id;
    _currentElement = 0;
    _elementsOfPage.clear();
    _elementsOfPage.push_back(parentPage);
}

Page::Page(char* name, std::vector<Page> elementsOfPage, char type, int id) {
    _name = name;
    _type = type;
    _id = id;
    _currentElement = 0;
    _elementsOfPage = elementsOfPage;
}

Page::Page(char* name, std::vector<Page> elementsOfPage, char type, int id, Page parentPage){
    _name = name;
    _type = type;
    _id = id;
    _currentElement = 0;
    _elementsOfPage = elementsOfPage;
    _elementsOfPage.push_back(parentPage);
}


void Page::NextElement() {
    if (_currentElement < _elementsOfPage.size() - 1) {
        _currentElement++;
    } else {
        _currentElement = 0; // Loop back to the first element
    }
}

void Page::doElement(Page& CurrentDisplayPage) {
    if (_elementsOfPage[_currentElement]._type == 't') {
        // Call the testing function with the ID of the current element
        testing(_elementsOfPage[_currentElement]._id);
    } else if (_elementsOfPage[_currentElement]._type == 's') {
        // Navigate to the sub-page
        CurrentDisplayPage = _elementsOfPage[_currentElement];
    } else if (_elementsOfPage[_currentElement]._type == 'c') {
        // Handle constant type, if needed
        updateValue(_elementsOfPage[_currentElement]._id);
    }
}

void Page::hardwareInterface(Page& CurrentDisplayPage){
    if(DISPLAY_FLAG_SELECT){
        this->doElement(CurrentDisplayPage);
        DISPLAY_FLAG_SELECT = false;
    }
    else if (DISPLAY_FLAG_NEXT){
        this->NextElement();
        DISPLAY_FLAG_NEXT = false;
    }
}