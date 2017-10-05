//
// Created by lly on 03/10/2017.
//

#ifndef VPARSER_SCENARIOCONNECTIONOBJECT_H
#define VPARSER_SCENARIOCONNECTIONOBJECT_H


#include "../schema/BaseObject.h"
#include "../doc/DocElement.h"
#include "../doc/DocObjectElement.h"
#include "../doc/DocScalarElement.h"
#include "../doc/DocSequenceElement.h"
#include "../data/ObjectDataObject.h"
#include "../data/StringDataObject.h"
#include "../data/SequenceDataObject.h"
#include "../data/NumberDataObject.h"
#include "../error/FieldIllegalError.h"
#include "../error/FieldMissError.h"
#include "../error/FieldInvalidError.h"
#include <string>
#include <vector>

using namespace std;

class ScenarioConnectionObject: public BaseObject {
public:
    ScenarioConnectionObject();
    ~ScenarioConnectionObject();

    BaseDataObject *toDataObject() override;

    string from;
    string to;
    vector<string> including;
    vector<string> excluding;
    double weigh;
};

struct ScenarioConnectionObjectFactory {
    static ScenarioConnectionObject *create(string filePath, DocElement *ele);
};


#endif //VPARSER_SCENARIOCONNECTIONOBJECT_H
