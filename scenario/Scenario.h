//
// Created by lly on 22/09/2017.
//

#ifndef VPARSER_SCENARIO_H
#define VPARSER_SCENARIO_H

#include <string>
#include "../schema/BaseObject.h"
#include "../schema/Controller.h"
#include "../error/InvalidSetError.h"
#include "../error/InvalidStateError.h"
#include "ScenarioInfoObject.h"
#include "ScenarioStateObject.h"
#include "ScenarioSetObject.h"
#include "ScenarioModuleObject.h"
#include "ScenarioConnectionObject.h"

using namespace std;

class Scenario: public BaseObject {
public:
    Scenario();
    ~Scenario();

    BaseDataObject *toDataObject() override;

    ScenarioInfoObject *info;
    map<string, ScenarioStateObject*> states;
    string initialState;
    map<string, ScenarioSetObject*> sets;
    map<string, ScenarioModuleObject*> modules;
    string startModule;
    vector<ScenarioConnectionObject*> connections;
};

struct ScenarioFactory {
    static Scenario *create(string filePath, DocObjectElement *ele, Controller *controller);
};

#endif //VPARSER_SCENARIO_H
