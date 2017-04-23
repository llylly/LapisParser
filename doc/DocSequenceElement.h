//
// Created by lly on 23/04/2017.
//

#ifndef VPARSER_DOCSEQUENCEELEMENT_H
#define VPARSER_DOCSEQUENCEELEMENT_H

#include <vector>
#include "DocElement.h"

class DocSequenceElement: public DocElement {
public:
    DocSequenceElement(int level = 0);
    void add(DocElement *ele);
    DocElement *get(int index);
    int getLength();
    void printTo(std::ostream &os) override;

private:
    std::vector<DocElement*> arr;
};


#endif //VPARSER_DOCSEQUENCEELEMENT_H
