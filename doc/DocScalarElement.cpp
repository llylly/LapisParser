//
// Created by lly on 23/04/2017.
//

#include "DocScalarElement.h"

DocScalarElement::DocScalarElement(std::string _value, int level) {
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