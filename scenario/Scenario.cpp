//
// Created by lly on 22/09/2017.
//

#include "Scenario.h"
#include "../error/DuplicateScenarioElementError.h"
#include "../error/ScenarioInitialStateError.h"
#include "../error/ScenarioStartModuleError.h"

Scenario::Scenario() {
    this->info = NULL;
    this->states.clear();
    this->initialState = "";
    this->sets.clear();
    this->modules.clear();
    this->startModule = "";
}

Scenario::~Scenario() {
    if (info != NULL) delete info;
    for (map<string, ScenarioStateObject*>::iterator ite = this->states.begin();
            ite != this->states.end();
            ++ite) {
        delete ite->second;
    }
    this->states.clear();
    for (map<string, ScenarioSetObject*>::iterator ite = this->sets.begin();
            ite != this->sets.end();
            ++ite) {
        delete ite->second;
    }
    this->sets.clear();
    for (map<string, ScenarioModuleObject*>::iterator ite = this->modules.begin();
            ite !=this->modules.end();
            ++ite) {
        delete ite->second;
    }
    this->modules.clear();
}

BaseDataObject *Scenario::toDataObject() {
    ObjectDataObject *obj = new ObjectDataObject();
    (*obj)["info"] = info->toDataObject();

    SequenceDataObject *stateSeq = new SequenceDataObject();
    for (map<string, ScenarioStateObject*>::iterator ite = this->states.begin();
            ite != this->states.end(); ++ite) {
        stateSeq->push(ite->second->toDataObject());
    }
    (*obj)["states"] = stateSeq;

    if (this->sets.size() > 0) {
        SequenceDataObject *setSeq = new SequenceDataObject();
        for (map<string, ScenarioSetObject *>::iterator ite = this->sets.begin();
             ite != this->sets.end(); ++ite) {
            setSeq->push(ite->second->toDataObject());
        }
        (*obj)["sets"] = setSeq;
    }

    if (this->modules.size() > 0) {
        SequenceDataObject *modulesSeq = new SequenceDataObject();
        for (map<string, ScenarioModuleObject*>::iterator ite = this->modules.begin();
             ite != this->modules.end(); ++ite) {
            modulesSeq->push(ite->second->toDataObject());
        }
        (*obj)["modules"] = modulesSeq;
    }

    if (this->connections.size() > 0) {
        SequenceDataObject *connSeq = new SequenceDataObject();
        for (vector<ScenarioConnectionObject*>::iterator ite = this->connections.begin();
                ite != this->connections.end(); ++ite) {
            connSeq->push((*ite)->toDataObject());
        }
        (*obj)["connections"] = connSeq;
    }

    return obj;
}

Scenario *ScenarioFactory::create(string filePath, DocObjectElement *ele, Controller *controller) {
    Scenario *res = new Scenario();
    /** Info (required) **/
    DocElement *infoEle = ele->get("info");
    if (infoEle == NULL) {
        Error::addError(new FieldMissError(filePath, ele->line, ele->col, "x-scenario.info"));
        delete res;
        return NULL;
    }
    ScenarioInfoObject *info = ScenarioInfoObjectFactory::create(filePath, infoEle);
    if (info == NULL) {
        delete res;
        return NULL;
    } else
        res->info = info;

    /** States (required) **/
    DocElement *priStatesEle = ele->get("states");
    if (priStatesEle == NULL) {
        Error::addError(new FieldMissError(filePath, ele->line, ele->col, "x-scenario.states"));
        delete res;
        return NULL;
    }
    if (priStatesEle->type != DOC_SEQUENCE) {
        Error::addError(new FieldInvalidError(filePath, priStatesEle->line, priStatesEle->col, "x-scenario.states", priStatesEle->type, DOC_SEQUENCE));
        delete res;
        return NULL;
    }
    DocSequenceElement *statesEle = (DocSequenceElement*)priStatesEle;
    int statesLen = statesEle->getLength();
    for (int i=0; i<statesLen; ++i) {
        DocElement *nowStateEle = statesEle->get(i);
        ScenarioStateObject *nowState = ScenarioStateObjectFactory::create(filePath, nowStateEle);
        if (nowState == NULL) {
            delete res;
            return NULL;
        } else {
            if (res->states.count(nowState->name) > 0) {
                Error::addError(new DuplicateScenarioElementError(filePath, nowStateEle->line, nowStateEle->col, "x-scenario.states.name"));
                delete res;
                return NULL;
            }
            res->states[nowState->name] = nowState;
        }
    }
    /** find the only initial state */
    int initialStateCnt = 0;
    for (map<string, ScenarioStateObject*>::iterator ite = res->states.begin(); ite != res->states.end();
            ++ite) {
        ScenarioStateObject *now = ite->second;
        if (now->isInitial) {
            res->initialState = now->name;
            ++initialStateCnt;
        }
    }
    if (initialStateCnt != 1) {
        Error::addError(new ScenarioInitialStateError(filePath, ele->line, ele->col));
        delete res;
        return NULL;
    }

    /** sets (optional) **/
    DocElement *priSetsEle = ele->get("sets");
    if (priSetsEle != NULL) {
        if (priSetsEle->type != DOC_SEQUENCE) {
            Error::addError(new FieldInvalidError(filePath, priSetsEle->line, priSetsEle->col, "x-scenario.sets", priSetsEle->type, DOC_SEQUENCE));
            delete res;
            return NULL;
        }
        DocSequenceElement *setsEle = (DocSequenceElement*)priSetsEle;
        int setsLen = setsEle->getLength();
        for (int i=0; i<setsLen; ++i) {
            DocElement *nowSetEle = setsEle->get(i);
            ScenarioSetObject *nowSet = ScenarioSetObjectFactory::create(filePath, nowSetEle);
            if (nowSet == NULL) {
                delete res;
                return NULL;
            } else {
                if (res->sets.count(nowSet->name) > 0) {
                    Error::addError(new DuplicateScenarioElementError(filePath, nowSetEle->line, nowSetEle->col, "x-scenario.sets.name"));
                    delete res;
                    return NULL;
                }
                res->sets[nowSet->name] = nowSet;
            }
        }
    }

    /** modules (optional) **/
    DocElement *priModulesEle = ele->get("modules");
    if (priModulesEle != NULL) {
        if (priModulesEle->type != DOC_SEQUENCE) {
            Error::addError(new FieldInvalidError(filePath, priModulesEle->line, priModulesEle->col, "x-scenario.modules", priModulesEle->type, DOC_SEQUENCE));
            delete res;
            return NULL;
        }
        DocSequenceElement *modulesEle = (DocSequenceElement*)priModulesEle;
        int modulesLen = modulesEle->getLength();
        for (int i=0; i<modulesLen; ++i) {
            DocElement *nowModuleEle = modulesEle->get(i);
            ScenarioModuleObject *nowModule = ScenarioModuleObjectFactory::create(filePath, nowModuleEle, controller);
            if (nowModule == NULL) {
                delete res;
                return NULL;
            } else {
                if (res->modules.count(nowModule->name) > 0) {
                    Error::addError(new DuplicateScenarioElementError(filePath, nowModuleEle->line, nowModuleEle->col, "x-scenario.modules.name"));
                    delete res;
                    return NULL;
                }
                res->modules[nowModule->name] = nowModule;
            }
        }
    }

    /** check legality of set names in module definitions **/
    for (map<string, ScenarioModuleObject*>::iterator ite = res->modules.begin();
            ite != res->modules.end();
            ++ite) {
        ScenarioModuleObject *now = ite->second;
        bool legal = true;
        string field = "";
        string setName = "";
        /** modules.inputs **/
        for (vector<ModuleInputConstraintObject*>::iterator iite = now->inputs.begin();
                iite != now->inputs.end();
                ++iite) {
            ModuleInputConstraintObject *noww = *iite;
            if (noww->type == FROMSET_INPUTCONSTRAINT)
                if (res->sets.count(noww->setName) == 0)
                    legal = false, field = "x-scenario.modules.inputs.setName", setName = noww->setName;
        }
        /** modules.checkpoint **/
        for (vector<ModuleCheckpointObject*>::iterator iite = now->checkpoints.begin();
                iite != now->checkpoints.end();
                ++iite) {
            ModuleCheckpointObject *noww = *iite;
            if ((noww->type == INSET_CHECKPOINT) || (noww->type == OUTSET_CHECKPOINT))
                if (res->sets.count(noww->setName) == 0)
                    legal = false, field = "x-scenario.modules.checkpoint.setName", setName = noww->setName;
        }
        /** modules.setEffects **/
        for (vector<ModuleSetEffectObject*>::iterator iite = now->setEffects.begin();
                iite != now->setEffects.end();
                ++iite) {
            ModuleSetEffectObject *noww = *iite;
            if (res->sets.count(noww->setName) == 0)
                legal = false, field = "x-scenario.modules.setEffects.set", setName = noww->setName;
        }
        if (!legal) {
            Error::addError(new InvalidSetError(filePath, ele->line, ele->col, field, setName));
            delete res;
            return NULL;
        }
    }

    /** check state quoted in module.transState definitions **/
    for (map<string, ScenarioModuleObject*>::iterator ite = res->modules.begin();
            ite != res->modules.end();
            ++ite) {
        ScenarioModuleObject *now = ite->second;
        for (vector<ModuleTransStateObject*>::iterator iite = now->transStates.begin();
                iite != now->transStates.end();
                ++iite) {
            ModuleTransStateObject *noww = *iite;
            if (noww->preState != "default")
                if (res->states.count(noww->preState) == 0) {
                    Error::addError(new InvalidStateError(filePath, ele->line, ele->col,
                                                          "x-scenario.modules.transState.preState", noww->preState));
                    delete res;
                    return NULL;
                }
            if (res->states.count(noww->nexState) == 0) {
                Error::addError(new InvalidStateError(filePath, ele->line, ele->col,
                                                      "x-scenario.modules.transState.nexState", noww->nexState));
                delete res;
                return NULL;
            }
        }
    }

    /** Check Uniqueness of Start Module **/
    int startModuleCnt = 0;
    for (map<string, ScenarioModuleObject*>::iterator ite = res->modules.begin();
            ite != res->modules.end();
            ++ite) {
        if (ite->second->type == SCENARIO_START_MODULE) {
            res->startModule = ite->second->name;
            ++startModuleCnt;
        }
    }
    if (startModuleCnt != 1) {
        Error::addError(new ScenarioStartModuleError(filePath, ele->line, ele->col));
        delete res;
        return NULL;
    }

    /** connections (optional) **/
    DocElement *connEle = ele->get("connections");
    if (connEle != NULL) {
        if (connEle->type != DOC_SEQUENCE) {
            Error::addError(
                    new FieldInvalidError(filePath, connEle->line, connEle->col, "x-scenario.connections",
                                          connEle->type, DOC_SEQUENCE)
            );
            delete res;
            return NULL;
        } else {
            DocSequenceElement *seqConn = (DocSequenceElement*)connEle;
            int len = seqConn->getLength();
            for (int i=0; i<len; ++i) {
                DocElement *nowEle = seqConn->get(i);
                ScenarioConnectionObject *nowObj = ScenarioConnectionObjectFactory::create(filePath, nowEle);
                if (nowObj == NULL) {
                    delete res;
                    return NULL;
                }
                /** check module name of 'from' and 'to' **/
                if (res->modules.count(nowObj->from) == 0) {
                    Error::addError(new InvalidModuleError(filePath, nowEle->line, nowEle->col,
                                                           "x-scenario.connections.from", nowObj->from));
                    delete res;
                    return NULL;
                }
                if (res->modules.count(nowObj->to) == 0) {
                    Error::addError(new InvalidModuleError(filePath, nowEle->line, nowEle->col,
                                                           "x-scenario.connections.to", nowObj->to));
                    delete res;
                    return NULL;
                }

                /** check 'including' and 'excluding' **/
                if (res->modules[nowObj->from]->type == SCENARIO_NORMAL_MODULE) {
                    APIObject *api = res->modules[nowObj->from]->api;
                    for (vector<string>::iterator iite = nowObj->excluding.begin(); iite != nowObj->excluding.end(); ++iite) {
                        if (api->responses.count(*iite) + api->responseExtensions.count(*iite) == 0) {
                            Error::addError(
                                    new InvalidResponseTypeError(filePath, nowEle->line, nowEle->col, "x-scenario.modules.connections.excluding",
                                                                 *iite)
                            );
                            delete res;
                            return NULL;
                        }
                    }
                    for (vector<string>::iterator iite = nowObj->including.begin(); iite != nowObj->including.end(); ++iite) {
                        if (api->responses.count(*iite) + api->responseExtensions.count(*iite) == 0) {
                            Error::addError(
                                    new InvalidResponseTypeError(filePath, nowEle->line, nowEle->col, "x-scenario.modules.connections.including",
                                                                 *iite)
                            );
                            delete res;
                            return NULL;
                        }
                    }
                }

                res->connections.push_back(nowObj);
            }
        }
    }

    return res;
}