//
// Created by lly on 23/09/2017.
//

#ifndef VPARSER_SCENARIOMODULEOBJECT_H
#define VPARSER_SCENARIOMODULEOBJECT_H


#include "../schema/BaseObject.h"
#include "../schema/APIObject.h"
#include "module/ModuleRepeatObject.h"
#include "module/ModuleInputConstraintObject.h"
#include "module/ModuleCheckpointObject.h"
#include "module/ModuleSetEffectObject.h"
#include "module/ModuleTransStateObject.h"
#include "../schema/Controller.h"
#include <string>

using namespace std;

enum ScenarioModuleType {
    SCENARIO_START_MODULE, SCENARIO_END_MODULE, SCENARIO_NORMAL_MODULE, SCENARIO_EMPTY_MODULE
};

class ScenarioModuleObject: public BaseObject {
public:
    ScenarioModuleObject();
    ~ScenarioModuleObject();
    virtual BaseDataObject* toDataObject() override;

    string name;
    ScenarioModuleType type;

        /* Caution: this API is only a reference of real API object, which is stored by APIPool
         * So DO NOT delete the API object here */
        APIObject *api;
        ModuleRepeatObject *repeat;
        vector<ModuleInputConstraintObject*> inputs;
        vector<ModuleCheckpointObject*> checkpoints;
        vector<ModuleSetEffectObject*> setEffects;
        vector<ModuleTransStateObject*> transStates;

};

struct ScenarioModuleObjectFactory {
    static ScenarioModuleObject *create(string filePath, DocElement *ele, Controller *controller);
};


#endif //VPARSER_SCENARIOMODULEOBJECT_H
