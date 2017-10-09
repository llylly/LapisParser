//
// Created by lly on 23/09/2017.
//

#ifndef VPARSER_MODULETRANSSTATEOBJECT_H
#define VPARSER_MODULETRANSSTATEOBJECT_H


#include "../../schema/BaseObject.h"
#include "../../doc/DocElement.h"
#include "../../doc/DocObjectElement.h"
#include "../../doc/DocSequenceElement.h"
#include "../../doc/DocScalarElement.h"
#include "../../data/ObjectDataObject.h"
#include "../../data/StringDataObject.h"
#include "../../data/SequenceDataObject.h"
#include "../../error/Error.h"
#include "../../error/FieldMissError.h"
#include "../../error/FieldIllegalError.h"
#include "../../error/FieldInvalidError.h"
#include <string>
#include <vector>

using namespace std;

class ModuleTransStateObject: public BaseObject {
public:
    ModuleTransStateObject();
    ~ModuleTransStateObject();

    virtual BaseDataObject *toDataObject() override;

    string preState;
    string nexState;
    vector<string> excluding;
    vector<string> including;
};

struct ModuleTransStateObjectFactory {
    static ModuleTransStateObject *create(string nexState);
    static ModuleTransStateObject *create(string filePath, DocElement *ele);
};


#endif //VPARSER_MODULETRANSSTATEOBJECT_H
