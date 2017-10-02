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
    ObjectDataObject *ans = new ObjectDataObject();
    (*ans)["name"] = new StringDataObject(this->name);

    string typeStr = "";
    switch (this->type) {
        case SCENARIO_START_MODULE:
            typeStr = "start";
            break;
        case SCENARIO_NORMAL_MODULE:
            typeStr = "normal";
            break;
        case SCENARIO_END_MODULE:
            typeStr = "end";
            break;
        case SCENARIO_EMPTY_MODULE:
            typeStr = "empty";
            break;
        default:
            ;
    }
    (*ans)["type"] = new StringDataObject(typeStr);

    if (this->type == SCENARIO_NORMAL_MODULE) {
        (*ans)["APIType"] = new StringDataObject(this->apiTypeStr);

        (*ans)["repeat"] = this->repeat->toDataObject();

        if (this->inputs.size() > 0) {
            ObjectDataObject *inputsObj = new ObjectDataObject();
            for (vector<ModuleInputConstraintObject*>::iterator ite = this->inputs.begin();
                    ite != this->inputs.end();
                    ++ite) {
                (*inputsObj)[(*ite)->getKeyName()] = (*ite)->toDataObject();
            }
            (*ans)["inputs"] = inputsObj;
        }

        if (this->checkpoints.size() > 0) {
            ObjectDataObject *checkpointObj = new ObjectDataObject();
            for (vector<ModuleCheckpointObject*>::iterator ite = this->checkpoints.begin();
                    ite != this->checkpoints.end();
                    ++ite) {
                (*checkpointObj)[(*ite)->getKeyName()] = (*ite)->toDataObject();
            }
            (*ans)["checkpoint"] = checkpointObj;
        }

        if (this->setEffects.size() > 0) {
            SequenceDataObject *setEffSeq = new SequenceDataObject();
            for (vector<ModuleSetEffectObject*>::iterator ite = this->setEffects.begin();
                    ite != this->setEffects.end();
                    ++ite) {
                setEffSeq->push((*ite)->toDataObject());
            }
            (*ans)["setEffects"] = setEffSeq;
        }
    }

    if (this->transStates.size() > 0) {
        SequenceDataObject *transStatesSeq = new SequenceDataObject();
        for (vector<ModuleTransStateObject*>::iterator ite = this->transStates.begin();
             ite != this->transStates.end();
             ++ite) {
            transStatesSeq->push((*ite)->toDataObject());
        }
        (*ans)["transState"] = transStatesSeq;
    }

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
    DocElement *typeEle = objEle->get("type");
    if (typeEle != NULL) {
        if (typeEle->type != DOC_SCALAR) {
            Error::addError(
                    new FieldInvalidError(filePath, typeEle->line, typeEle->col, "x-scenario.modules.type",
                                          typeEle->type, DOC_SCALAR)
            );
            delete res;
            return NULL;
        } else {
            string s_type = ((DocScalarElement*)typeEle)->getValue();
            if (s_type == "start") {
                res->type = SCENARIO_START_MODULE;
            } else
                if (s_type == "end") {
                    res->type = SCENARIO_END_MODULE;
                } else
                    if (s_type == "normal") {
                        res->type = SCENARIO_NORMAL_MODULE;
                    } else
                        if (s_type == "empty") {
                            res->type = SCENARIO_EMPTY_MODULE;
                        } else {
                            Error::addError(
                                    new  FieldIllegalError(filePath, typeEle->line, typeEle->col, "x-scenario.modules.type")
                            );
                            delete res;
                            return NULL;
                        }
        }
    }

    if (res->type == SCENARIO_NORMAL_MODULE) {
        /** APIType (required) **/
        DocElement *apiTypeEle = objEle->get("APIType");
        if ((apiTypeEle == NULL) || (apiTypeEle->type != DOC_SCALAR)) {
            Error::addError(
                    new FieldMissError(filePath, objEle->line, objEle->col, "x-scenario.modules.APIType")
            );
            delete res;
            return NULL;
        } else {
            string apiTypeStr = ((DocScalarElement*)apiTypeEle)->getValue();
            res->apiTypeStr = apiTypeStr;
            DocElement *apiEle = RefExpand::findByPath(filePath, apiTypeStr, apiTypeEle->line, apiTypeEle->col);
            if (apiEle == NULL) {
                Error::addError(
                        new FieldIllegalError(filePath, apiTypeEle->line, apiTypeEle->col, "x-scenario.modules.APIType")
                );
                delete res;
                return NULL;
            } else {
                APIObject *apiObj = controller->paths->getObjectByElement(apiEle);
                if (apiObj == NULL) {
                    Error::addError(
                            new InvalidAPIReferenceError(filePath, apiTypeEle->line, apiTypeEle->col, "x-scenario.modules.APIType")
                    );
                    delete res;
                    return NULL;
                } else {
                    res->api = apiObj;
                }
            }
        }

        /** repeat (optional) **/
        DocElement *repeatEle = objEle->get("repeat");
        if (repeatEle != NULL) {
            ModuleRepeatObject *o = ModuleRepeatObjectFactory::create(filePath, repeatEle);
            if (o == NULL) {
                delete res;
                return NULL;
            } else {
                /** including & excluding state check **/
                for (vector<string>::iterator ite = o->excluding.begin(); ite != o->excluding.end(); ++ite) {
                    if (res->api->responses.count(*ite) + res->api->responseExtensions.count(*ite) == 0) {
                        Error::addError(
                                new InvalidResponseTypeError(filePath, repeatEle->line, repeatEle->col, "x-scenario.modules.repeat.excluding",
                                *ite)
                        );
                        delete res;
                        return NULL;
                    }
                }
                for (vector<string>::iterator ite = o->including.begin(); ite != o->including.end(); ++ite) {
                    if (res->api->responses.count(*ite) + res->api->responseExtensions.count(*ite) == 0) {
                        Error::addError(
                                new InvalidResponseTypeError(filePath, repeatEle->line, repeatEle->col, "x-scenario.modules.repeat.including",
                                *ite)
                        );
                        delete res;
                        return NULL;
                    }
                }
            }
        } else
            res->repeat = ModuleRepeatObjectFactory::createDefault();

        /** inputs (optional) **/
        DocElement *inputsEle = objEle->get("inputs");
        if (inputsEle != NULL) {
            if (inputsEle->type != DOC_OBJECT) {
                Error::addError(
                        new FieldInvalidError(filePath, inputsEle->line, inputsEle->col, "x-scenarip.modules.inputs",
                        inputsEle->type, DOC_OBJECT)
                );
                delete res;
                return NULL;
            } else {
                DocObjectElement *objInputsEle = (DocObjectElement*)inputsEle;
                map<string, DocElement*> *objInputsMap = objInputsEle->getMemberMap();
                for (map<string, DocElement*>::iterator ite = objInputsMap->begin();
                        ite != objInputsMap->end();
                        ++ite) {
                    ModuleInputConstraintObject *item = ModuleInputConstraintObjectFactory::create(filePath, ite->first, ite->second, controller);
                    if (item == NULL) {
                        delete res;
                        return NULL;
                    }

                    /** key validation **/
                    /** the key must be actually pointed to one of request parameters or its subfield **/
                    {
                        vector<string> &fieldVec = item->fieldVec;
                        string &paramName = fieldVec[1];
                        bool legal = true;
                        if (res->api->parameters.count(paramName) == 0) {
                            legal = false;
                        } else {
                            DataSchemaObject *schema = res->api->parameters[paramName]->schema;
                            for (int i=2; i<fieldVec.size(); ++i) {
                                if (schema->type == DataSchemaObject::TYPE_OBJECT) {
                                    ObjectSchema *objSchema = (ObjectSchema*)schema;
                                    if (objSchema->properties.count(fieldVec[i]) == 0) {
                                        legal = false;
                                        break;
                                    } else {
                                        schema = objSchema->properties[fieldVec[i]];
                                    }
                                }
                                else if (schema->type == DataSchemaObject::TYPE_ARRAY) {
                                    try {
                                        stoi(fieldVec[i]);
                                    } catch (std::invalid_argument) {
                                        legal = false;
                                        break;
                                    } catch (std::out_of_range) {
                                        legal = false;
                                        break;
                                    }
                                    ArraySchema *arrSchema = (ArraySchema*)schema;
                                    schema = arrSchema->items;
                                }
                                else {
                                    legal = false;
                                    break;
                                }
                            }
                        }
                        if (!legal) {
                            Error::addError(new FieldIllegalError(filePath, ite->second->line, ite->second->col, "x-scenario.modules.inputs.key"));
                            delete res;
                            return NULL;
                        }
                    }

                    res->inputs.push_back(item);
                }
            }
        }

        /** checkpoint (optional) **/
        DocElement *checkpEle = objEle->get("checkpoint");
        if (checkpEle != NULL) {
            if (checkpEle->type != DOC_OBJECT) {
                Error::addError(new FieldInvalidError(filePath, checkpEle->line, checkpEle->col, "x-scenario.modules.checkpoint",
                                                      checkpEle->type, DOC_OBJECT));
                delete res;
                return NULL;
            } else {
                DocObjectElement *objCheckpEle = (DocObjectElement*)checkpEle;
                map<string, DocElement*> *objCheckpMap = objCheckpEle->getMemberMap();
                for (map<string, DocElement*>::iterator ite = objCheckpMap->begin();
                        ite != objCheckpMap->end();
                        ++ite) {
                    ModuleCheckpointObject *item = ModuleCheckpointObjectFactory::create(filePath, ite->first, ite->second);
                    if (item == NULL) {
                        delete res;
                        return NULL;
                    }

                    /** including & excluding state check **/
                    for (vector<string>::iterator iite = item->excluding.begin(); iite != item->excluding.end(); ++iite) {
                        if (res->api->responses.count(*iite) + res->api->responseExtensions.count(*iite) == 0) {
                            Error::addError(
                                    new InvalidResponseTypeError(filePath, ite->second->line, ite->second->col, "x-scenario.modules.checkpoint.excluding",
                                                                 *iite)
                            );
                            delete res;
                            return NULL;
                        }
                    }
                    for (vector<string>::iterator iite = item->including.begin(); iite != item->including.end(); ++iite) {
                        if (res->api->responses.count(*iite) + res->api->responseExtensions.count(*iite) == 0) {
                            Error::addError(
                                    new InvalidResponseTypeError(filePath, ite->second->line, ite->second->col, "x-scenario.modules.checkpoint.including",
                                                                 *iite)
                            );
                            delete res;
                            return NULL;
                        }
                    }

                    res->checkpoints.push_back(item);
                }
            }
        }

        /** setEffect (optional) **/
        DocElement *setEffEle = objEle->get("setEffects");
        if (setEffEle != NULL) {
            if (setEffEle->type != DOC_SEQUENCE) {
                Error::addError(new FieldInvalidError(filePath, setEffEle->line, setEffEle->col, "x-scenario.modules.setEffects",
                                                      setEffEle->type, DOC_SEQUENCE));
                delete res;
                return NULL;
            } else {
                DocSequenceElement *seqSetEffEle = (DocSequenceElement*)setEffEle;
                int len = seqSetEffEle->getLength();
                for (int i=0; i<len; ++i) {
                    DocElement *nowEle = seqSetEffEle->get(i);
                    ModuleSetEffectObject *item = ModuleSetEffectObjectFactory::create(filePath, nowEle, res->api);
                    if (item == NULL) {
                        delete res;
                        return NULL;
                    }

                    /** including & excluding state check **/
                    for (vector<string>::iterator iite = item->excluding.begin(); iite != item->excluding.end(); ++iite) {
                        if (res->api->responses.count(*iite) + res->api->responseExtensions.count(*iite) == 0) {
                            Error::addError(
                                    new InvalidResponseTypeError(filePath, nowEle->line, nowEle->col, "x-scenario.modules.setEffects.excluding",
                                                                 *iite)
                            );
                            delete res;
                            return NULL;
                        }
                    }
                    for (vector<string>::iterator iite = item->including.begin(); iite != item->including.end(); ++iite) {
                        if (res->api->responses.count(*iite) + res->api->responseExtensions.count(*iite) == 0) {
                            Error::addError(
                                    new InvalidResponseTypeError(filePath, nowEle->line, nowEle->col, "x-scenario.modules.setEffects.including",
                                                                 *iite)
                            );
                            delete res;
                            return NULL;
                        }
                    }

                    res->setEffects.push_back(item);
                }
            }
        }
    }
    
    /** transState (optional) **/
    DocElement *transStateEle = objEle->get("transState");
    if (transStateEle != NULL) {
        if (transStateEle->type == DOC_SCALAR) {
            string value = ((DocScalarElement *) transStateEle)->getValue();
            res->transStates.push_back(ModuleTransStateObjectFactory::create(value));
        } else if (transStateEle->type == DOC_SEQUENCE) {
            DocSequenceElement *seqEle = (DocSequenceElement *) transStateEle;
            int len = seqEle->getLength();
            for (int i = 0; i < len; ++i) {
                DocElement *nowEle = seqEle->get(i);
                ModuleTransStateObject *nowObj = ModuleTransStateObjectFactory::create(filePath, nowEle);
                if (nowObj == NULL) {
                    delete res;
                    return NULL;
                }
                res->transStates.push_back(nowObj);
            }
        } else {
            Error::addError(
                    new FieldInvalidError(filePath, transStateEle->line, transStateEle->col, "x-scenario.modules.transState",
                    transStateEle->type, DOC_OBJECT)
            );
            delete res;
            return NULL;
        }
    }

    return res;
}
