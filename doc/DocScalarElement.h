//
// Created by lly on 23/04/2017.
//

#ifndef VPARSER_DOCSCALARELEMENT_H
#define VPARSER_DOCSCALARELEMENT_H

#include "DocElement.h"

class DocScalarElement: public DocElement {
public:
    DocScalarElement(int line, int col, std::string _value, int level = 0);
    std::string getValue();
    void printTo(std::ostream &os) override;

private:
    std::string value;
};


#endif //VPARSER_DOCSCALARELEMENT_H
