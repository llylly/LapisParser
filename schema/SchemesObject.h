//
// Created by lly on 29/05/2017.
//

#ifndef VPARSER_SCHEMESOBJECT_H
#define VPARSER_SCHEMESOBJECT_H

#include "../doc/DocScalarElement.h"
#include "../doc/DocSequenceElement.h"

enum SUPPORT_SCHEME {INVALID, HTTP, HTTPS};

struct SchemesObject {
    bool http;
    bool https;
};

struct SchemesObjectFactory {
    static SchemesObject *create(string filePath, DocSequenceElement* obj);
};


#endif //VPARSER_SCHEMESOBJECT_H
