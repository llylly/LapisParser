//
// Created by lly on 23/04/2017.
//

#include "DocSequenceElement.h"

DocSequenceElement::DocSequenceElement(int level) {
    this->type = DOC_SEQUENCE;
    this->level = level;
}

void DocSequenceElement::add(DocElement *ele) {
    arr.push_back(ele);
}

DocElement *DocSequenceElement::get(int index) {
    if ((index >= 0) && (index < this->arr.size()))
        return (this->arr)[index];
    else
        return NULL;
}

int DocSequenceElement::getLength() {
    return (int)this->arr.size();
}

void DocSequenceElement::printTo(std::ostream &os) {
    for (int i=0; i<level; ++i) os << "  ";
    os << "[array]" << endl;
    for (std::vector<DocElement*>::iterator ite = this->arr.begin();
            ite != this->arr.end();
            ++ite) {
        for (int i=0; i<=level; ++i) os << "  ";
        os << endl;
        (*ite)->printTo(os);
    }
}