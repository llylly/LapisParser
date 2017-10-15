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
    os << "[" << endl;
    for (std::vector<DocElement*>::iterator ite = this->arr.begin();
            ite != this->arr.end();
            ++ite) {
        (*ite)->printTo(os);
    }
    for (int i=0; i<level; ++i) os << "  ";
    os << "]" << endl;
}

DocSequenceElement::~DocSequenceElement() {
    for (vector<DocElement*>::iterator ite = this->arr.begin();
            ite != this->arr.end();
            ++ite) {
        delete *ite;
    }
}

bool DocSequenceElement::equals(DocElement *ele) {
    if (ele == NULL) return false;
    if (ele->type != DOC_SEQUENCE) return false;
    DocSequenceElement *seqEle = (DocSequenceElement*)ele;
    if (this->getLength() != seqEle->getLength()) return false;
    int len = this->getLength();
    for (int i=0; i<len; ++i) {
        DocElement *nowItem = this->get(i);
        if (nowItem == NULL) return false;
        if (!nowItem->equals(seqEle->get(i))) return false;
    }
    return true;
}