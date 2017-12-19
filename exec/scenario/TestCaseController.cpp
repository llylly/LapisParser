//
// Created by lly on 08/10/2017.
//

#include "TestCaseController.h"
#include "../../schema/data_schema/IntegerSchema.h"

TestCaseController::TestCaseController(Scenario *scenario, ConfigObject *config, string host, string basePath, int verbose) {
    assert(scenario != NULL);
    assert(config != NULL);
    this->scenario = scenario;
    this->config = config;
    this->host = host;
    this->basePath = basePath;
    this->verbose = verbose;
}

TestCaseReport *TestCaseController::run() {
    TestCaseReport *ans = new TestCaseReport;
    ans->start();

    /** init **/
    {
        /* module */
        nowModule = scenario->startModule;

        /* state */
        nowState = scenario->initialState;

        /* responseType */
        nowResponseType = "";

        /* sets */
        for (map<string, ScenarioSetObject*>::iterator ite = scenario->sets.begin();
                ite != scenario->sets.end();
                ++ite) {
            ScenarioSetObject *setObj = ite->second;
            nowSets[ite->first] = vector<DocElement*>();
            TestCaseController::vecDocEleCpy(setObj->_default, nowSets[ite->first]);
        }

        /* nowRequest */
        nowRequest = NULL;

        moduleSeq.clear();
        moduleSeq.push_back(nowModule);
        nowRepeatTimes = 1;

        ans->addStep(this->packetNow());
    }
    /** main loop **/
    while (true) {
        if (verbose == 2)
            cerr << "  step #" << moduleSeq.size() << "  " << moduleSeq[moduleSeq.size()-1] << endl;
        if (ans->getTerminate() != NULL) {
            /** has been terminated for some reason, break **/
            break;
        }
        if (moduleSeq.size() >= config->maxLength) {
            /** length exceed **/
            ans->setTerminate(new LengthExceedTerminate());
            break;
        }
        /** select next module part **/

        map<string, double> nexModuleCandidates;
        nexModuleCandidates.clear();

        ScenarioModuleObject *nowModuleObj = scenario->modules[nowModule];
        bool needRepeat = false;
        if ((nowModuleObj->type == SCENARIO_NORMAL_MODULE) && (nowModuleObj->repeat))
            if (nowRepeatTimes < nowModuleObj->repeat->time) {
                needRepeat = true;
                bool find = false;
                if (nowModuleObj->repeat->including.size() > 0) {
                    for (vector<string>::iterator ite = nowModuleObj->repeat->including.begin();
                         ite != nowModuleObj->repeat->including.end();
                         ++ite)
                        if (*ite == nowResponseType) {
                            find = true;
                            break;
                        }
                    if (!find) needRepeat = false;
                }
                if (nowModuleObj->repeat->excluding.size() > 0) {
                    find = false;
                    for (vector<string>::iterator ite = nowModuleObj->repeat->excluding.begin();
                            ite != nowModuleObj->repeat->excluding.end();
                            ++ite)
                        if (*ite == nowResponseType) {
                            find = true;
                            break;
                        }
                    if (find) needRepeat = false;
                }
            }

        if (needRepeat) {
            nexModuleCandidates[nowModule] = 1.0;
        } else {
            for (vector<ScenarioConnectionObject *>::iterator ite = scenario->connections.begin();
                 ite != scenario->connections.end();
                 ++ite) {
                ScenarioConnectionObject *nowEdge = *ite;
                if (nowEdge->from == nowModule) {
                    bool legal = true;
                    if (nowEdge->including.size() > 0) {
                        bool find = false;
                        for (vector<string>::iterator iite = nowEdge->including.begin();
                             iite != nowEdge->including.end();
                             ++iite)
                            if (*iite == nowResponseType) {
                                find = true;
                                break;
                            }
                        if (!find) legal = false;
                    }
                    if (nowEdge->excluding.size() > 0) {
                        bool find = false;
                        for (vector<string>::iterator iite = nowEdge->excluding.begin();
                             iite != nowEdge->excluding.end();
                             ++iite)
                            if (*iite == nowResponseType) {
                                find = true;
                                break;
                            }
                        if (find) legal = false;
                    }
                    if (legal) {
                        if (nexModuleCandidates.count(nowEdge->to) == 0)
                            nexModuleCandidates[nowEdge->to] = nowEdge->weigh;
                        else
                            nexModuleCandidates[nowEdge->to] += nowEdge->weigh;
                    }
                }
            }
        }

        for (map<string, double>::iterator ite = nexModuleCandidates.begin();
             ite != nexModuleCandidates.end();
             ++ite)
            if (!checkConstraintsWith(ite->first))
                nexModuleCandidates.erase(ite);

        if (nexModuleCandidates.size() <= 0) {
            /** noway **/
            ans->setTerminate(new NowayTerminate(nowModule));
            break;
        }

        string nexModule = "";
        {
            // get nexModule
            double totWeigh = 0.0;
            for (map<string, double>::iterator ite = nexModuleCandidates.begin();
                    ite != nexModuleCandidates.end();
                    ++ite)
                totWeigh += ite->second;
            double rand = DataSchemaObject::randomReal();
            double tmp = 0.0;
            for (map<string, double>::iterator ite = nexModuleCandidates.begin();
                    ite != nexModuleCandidates.end();
                    ++ite) {
                tmp += ite->second;
                if (tmp >= rand * totWeigh) {
                    nexModule = ite->first;
                    break;
                }
            }
            assert(nexModule != "");
        }

        if (nexModule == nowModule) ++nowRepeatTimes; else nowRepeatTimes = 1;

        ScenarioModuleObject *nexModuleObj = scenario->modules[nexModule];
        string nexState = nowState;
        string nexResponseType = "";
        RequesterReport *nexRequesterReport = NULL;

        /** transState **/
        for (vector<ModuleTransStateObject*>::iterator ite = nowModuleObj->transStates.begin();
                ite != nowModuleObj->transStates.end();
                ++ite) {
            ModuleTransStateObject *transObj = *ite;
            if ((transObj->preState == "default") || (transObj->preState == nowState)) {
                bool legal = true;
                bool find = false;
                if (transObj->including.size() > 0) {
                    for (vector<string>::iterator iite = transObj->including.begin();
                            iite != transObj->including.end();
                            ++iite)
                        if (*iite == nowResponseType) {
                            find = true;
                            break;
                        }
                    if (!find) legal = false;
                }
                find = false;
                if (transObj->excluding.size() > 0) {
                    for (vector<string>::iterator iite = transObj->excluding.begin();
                            iite != transObj->excluding.end();
                            ++iite)
                        if (*iite == nowResponseType) {
                            find = true;
                            break;
                        }
                    if (find) legal = false;
                }
                if (legal) {
                    nexState = transObj->nexState;
                    break;
                }
            }
        }

        bool checkPointPass = true;
        bool setEffectPass = true;
        /** execution part for nex module **/
        if (nexModuleObj->type == SCENARIO_NORMAL_MODULE) {
            RequesterReport *requesterReport = new RequesterReport();
            map<string, string> *(*middleware_func)(map<string, string>*, void*) = NULL;
            if (config->isAli) {
                middleware_func = &(AliMiddleware::main);
            }
            ModuleRequester *requester = new ModuleRequester(this->host, this->basePath, requesterReport,
                                                             middleware_func, config->timeout);
            requester->init(nexModuleObj, &(this->nowSets));
            if (config->isAli) {
                requester->setUserP(&(config->aliKeySecret));
            }
            requester->work();

            nexResponseType = requesterReport->responseType;
            nexRequesterReport = requesterReport;

            if (requesterReport->err == NULL) {
                /** request success **/

                /** checkpoint inspection **/
                BaseDataObject *response = requesterReport->response;
                DataSchemaObject *schema = requesterReport->schema;
                for (vector<ModuleCheckpointObject*>::iterator ite = nexModuleObj->checkpoints.begin();
                        ite != nexModuleObj->checkpoints.end();
                        ++ite) {
                    ModuleCheckpointObject *cpNow = *ite;

                    bool legal = true;
                    bool find = false;
                    if (cpNow->including.size() > 0) {
                        for (vector<string>::iterator iite = cpNow->including.begin();
                                iite != cpNow->including.end();
                                ++iite)
                            if (*iite == nexResponseType) {
                                find = true;
                                break;
                            }
                        if (!find) legal = false;
                    }
                    find = false;
                    if (cpNow->excluding.size() > 0) {
                        for (vector<string>::iterator iite = cpNow->excluding.begin();
                                iite != cpNow->excluding.end();
                                ++iite)
                            if (*iite == nexResponseType) {
                                find = true;
                                break;
                            }
                        if (find) legal = false;
                    }
                    if (!legal) continue;

                    BaseDataObject *nowField = response;
                    DataSchemaObject *nowSchema = schema;
                    if ((nowField == NULL) || (nowField->type != OBJECT) || (nowSchema == NULL)) {
                        checkPointPass = false;
                        break;
                    }
                    vector<string> &fieldVec = cpNow->fieldVec;
                    if ((fieldVec.size() <= 1) || (fieldVec[1] != "headers"))
                        nowField = (*((ObjectDataObject*)response))["body"];
                    bool needDel = false;
                    for (int i=1; i<fieldVec.size(); ++i) {
                        // add "size" support
                        if (nowField->type == OBJECT) {
                            ObjectDataObject *objField = (ObjectDataObject*)nowField;
                            if ((fieldVec[i] == "size") && (((ObjectSchema*)nowSchema)->properties.count(fieldVec[i]) == 0)) {
                                // 'size' is an extra field
                                nowField = new IntegerDataObject(objField->size());
                                nowSchema = IntegerSchema::getSizeFieldSchema();
                                needDel = true;
                            } else {
                                if (!objField->hasKey(fieldVec[i]) ||
                                    (nowSchema->type != DataSchemaObject::TYPE_OBJECT) ||
                                    (((ObjectSchema *) nowSchema)->properties.count(fieldVec[i]) == 0)) {
                                    checkPointPass = false;
                                    break;
                                } else {
                                    nowField = (*objField)[fieldVec[i]];
                                    nowSchema = ((ObjectSchema *) nowSchema)->properties[fieldVec[i]];
                                }
                            }
                        }
                        else if (nowField->type == SEQUENCE) {
                            SequenceDataObject *seqField = (SequenceDataObject*)nowField;
                            if (fieldVec[i] == "size") {
                                nowField = new IntegerDataObject(seqField->length());
                                nowSchema = IntegerSchema::getSizeFieldSchema();
                                needDel = true;
                            } else {
                                if ((seqField->length() == 0) ||
                                    (nowSchema->type != DataSchemaObject::TYPE_ARRAY)) {
                                    checkPointPass = false;
                                    break;
                                }
                                int ind;
                                try {
                                    ind = stoi(fieldVec[i]);
                                } catch (std::invalid_argument) {
                                    checkPointPass = false;
                                    break;
                                } catch (std::out_of_range) {
                                    checkPointPass = false;
                                    break;
                                }
                                if (ind >= 0)
                                    nowField = (*seqField)[ind % seqField->length()];
                                else {
                                    ind = abs(ind) % seqField->length();
                                    if (ind == 0) ind = seqField->length();
                                    nowField = (*seqField)[seqField->length() - ind];
                                }
                                nowSchema = ((ArraySchema *) nowSchema)->items;
                            }
                        }
                        else {
                            checkPointPass = false;
                            if (needDel) delete nowField;
                            break;
                        }
                    }
                    if (!checkPointPass) break;
                    if (nowField == NULL) {
                        checkPointPass = false;
                        break;
                    }

                    if ((cpNow->type == EQUAL_CHECKPOINT) || (cpNow->type == UNEQUAL_CHECKPOINT)) {
                        BaseDataObject *guest = schema->transform(cpNow->value);
                        if (guest == NULL) {
                            if (needDel) delete nowField;
                            checkPointPass = false;
                            break;
                        } else {
                            if ((cpNow->type == EQUAL_CHECKPOINT) && (!nowField->equals(guest))) {
                                if (needDel) delete nowField;
                                checkPointPass = false;
                                delete guest;
                                break;
                            }
                            if ((cpNow->type == UNEQUAL_CHECKPOINT) && (nowField->equals(guest))) {
                                if (needDel) delete nowField;
                                checkPointPass = false;
                                delete guest;
                                break;
                            }
                            delete guest;
                        }
                    }

                    if (cpNow->type == ISSETSIZE_CHECKPOINT) {
                        int num = -1;
                        if (nowField->type == INTEGER)
                            num = (int)((IntegerDataObject*)nowField)->value;
                        if (nowField->type == NUMBER)
                            num = (int)((NumberDataObject*)nowField)->value;
                        if (num != nowSets[cpNow->setName].size()) {
                            checkPointPass = false;
                            if (needDel) delete nowField;
                            break;
                        }
                    }

                    if ((cpNow->type == INSET_CHECKPOINT) || (cpNow->type == OUTSET_CHECKPOINT)) {
                        bool in_set = false;
                        assert(nowSets.count(cpNow->setName) > 0);
                        vector<DocElement*> &nowSet = nowSets[cpNow->setName];
                        for (vector<DocElement*>::iterator iite = nowSet.begin();
                                iite != nowSet.end();
                                ++iite) {
                            BaseDataObject *guest = nowSchema->transform(*iite);
                            if (guest) {
                                if (nowField->equals(guest))
                                    in_set = true;
                                delete guest;
                            }
                            if (in_set) break;
                        }
                        if ((cpNow->type == INSET_CHECKPOINT) && (!in_set)) {
                            if (needDel) delete nowField;
                            checkPointPass = false;
                            break;
                        }
                        if ((cpNow->type == OUTSET_CHECKPOINT) && (in_set)) {
                            if (needDel) delete nowField;
                            checkPointPass = false;
                            break;
                        }
                        if (needDel) delete nowField;
                    }
                }

                if (checkPointPass) {
                    /** set effects **/
                    for (vector<ModuleSetEffectObject*>::iterator ite = nexModuleObj->setEffects.begin();
                            ite != nexModuleObj->setEffects.end();
                            ++ite) {
                        ModuleSetEffectObject *seNow = *ite;

                        bool legal = true;
                        bool find = false;
                        if (seNow->including.size() > 0) {
                            for (vector<string>::iterator iite = seNow->including.begin();
                                 iite != seNow->including.end();
                                 ++iite)
                                if (*iite == nexResponseType) {
                                    find = true;
                                    break;
                                }
                            if (!find) legal = false;
                        }
                        find = false;
                        if (seNow->excluding.size() > 0) {
                            for (vector<string>::iterator iite = seNow->excluding.begin();
                                 iite != seNow->excluding.end();
                                 ++iite)
                                if (*iite == nexResponseType) {
                                    find = true;
                                    break;
                                }
                            if (find) legal = false;
                        }
                        if (!legal) continue;

                        vector<DocElement*> &nowSet = nowSets[seNow->setName];
                        if (seNow->type == ADD_SETEFFECT) {
                            vector<string> &object = seNow->object;
                            BaseDataObject *obj = NULL;
                            int p = 0;
                            if (object[0] == "in")
                                obj = requesterReport->request[object[1]], p = 2;
                            if (object[0] == "out") {
                                if ((object.size() <= 1) || (object[1] != "headers"))
                                    obj = (*((ObjectDataObject*)response))["body"], p = 1;
                                else
                                    obj = response, p = 1;
                            }
                            bool needDel = false;
                            // add "size" support
                            for (int i = p; i < object.size(); ++i) {
                                if (obj == NULL)  {
                                    setEffectPass = false;
                                    break;
                                }
                                if (obj->type == OBJECT) {
                                    ObjectDataObject *objObj = (ObjectDataObject*)obj;
                                    if (!objObj->hasKey(object[i])) {
                                        if (object[i] == "size") {
                                            needDel = true;
                                            obj = new IntegerDataObject(objObj->size());
                                        } else {
                                            setEffectPass = false;
                                            break;
                                        }
                                    }
                                    obj = (*objObj)[object[i]];
                                }
                                else if (obj->type == SEQUENCE) {
                                    SequenceDataObject *seqObj = (SequenceDataObject*)obj;
                                    if (object[i] == "size") {
                                        needDel = true;
                                        obj = new IntegerDataObject(seqObj->length());
                                    } else {
                                        if (seqObj->length() == 0) {
                                            setEffectPass = false;
                                            break;
                                        }
                                        int ind;
                                        try {
                                            ind = stoi(object[i]);
                                        } catch (std::invalid_argument) {
                                            checkPointPass = false;
                                            break;
                                        } catch (std::out_of_range) {
                                            checkPointPass = false;
                                            break;
                                        }
                                        if (ind >= 0)
                                            obj = (*seqObj)[ind % seqObj->length()];
                                        else {
                                            ind = abs(ind) % seqObj->length();
                                            if (ind == 0) ind = seqObj->length();
                                            obj = (*seqObj)[seqObj->length() - ind];
                                        }
                                    }
                                } else {
                                    setEffectPass = false;
                                    if (needDel) delete obj;
                                    break;
                                }
                            }
                            if ((obj == NULL) || (!setEffectPass)) {
                                setEffectPass = false;
                                break;
                            }
                            DocElement *ele = DataObjectAdapter::toDocElement(obj);
                            bool exist = false;
                            if (scenario->sets[seNow->setName]->unique) {
                                for (vector<DocElement*>::iterator iite = nowSet.begin(); iite != nowSet.end(); ++iite)
                                    if (((*iite) != NULL) && ((*iite)->equals(ele))) {
                                        exist = true;
                                        break;
                                    }
                            }
                            if (!exist)
                                nowSet.push_back(ele);
                            if (needDel) delete obj;
                        }
                        if (seNow->type == DELETE_SETEFFECT) {
                            string &condition = seNow->condition;
                            vector<DocElement*> toDel;
                            toDel.clear();
                            /** TODO: illness delete **/
                            for (vector<DocElement*>::iterator iite = nowSet.begin();
                                    iite != nowSet.end();
                                    ++iite)
                                if (ModuleSetEffectObject::needDelete(condition, requesterReport->request, response, *iite)) {
//                                    toDel.push_back(*iite);
//                                    nowSet.erase(iite);
                                }
                            for (vector<DocElement*>::iterator iite = toDel.begin();
                                    iite != toDel.end();
                                    ++iite)
                                ;
//                                delete *iite;
                            nowSet.clear();
                        }
                    }
                }
            }
        }

        /** package part **/
        nowModule = nexModule;
        nowState = nexState;
        nowResponseType = nexResponseType;
        nowRequest = nexRequesterReport;
        moduleSeq.push_back(nexModule);
        ans->addStep(packetNow());

        /** checkpoint passed? **/
        if (ans->getTerminate() == NULL)
            if (!checkPointPass)
                ans->setTerminate(new CheckpointNotPassTerminate(nowModule));

        /** setEffect passed? **/
        if (ans->getTerminate() == NULL)
            if (!setEffectPass)
                ans->setTerminate(new IllegalSetEffectError(nowModule));

        /** meet end module? **/
        if (ans->getTerminate() == NULL)
            if (nexModuleObj->type == SCENARIO_END_MODULE)
                ans->setTerminate(new EndModuleTerminate(nowModule));
    }

    ans->end();
    return ans;
}

TestCaseStepRecord *TestCaseController::packetNow() {
    TestCaseStepRecord *now = new TestCaseStepRecord();
    now->moduleName = this->nowModule;
    TestCaseController::setsCpy(this->nowSets, now->sets);
    now->stateName = this->nowState;
    if (this->nowRequest != NULL)
        now->request = this->nowRequest;
    return now;
}

bool TestCaseController::checkConstraintsWith(string nowModule) {
    bool ans = true;
    moduleSeq.push_back(nowModule);

    for (int i=0; i<moduleSeq.size(); ++i)
        if (scenario->modules[moduleSeq[i]]->type == SCENARIO_NORMAL_MODULE) {
            APIObject *api = scenario->modules[moduleSeq[i]]->api;
            for (vector<APIConstraintObject*>::iterator ite = api->constraint.begin();
                    ite != api->constraint.end();
                    ++ite) {
                APIConstraintObject *nowCons = *ite;
                int l, r;
                if (nowCons->type == APIConstraintType::BEFORE) {
                    r = i - nowCons->minInterval;
                    if (nowCons->maxInterval == -1)
                        l = 0;
                    else
                        l = i - nowCons->maxInterval;
                }
                    /** TODO: now temporarily discard AFTER constraints **/
//                else if (nowCons->type == APIConstraintType::AFTER) {
//                    l = i + nowCons->minInterval;
//                    if (nowCons->maxInterval == -1)
//                        r = (int)moduleSeq.size() - 1;
//                    else
//                        r = i + nowCons->maxInterval;
//                    if (r > moduleSeq.size() - 1)
//                        r = (int)moduleSeq.size() - 1;
//                }
                else {
                    // err
                    l = 0, r = -1;
                }
                if (l > r) {
                    ans = false;
                    continue;
                }
                APIObject *guest = nowCons->guest;
                int appearTimes = 0;
                for (int j=l; j<=r; ++j)
                    if ((scenario->modules[moduleSeq[j]]->type == SCENARIO_NORMAL_MODULE)
                        && (scenario->modules[moduleSeq[j]]->api == guest))
                        ++appearTimes;
                if (appearTimes < nowCons->minTimes) ans = false;
                if ((nowCons->maxTimes != -1) && (appearTimes > nowCons->maxTimes)) ans = false;
            }
        }

    moduleSeq.pop_back();
    return ans;
}

void TestCaseController::setsCpy(
        map<string, vector<DocElement *>> &from,
        map<string, vector<DocElement *>> &to) {
    to.clear();
    for (map<string, vector<DocElement*>>::iterator ite = from.begin();
            ite != from.end();
            ++ite) {
        to[ite->first] = vector<DocElement*>();
        TestCaseController::vecDocEleCpy(from[ite->first], to[ite->first]);
    }
}

void TestCaseController::vecDocEleCpy(
        vector<DocElement *> &from,
        vector<DocElement *> &to) {
    to.clear();
    for (vector<DocElement*>::iterator ite = from.begin();
            ite != from.end();
            ++ite) {
        to.push_back(DocElementHelper::deepCopy(*ite));
    }
}