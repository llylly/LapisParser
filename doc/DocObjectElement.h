//
// Created by lly on 23/04/2017.
//

#ifndef VPARSER_DOCOBJECTELEMENT_H
#define VPARSER_DOCOBJECTELEMENT_H

#include <map>
#include "DocElement.h"

class DocObjectElement: public DocElement {
public:
    DocObjectElement(int line, int col, int level = 0);
    void add(std::string key, DocElement *ele);
    DocElement *get(std::string key);
    int getSize();
    void printTo(std::ostream &os) override;

private:
    std::map<std::string, DocElement*> member;
};


#endif //VPARSER_DOCOBJECTELEMENT_H