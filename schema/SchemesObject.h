//
// Created by lly on 29/05/2017.
//

#ifndef VPARSER_SCHEMESOBJECT_H
#define VPARSER_SCHEMESOBJECT_H

#include "../doc/DocScalarElement.h"
#include "../doc/DocSequenceElement.h"
#include "BaseObject.h"

enum SUPPORT_SCHEME {INVALID, HTTP, HTTPS};

struct SchemesObject: public BaseObject {
    bool http;
    bool https;

public:
    SchemesObject();
    BaseDataObject* toDataObject() override;
};

struct SchemesObjectFactory {
    static SchemesObject *create(string filePath, DocSequenceElement* obj);
};


#endif //VPARSER_SCHEMESOBJECT_H
