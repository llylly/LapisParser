//
// Created by lly on 23/04/2017.
//

#ifndef VPARSER_DOCELEMENT_H
#define VPARSER_DOCELEMENT_H

#include <string>
#include <vector>
#include <iostream>

using namespace std;

enum DOC_ELEMENT_TYPE {
    DOC_OBJECT, DOC_SCALAR, DOC_SEQUENCE
};

class DocElement {
public:
    DOC_ELEMENT_TYPE type;
    int level;

    virtual void printTo(std::ostream &os) = 0;

    static std::vector<DocElement*> docs;
};


#endif //VPARSER_DOCELEMENT_H
