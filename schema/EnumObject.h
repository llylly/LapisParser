//
// Created by lly on 29/05/2017.
//

#ifndef VPARSER_ENUMOBJECT_H
#define VPARSER_ENUMOBJECT_H

#include <vector>
#include "../doc/DocSequenceElement.h"

using namespace std;

struct EnumObject {
    vector<string> vec;
};

struct EnumObjectFactory {
    static EnumObject *create(string filePath, string nodeName, DocSequenceElement *ele);
};


#endif //VPARSER_ENUMOBJECT_H
