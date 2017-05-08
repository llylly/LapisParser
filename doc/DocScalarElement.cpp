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