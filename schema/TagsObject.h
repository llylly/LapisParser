//
// Created by lly on 29/05/2017.
//

#ifndef VPARSER_TAGSOBJECT_H
#define VPARSER_TAGSOBJECT_H

#include <string>
#include <vector>
#include "ExternalDocObject.h"
#include "../doc/DocSequenceElement.h"

using namespace std;

struct TagItem {
    string name;
    bool hasDescription;
    string description;
    bool hasExternalDocObject;
    ExternalDocObject externalDocObject;
};

struct TagsObject {
    vector<TagItem> vec;
};

struct TagsObjectFactory {
    static TagsObject *create(string filePath, DocSequenceElement *ele);
};


#endif //VPARSER_TAGSOBJECT_H
