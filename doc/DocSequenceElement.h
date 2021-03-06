//
// Created by lly on 23/04/2017.
//

#ifndef VPARSER_DOCSEQUENCEELEMENT_H
#define VPARSER_DOCSEQUENCEELEMENT_H

#include <vector>
#include "DocElement.h"

class DocSequenceElement: public DocElement {
public:
    DocSequenceElement(int line, int col, int level = 0);
    void add(DocElement *ele);
    DocElement *get(int index);
    DocElement **editGet(int index);
    int getLength();
    void printTo(std::ostream &os) override;
    ~DocSequenceElement();
    bool equals(DocElement *ele) override;

private:
    std::vector<DocElement*> arr;
};


#endif //VPARSER_DOCSEQUENCEELEMENT_H
