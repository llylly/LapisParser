//
// Created by lly on 23/09/2017.
//

#include "ScenarioModuleObject.h"

ScenarioModuleObject::ScenarioModuleObject() {
    this->name = "";
    this->type = SCENARIO_NORMAL_MODULE;
    this->api = NULL;
    this->repeat = NULL;
    this->inputs.clear();
    this->checkpoints.clear();
    this->setEffects.clear();
    this->transStates.clear();
}

ScenarioModuleObject::~ScenarioModuleObject() {
    delete this->repeat;
    for (vector<ModuleInputConstraintObject*>::iterator ite = this->inputs.begin();
            ite != this->inputs.end();
            ++ite) {
        delete *ite;
    }
    this->inputs.clear();
    for (vector<ModuleCheckpointObject*>::iterator ite = this->checkpoints.begin();
            ite != this->checkpoints.end();
            ++ite) {
        delete *ite;
    }
    this->checkpoints.clear();
    for (vector<ModuleSetEffectObject*>::iterator ite = this->setEffects.begin();
            ite !=this->setEffects.end();
            ++ite) {
        delete *ite;
    }
    this->setEffects.clear();
    for (vector<ModuleTransStateObject*>::iterator ite = this->transStates.begin();
            ite != this->transStates.end();
            ++ite) {
        delete *ite;
    }
    this->transStates.clear();
}

BaseDataObject *ScenarioModuleObject::toDataObject() {
    /** TODO **/
    ObjectDataObject *ans = new ObjectDataObject();
    return ans;
}

ScenarioModuleObject *ScenarioModuleObjectFactory::create(string filePath, DocElement *ele, Controller *controller) {
    if (ele == NULL) {
        return NULL;
    }
    if (ele->type != DOC_OBJECT) {
        Error::addError(
                new FieldInvalidError(filePath, ele->line, ele->col, "x-scenario.modules",
                                      ele->type, DOC_OBJECT)
        );
        return NULL;
    }
    DocObjectElement *objEle = (DocObjectElement*)ele;

    ScenarioModuleObject *res = new ScenarioModuleObject();

    /** name (required) **/
    DocElement *nameEle = objEle->get("name");
    if ((nameEle == NULL) || (nameEle->type != DOC_SCALAR)) {
        Error::addError(
                new FieldMissError(filePath, objEle->line, objEle->col, "x-scenario.modules.name")
        );
        delete res;
        return NULL;
    }
    res->name = ((DocScalarElement*)nameEle)->getValue();

    /** type (optional) **/
    

    /** TODO **/
    return NULL;
}
