//
// Created by lly on 03/10/2017.
//

#include "ConfigObject.h"

ConfigObject::ConfigObject() {
    this->scenario = "";
    this->setInitial.clear();
    this->policy = INVALID_TRAVERSE_POLICY;
    this->maxCase = 0;
    this->vitalModules.clear();
    this->maxLength = 32768;
    this->timeout = 2000;
    this->isAli = false;
    this->aliKeySecret = "";
}

ConfigObject::~ConfigObject() {
    this->setInitial.clear();
    this->vitalModules.clear();
}


BaseDataObject *ConfigObject::toDataObject() {
    ObjectDataObject *obj = new ObjectDataObject();
    (*obj)["scenario"] = new StringDataObject(this->scenario);
    if (this->setInitial.size() > 0) {
        ObjectDataObject *setIniObj = new ObjectDataObject();
        for (map<string, vector<DocElement*>>::iterator ite = this->setInitial.begin();
                ite != this->setInitial.end();
                ++ite) {
            SequenceDataObject *seqObj = new SequenceDataObject();
            for (vector<DocElement*>::iterator iite = ite->second.begin();
                    iite != ite->second.end();
                    ++iite)
                seqObj->push(DataObjectAdapter::defaultFromDocElement(*iite));
            (*setIniObj)[ite->first] = seqObj;
        }
        (*obj)["setInitial"] = setIniObj;
    }

    string policyStr = "";
    switch (this->policy) {
        case RANDOM_TRAVERSE_POLICY:
            policyStr = "random";
            break;
        case BASIC_EXPLORE_TRAVERSE_POLICY:
            policyStr = "basicExplore";
            break;
        case ADVANCE_EXPLORE_TRAVERSE_POLICY:
            policyStr = "advanceExplore";
            break;
        default:
            ;
    }
    (*obj)["policy"] = new StringDataObject(policyStr);

    (*obj)["maxCase"] = new IntegerDataObject(this->maxCase);

    if (this->vitalModules.size() > 0) {
        SequenceDataObject *seq = new SequenceDataObject();
        for (vector<string>::iterator ite = vitalModules.begin();
                ite != vitalModules.end();
                ++ite) {
            seq->push(new StringDataObject(*ite));
        }
        (*obj)["vitalModules"] = seq;
    }

    (*obj)["maxLength"] = new IntegerDataObject(this->maxLength);
    (*obj)["timeout"] = new IntegerDataObject(this->timeout);

    if (this->isAli) {
        (*obj)["x-custom"] = new StringDataObject("ali");
        (*obj)["x-keySecret"] = new StringDataObject(this->aliKeySecret);
    }

    return obj;
}

bool ConfigObject::work(Controller *controller, Scenarios *scenarios) {
    if ((controller == NULL) || (scenarios == NULL))
        return false;
    int testConfigCnt = 0;
    for (map<string, DocElement *>::iterator ite = DocElement::docs.begin(); ite != DocElement::docs.end(); ++ite) {
        string filePath = ite->first;
        DocElement *priDocRoot = ite->second;
        if (priDocRoot->type == DOC_OBJECT) {
            DocObjectElement *docRoot = (DocObjectElement*)priDocRoot;
            if (docRoot->get("x-config") != NULL) {
                ++testConfigCnt;
                if (testConfigCnt == 1) {
                    DocElement *rootEle = docRoot->get("x-config");
                    if (rootEle->type != DOC_OBJECT) {
                        Error::addError(new FieldInvalidError(filePath, rootEle->line, rootEle->col, "x-config",
                                                              rootEle->type, DOC_OBJECT));
                        return false;
                    } else {
                        DocObjectElement *rootObjEle = (DocObjectElement*)rootEle;

                        /** scenario (required) **/
                        DocElement *scenarioEle = rootObjEle->get("scenario");
                        if (scenarioEle == NULL) {
                            Error::addError(new FieldMissError(filePath, rootEle->line, rootEle->col, "x-config.scenario"));
                            return false;
                        } else {
                            pair<string, bool> res = DocElementHelper::parseToString(scenarioEle);
                            if (!res.second) {
                                Error::addError(new FieldInvalidError(filePath, scenarioEle->line, scenarioEle->col, "x-config.scenario",
                                                                      scenarioEle->type, DOC_SCALAR));
                                return false;
                            } else {
                                this->scenario = res.first;
                            }
                        }

                        /** setInitial (optional) **/
                        DocElement *setIniEle = rootObjEle->get("setInitial");
                        if (setIniEle != NULL) {
                            if (setIniEle->type != DOC_OBJECT) {
                                Error::addError(new FieldInvalidError(filePath, setIniEle->line, setIniEle->col, "x-config.setInitial",
                                                                      setIniEle->type, DOC_OBJECT));
                                return false;
                            } else {
                                DocObjectElement *objSetIniEle = (DocObjectElement*)setIniEle;
                                map<string, DocElement*> *objSetIniMap = objSetIniEle->getMemberMap();
                                for (map<string, DocElement*>::iterator iite = objSetIniMap->begin();
                                     iite != objSetIniMap->end();
                                     ++iite) {
                                    if (iite->second->type != DOC_SEQUENCE) {
                                        Error::addError(new FieldInvalidError(filePath, iite->second->line, iite->second->col, "x-config.setInitial.value",
                                                                              iite->second->type, DOC_SEQUENCE));
                                        return false;
                                    } else {
                                        DocSequenceElement *seqEle = (DocSequenceElement*)iite->second;
                                        int len = seqEle->getLength();
                                        this->setInitial[iite->first].clear();
                                        for (int i=0; i<len; ++i) {
                                            this->setInitial[iite->first].push_back(seqEle->get(i));
                                        }
                                    }
                                }
                            }
                        }

                        /** policy (required) **/
                        DocElement *policyEle = rootObjEle->get("policy");
                        if (policyEle == NULL) {
                            Error::addError(new FieldMissError(filePath, rootEle->line, rootEle->col, "x-config.policy"));
                            return false;
                        } else {
                            pair<string, bool> res = DocElementHelper::parseToString(policyEle);
                            if (!res.second) {
                                Error::addError(new FieldInvalidError(filePath, policyEle->line, policyEle->col, "x-config.policy",
                                                                      policyEle->type, DOC_SCALAR));
                                return false;
                            } else {
                                string &str = res.first;
                                if (str == "random")
                                    this->policy = RANDOM_TRAVERSE_POLICY;
                                else if (str == "basicExplore")
                                    this->policy = BASIC_EXPLORE_TRAVERSE_POLICY;
                                else if (str == "advanceExplore")
                                    this->policy = ADVANCE_EXPLORE_TRAVERSE_POLICY;
                                else {
                                    Error::addError(new FieldIllegalError(filePath, policyEle->line, policyEle->col, "x-config.policy"));
                                    return false;
                                }
                            }
                        }

                        /** maxCase (required) **/
                        DocElement *maxCaseEle = rootObjEle->get("maxCase");
                        if (maxCaseEle == NULL) {
                            Error::addError(new FieldMissError(filePath, rootEle->line, rootEle->col, "x-config.maxCase"));
                            return false;
                        } else {
                            pair<long long, bool> res = DocElementHelper::parseToInt(maxCaseEle);
                            if (!res.second) {
                                Error::addError(new FieldIllegalError(filePath, maxCaseEle->line, maxCaseEle->col, "x-config.maxCase"));
                                return false;
                            } else {
                                this->maxCase = (int)res.first;
                            }
                        }

                        /** vitalModules (optional) **/
                        DocElement *vitalModulesEle = rootObjEle->get("vitalModules");
                        if (vitalModulesEle != NULL) {
                            if (vitalModulesEle->type != DOC_SEQUENCE) {
                                Error::addError(new FieldInvalidError(filePath, vitalModulesEle->line, vitalModulesEle->col, "x-config.vitalModules",
                                                                      vitalModulesEle->type, DOC_SEQUENCE));
                                return false;
                            } else {
                                DocSequenceElement *seqEle = (DocSequenceElement*)vitalModulesEle;
                                int len = seqEle->getLength();
                                for (int i=0; i<len; ++i) {
                                    DocElement *nowEle = seqEle->get(i);
                                    pair<string, bool> res = DocElementHelper::parseToString(nowEle);
                                    if (!res.second) {
                                        Error::addError(new FieldInvalidError(filePath, nowEle->line, nowEle->col, "x-config.vitalModules",
                                                                              nowEle->type, DOC_SCALAR));
                                        return false;
                                    } else {
                                        this->vitalModules.push_back(res.first);
                                    }
                                }
                            }
                        }

                        /** maxLength (optional) **/
                        DocElement *maxLenEle = rootObjEle->get("maxLength");
                        if (maxLenEle != NULL) {
                            pair<long long, bool> parseRes = DocElementHelper::parseToInt(maxLenEle);
                            if (!parseRes.second) {
                                Error::addError(new FieldIllegalError(filePath, maxLenEle->line, maxLenEle->col, "x-config.maxLength"));
                                return false;
                            } else {
                                this->maxLength = (int)parseRes.first;
                            }
                        }

                        /** timeout (optional) **/
                        DocElement *timeoutEle = rootObjEle->get("timeout");
                        if (timeoutEle != NULL) {
                            pair<long long, bool> parseRes = DocElementHelper::parseToInt(timeoutEle);
                            if (!parseRes.second) {
                                Error::addError(new FieldIllegalError(filePath, timeoutEle->line, timeoutEle->col, "x-config.timeout"));
                                return false;
                            } else {
                                this->timeout = (int)parseRes.first;
                            }
                        }

                        /** beginning of customizations **/
                        {
                            /** ALI **/
                            DocElement *customEle = rootObjEle->get("x-custom");
                            if (customEle) {
                                pair<string, bool> parseRes = DocElementHelper::parseToString(customEle);
                                if (!parseRes.second) {
                                    Error::addError(new FieldInvalidError(filePath, customEle->line, customEle->col, "x-config.x-custom",
                                                                          customEle->type, DOC_SCALAR));
                                    return false;
                                } else {
                                    if (parseRes.first == "ali") {
                                        this->isAli = true;
                                        DocElement *secretEle = rootObjEle->get("x-keySecret");
                                        if (secretEle == NULL) {
                                            Error::addError(new FieldMissError(filePath, rootEle->line, rootEle->col, "x-config.x-keySecret"));
                                            return false;
                                        } else {
                                            pair<string, bool> keyParseRes = DocElementHelper::parseToString(secretEle);
                                            if (!keyParseRes.second) {
                                                Error::addError(new FieldInvalidError(filePath, secretEle->line, secretEle->col, "x-config.x-keySecret",
                                                                                      secretEle->type, DOC_SCALAR));
                                                return false;
                                            } else {
                                                this->aliKeySecret = keyParseRes.first;
                                            }
                                        }
                                    }
                                }
                            }

                        }
                        /** ending of customizations **/
                    }
                }
            }
        }
    }
    if (testConfigCnt == 0) {
        Error::addError(new FieldMissError("", 1, 1, "x-config"));
        return false;
    }
    if (testConfigCnt > 1) {
        Error::addError(new DuplicateTestConfigError());
        return false;
    }
    /** check legality of scenario **/
    if (scenarios->getScenarioByTitle(this->scenario) == NULL) {
        Error::addError(new InvalidScenarioError("", 1, 1, "x-config.scenario", this->scenario));
        return false;
    }

    /** check legality of vitalModules **/
    for (vector<string>::iterator ite = this->vitalModules.begin();
            ite != this->vitalModules.end();
            ++ite) {
        if (scenarios->getScenarioByTitle(this->scenario)->modules.count(*ite) == 0) {
            Error::addError(new InvalidModuleError("", 1, 1, "x-config.vitalModules", *ite));
            return false;
        }
    }


    return true;
}