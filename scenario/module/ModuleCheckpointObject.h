//
// Created by lly on 23/09/2017.
//

#ifndef VPARSER_MODULECHECKPOINTOBJECT_H
#define VPARSER_MODULECHECKPOINTOBJECT_H


#include "../../schema/BaseObject.h"
#include "../../doc/DocElement.h"
#include "../../doc/DocObjectElement.h"
#include "../../doc/DocSequenceElement.h"
#include "../../data/ObjectDataObject.h"
#include "../../data/SequenceDataObject.h"
#include "../../data/DataObjectAdapter.h"
#include "../../error/Error.h"
#include "../../error/FieldIllegalError.h"
#include "../../error/FieldInvalidError.h"
#include "../../error/FieldMissError.h"
#include <string>
#include <vector>

using namespace std;

enum ModuleCheckpointType {
    INVALID_CHECKPOINT, INSET_CHECKPOINT, OUTSET_CHECKPOINT, ISSETSIZE_CHECKPOINT, EQUAL_CHECKPOINT, UNEQUAL_CHECKPOINT
};

class ModuleCheckpointObject: public BaseObject {
public:
    ModuleCheckpointObject();
    ~ModuleCheckpointObject();

    string getKeyName();
    virtual BaseDataObject* toDataObject() override;

    vector<string> fieldVec;
    ModuleCheckpointType type;
    string setName;
    /** just store it, but not own it **/
    DocElement *value;
    vector<string> excluding;
    vector<string> including;
};

struct ModuleCheckpointObjectFactory {
    static ModuleCheckpointObject *create(string filePath, string key, DocElement *ele);
};


#endif //VPARSER_MODULECHECKPOINTOBJECT_H
