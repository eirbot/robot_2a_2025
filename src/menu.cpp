#include "menu.hpp"
 

void testing(int id) {
    Serial.print("Testing ID: ");
    Serial.println(id);
}

void updateValue(int id) {
    Serial.print("Updating value for ID: ");
    Serial.println(id);
}

Page::Page(const char* name, char type, int id) {
    _name = name;
    _type = type;
    _id = id;
    _currentElement = 0;
}

Page::Page(const char* name, char type, int id, Page * parentPage) {
    _name = name;
    _type = type;
    _id = id;
    _currentElement = 0;
    _elementsOfPage.push_back(parentPage);
}

Page::Page(const char* name, std::vector<Page *> elementsOfPage, char type, int id) {
    _name = name;
    _type = type;
    _id = id;
    _currentElement = 0;
    // _elementsOfPage = elementsOfPage;
    for (const auto& element : elementsOfPage) {
        _elementsOfPage.push_back(element);
    }
}

Page::Page(const char* name, std::vector<Page *> elementsOfPage, char type, int id, Page * parentPage){
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

Page* Page::doElement(Page*  CurrentDisplayPage) {
    if (_elementsOfPage[_currentElement]->_type == 't') {
        // Call the testing function with the ID of the current element
        testing(_elementsOfPage[_currentElement]->_id);
        return CurrentDisplayPage; // Return the current display page after testing
    } else if (_elementsOfPage[_currentElement]->_type == 's') {
        // Navigate to the sub-page
        return _elementsOfPage[_currentElement];
    } else if (_elementsOfPage[_currentElement]->_type == 'c') {
        // Handle constant type, if needed
        updateValue(_elementsOfPage[_currentElement]->_id);
        return CurrentDisplayPage; // Return the current display page after updating value
    }
    else{
        return CurrentDisplayPage; // If no valid type, return current display page
    }
}


void Page::addParent(Page * parentPage) {
    _elementsOfPage.push_back(parentPage);
}
