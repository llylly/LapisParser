//
// Created by lly on 29/05/2017.
//

#ifndef VPARSER_EXTERNELDOCOBJECT_H
#define VPARSER_EXTERNELDOCOBJECT_H

#include <string>
#include "../doc/DocObjectElement.h"
#include "BaseObject.h"

using namespace std;

struct ExternalDocObject: public BaseObject {
    string url;
    string description;
    bool hasDescription;

public:
    ExternalDocObject();
    BaseDataObject* toDataObject() override;
};

struct ExternalDocObjectFactory {
    static ExternalDocObject* create(string filePath, string nodeName, DocObjectElement *ele);
};


#endif //VPARSER_EXTERNELDOCOBJECT_H
