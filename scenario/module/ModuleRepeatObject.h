//
// Created by lly on 23/09/2017.
//

#ifndef VPARSER_MODULEREPEATOBJECT_H
#define VPARSER_MODULEREPEATOBJECT_H


#include "../../schema/BaseObject.h"
#include "../../doc/DocElement.h"
#include "../../error/FieldIllegalError.h"
#include "../../error/FieldInvalidError.h"
#include "../../error/FieldMissError.h"
#include <string>
#include <vector>

using namespace std;

class ModuleRepeatObject: public BaseObject {
public:
    ModuleRepeatObject();
    ~ModuleRepeatObject();
    virtual BaseDataObject* toDataObject() override;

    int time;
    vector<string> excluding;
    vector<string> including;
};

struct ModuleRepeatObjectFactory {
    static ModuleRepeatObject *createDefault();
    static ModuleRepeatObject *create(string filePath, DocElement *ele);
};

#endif //VPARSER_MODULEREPEATOBJECT_H
