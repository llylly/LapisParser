//
// Created by lly on 22/09/2017.
//

#ifndef VPARSER_SCENARIOSTATEOBJECT_H
#define VPARSER_SCENARIOSTATEOBJECT_H


#include "../schema/BaseObject.h"
#include "../doc/DocElement.h"
#include <string>

using namespace std;

class ScenarioStateObject: public BaseObject {
public:
    ScenarioStateObject();
    virtual BaseDataObject* toDataObject() override;

    string name;
    string description;
    bool isInitial;
};

struct ScenarioStateObjectFactory {
    static ScenarioStateObject *create(string filePath, DocElement *ele);
};


#endif //VPARSER_SCENARIOSTATEOBJECT_H
