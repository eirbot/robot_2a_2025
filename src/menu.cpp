#include "menu.hpp"
 
void testing(int id);
void updateValue(int id);

Page::Page(const char* name, char type, int id) {
    _name = name;
    _type = type;
    _id = id;
    _currentElement = 0;
}


Page::Page(const char* name, std::vector<Page*> elementsOfPage, char type, int id) {
    _name = name;
    _type = type;
    _id = id;
    _currentElement = 0;
    for (auto element : elementsOfPage)
    {
        _elementsOfPage.push_back(element);
    }
}



void Page::NextElement() {
    if (_currentElement < _elementsOfPage.size() - 1) {
        _currentElement++;
    } else {
        _currentElement = 0; // Loop back to the first element
    }
}

void Page::doElement(Page * CurrentDisplayPage) {
    if (_elementsOfPage[_currentElement]->_type == 't') {
        // Call the testing function with the ID of the current element
        testing(_elementsOfPage[_currentElement]->_id);
    } else if (_elementsOfPage[_currentElement]->_type == 's') {
        // Navigate to the sub-page
        CurrentDisplayPage = _elementsOfPage[_currentElement];
    } else if (_elementsOfPage[_currentElement]->_type == 'c') {
        // Handle constant type, if needed
        updateValue(_elementsOfPage[_currentElement]->_id);
    }
}

void Page::addParent(Page * parentPage){
    _elementsOfPage.push_back(parentPage);
}