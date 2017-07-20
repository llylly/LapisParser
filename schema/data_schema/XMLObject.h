//
// Created by lly on 17/07/2017.
//

#ifndef VPARSER_XMLOBJECT_H
#define VPARSER_XMLOBJECT_H

#include <string>
#include "../BaseObject.h"
#include "../../error/Error.h"
#include "../../error/FieldInvalidError.h"
#include "../../error/FieldIllegalError.h"
#include "../../error/FieldMissError.h"
#include "../../doc/DocObjectElement.h"

using namespace std;

class XMLObject: public BaseObject {
public:
    bool hasName;
    string name;
    bool hasNameSpace;
    string _namespace;
    bool hasPrefix;
    string prefix;
    bool availAttribute;
    bool attribute;
    bool availWrapped;
    bool wrapped;

    BaseDataObject *toDataObject() override;

    XMLObject();
};

class XMLObjectFactory {
public:
    static XMLObject *create(string filePath, string fieldName, DocObjectElement *ele);
};


#endif //VPARSER_XMLOBJECT_H
