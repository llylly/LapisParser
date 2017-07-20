//
// Created by lly on 29/05/2017.
//

#ifndef VPARSER_TAGSOBJECT_H
#define VPARSER_TAGSOBJECT_H

#include <string>
#include <vector>
#include "ExternalDocObject.h"
#include "../doc/DocSequenceElement.h"
#include "BaseObject.h"

using namespace std;

struct TagItem {
    string name;
    bool hasDescription;
    string description;
    bool hasExternalDocObject;
    ExternalDocObject externalDocObject;

    TagItem();
};

struct TagsObject: public BaseObject {
    vector<TagItem> vec;

public:
    BaseDataObject* toDataObject() override;
};

struct TagsObjectFactory {
    static TagsObject *create(string filePath, DocSequenceElement *ele);
};


#endif //VPARSER_TAGSOBJECT_H
