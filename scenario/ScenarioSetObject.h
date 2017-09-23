//
// Created by lly on 22/09/2017.
//

#ifndef VPARSER_SCENARIOSETOBJECT_H
#define VPARSER_SCENARIOSETOBJECT_H


#include "../schema/BaseObject.h"
#include "../doc/DocElement.h"
#include <string>
#include <vector>

using namespace std;

class ScenarioSetObject: public BaseObject {
public:
    ScenarioSetObject();
    virtual BaseDataObject* toDataObject() override;

    string name;
    bool unique;
    /* Caution: It only stores doc elements which represent the default values.
     * They may not unique. And the class doesn't own them */
    vector<DocElement*> _default;
};

struct ScenarioSetObjectFactory {
    static ScenarioSetObject *create(string filePath, DocElement *ele);
};


#endif //VPARSER_SCENARIOSETOBJECT_H
