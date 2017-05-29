//
// Created by lly on 23/04/2017.
//

#include "DocSequenceElement.h"

DocSequenceElement::DocSequenceElement(int line, int col, int level) {
    this->line = line;
    this->col = col;
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

DocElement **DocSequenceElement::editGet(int index) {
    if ((index >= 0) && (index < this->arr.size()))
        return &((this->arr)[index]);
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
        (*ite)->printTo(os);
    }
}

DocSequenceElement::~DocSequenceElement() {
    for (vector<DocElement*>::iterator ite = this->arr.begin();
            ite != this->arr.end();
            ++ite) {
        delete *ite;
    }
}