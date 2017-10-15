//
// Created by lly on 23/04/2017.
//

#include "DocScalarElement.h"

DocScalarElement::DocScalarElement(int line, int col, std::string _value, int level) {
    this->line = line;
    this->col = col;
    this->type = DOC_SCALAR;
    this->level = level;
    this->value = _value;
}

std::string DocScalarElement::getValue() {
    return this->value;
}

void DocScalarElement::printTo(std::ostream &os) {
    for (int i=0; i<level; ++i)
        os << "  ";
    os << value << endl;
}

DocScalarElement::~DocScalarElement() {}

bool DocScalarElement::equals(DocElement *ele) {
    if (ele == NULL) return false;
    if (ele->type != DOC_SCALAR) return false;
    DocScalarElement *scaEle = (DocScalarElement*)ele;
    return this->value == scaEle->value;
}
