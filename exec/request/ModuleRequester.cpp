//
// Created by lly on 06/10/2017.
//

#include "ModuleRequester.h"

ModuleRequester::ModuleRequester(string host,
                                 string basePath,
                                 RequesterReport *report,
                                 map<string, string> *(*middleware_func)(map<string, string>*, void*),
                                 int timeout):
        BaseRequester(host, basePath, report, middleware_func, timeout) { }

map<string, BaseDataObject*> *ModuleRequester::dataGen() {
    srand(time(0));
    map<string, BaseDataObject*> *ans = new map<string, BaseDataObject*>();
    for (map<string, ParameterObject*>::iterator ite = api->parameters.begin();
         ite != api->parameters.end();
         ++ite) {
        const string &paramName = ite->first;
        ParameterObject *nowParam = ite->second;
        if (DataSchemaObject::randomReal() <= nowParam->nullProbability)
            continue;
        BaseDataObject *nowData = nowParam->schema->generate();

        /** input-constraints begin **/
        vector<ModuleInputConstraintObject*> &inputCons = module->inputs;
        vector<ModuleInputConstraintObject*> nowConsVec;
        nowConsVec.clear();
        for (vector<ModuleInputConstraintObject*>::iterator iite = inputCons.begin();
                iite != inputCons.end();
                ++iite) {
            if ((*iite)->fieldVec[1] == paramName)
                nowConsVec.push_back(*iite);
        }
        if (nowConsVec.size() > 0) {
            /** check embodied fields and invalidate them **/
            {
                bool found;
                do {
                    found = false;
                    for (vector<ModuleInputConstraintObject*>::iterator i = nowConsVec.begin();
                            i != nowConsVec.end();
                            ++i) {
                        for (vector<ModuleInputConstraintObject*>::iterator j = nowConsVec.begin();
                                j != nowConsVec.end();
                                ++j)
                            if (i != j) {
                                ModuleInputConstraintObject *a = *i;
                                ModuleInputConstraintObject *b = *j;
                                vector<string> &veca = a->fieldVec;
                                vector<string> &vecb = b->fieldVec;
                                if (veca.size() != vecb.size()) {
                                    int min = (int)((veca.size() < vecb.size()) ? veca.size(): vecb.size());
                                    bool same = true;
                                    for (int k=0; k<min; ++k)
                                        if (veca[k] != vecb[k]) {
                                            same = false;
                                            break;
                                        }
                                    if (same) {
                                        found = true;
                                        if (veca.size() > min)
                                            nowConsVec.erase(i);
                                        else
                                            nowConsVec.erase(j);
                                    }
                                }
                            }
                    }
                } while (found);
            }

            int MAX_TRY_TIMES = 100;

            /** After MAX_TRY_TIMES tries, if the new values still don't satisfy the schema, just give up **/

            for (vector<ModuleInputConstraintObject *>::iterator now = nowConsVec.begin();
                 now != nowConsVec.end();
                 ++now) {

                ModuleInputConstraintObject *nowCons = *now;
                vector<string> &nowFieldVec = (*now)->fieldVec;
                BaseDataObject *preField = NULL;
                BaseDataObject *nowField = nowData;
                DataSchemaObject *nowSchema = nowParam->schema;

                /** Create nodes of required constraints **/
                for (int i = 2; i < nowFieldVec.size(); ++i) {
                    if (nowSchema->type == DataSchemaObject::TYPE_OBJECT) {
                        if (nowField->type != OBJECT) {
                            // this should never happen
                            this->err = new IllegalInputConstraintError(nowCons->getKeyName());
                            return NULL;
                        } else {
                            ObjectDataObject *objField = (ObjectDataObject*)nowField;
                            for (int tr=0; tr<MAX_TRY_TIMES; ++tr)
                                if (!objField->hasKey(nowFieldVec[i])) {
                                    BaseDataObject *gen = (((ObjectSchema *) nowSchema)->properties)[nowFieldVec[i]]->generate();
                                    if (gen)
                                        (*objField)[nowFieldVec[i]] = gen;
                                } else
                                    break;
                            if (!objField->hasKey(nowFieldVec[i])) {
                                // after tried MAX_TRY_TIMES times, still can't get a non-null value
                                this->err = new IllegalInputConstraintError(nowCons->getKeyName());
                                return NULL;
                            }
                            preField = nowField;
                            nowField = (*objField)[nowFieldVec[i]];
                            nowSchema = ((ObjectSchema*)nowSchema)->properties[nowFieldVec[i]];
                        }
                    }
                    else if (nowSchema->type == DataSchemaObject::TYPE_ARRAY) {
                        if (nowField->type != SEQUENCE) {
                            // this should never happen
                            this->err = new IllegalInputConstraintError(nowCons->getKeyName());
                            return NULL;
                        } else {
                            if (((ArraySchema*)nowSchema)->maxItems <= 0) {
                                // if the sequence is always empty, in no way can we exert constraints on it
                                this->err = new IllegalInputConstraintError(nowCons->getKeyName());
                                return NULL;
                            }
                            SequenceDataObject *seqField = (SequenceDataObject*)nowField;
                            if (seqField->length() == 0) {
                                for (int tr=0; tr<MAX_TRY_TIMES; ++tr) {
                                    if (seqField->length() == 0) {
                                        BaseDataObject *gen = ((ArraySchema *) nowSchema)->items->generate();
                                        if (gen)
                                            seqField->push(gen);
                                    } else
                                        break;
                                }
                                if (seqField->length() == 0) {
                                    // after tried MAX_TRY_TIMES times, still can't get a non-null value
                                    this->err = new IllegalInputConstraintError(nowCons->getKeyName());
                                    return NULL;
                                }
                            }
                            preField = nowField;
                            int ind = stoi(nowFieldVec[i]);
                            if (ind >= 0)
                                nowField = (*seqField)[ind % seqField->length()];
                            else {
                                ind = abs(ind) % seqField->length();
                                if (ind == 0) ind = seqField->length();
                                nowField = (*seqField)[seqField->length() - ind];
                            }
                            nowSchema = ((ArraySchema*)nowSchema)->items;
                        }
                    }
                    else {
                        // this should never happen
                        this->err = new IllegalInputConstraintError(nowCons->getKeyName());
                        return NULL;
                    }
                }


                /** try to generate new value */
                BaseDataObject *newValue = NULL;
                for (int tr=0; tr<MAX_TRY_TIMES; ++tr) {
                    if (nowCons->type == RANDOM_INPUTCONSTRAINT) {
                        newValue = nowCons->from->generate();
                    }
                    if (nowCons->type == FROMSET_INPUTCONSTRAINT) {
                        if ((this->sets == NULL) || (this->sets->count(nowCons->setName) == 0)) {
                            if (nowCons->setPolicy == SIZE_SETPOLICY)
                                newValue = new IntegerDataObject(0);
                            if (nowCons->setPolicy == MEMBER_SETPOLICY) {
                                if (nowCons->nullPolicy == RANDOM_NULLPOLICY)
                                    newValue = DataObjectAdapter::deepCopy(nowField);
                                if (nowCons->nullPolicy == BREAK_NULLPOLICY) {
                                    this->err = new SetEmptyTerminate(nowCons->getKeyName(), nowCons->setName);
                                    return NULL;
                                }
                            }
                        } else {
                            if (nowCons->setPolicy == SIZE_SETPOLICY)
                                newValue = new IntegerDataObject(int((*this->sets)[nowCons->setName].size()));
                            if (nowCons->setPolicy == MEMBER_SETPOLICY) {
                                int setSize = int((*this->sets)[nowCons->setName].size());
                                DocElement *nowEle = (*this->sets)[nowCons->setName][rand() % setSize];
                                BaseDataObject *nowObj = nowSchema->transform(nowEle);
                                if (nowObj != NULL)
                                    newValue = nowObj;
                            }
                        }
                    }
                    if (newValue)
                        newValue = ModuleInputConstraintObjectFactory::expEval(nowCons->expression, newValue);
                    if (newValue && (!nowSchema->check(newValue))) {
                        delete newValue;
                        newValue = NULL;
                    }
                    if (newValue) break;
                }

                if (newValue == NULL) {
                    this->err = new IllegalInputConstraintError(nowCons->getKeyName());
                    return NULL;
                }

                /** substitute old random value with the new value */
                if (preField == NULL)
                    nowData = newValue;
                else {
                    if (preField->type == OBJECT) {
                        ObjectDataObject *objVer = (ObjectDataObject*)preField;
                        map<string, BaseDataObject*> &m = objVer->m;
                        for (map<string, BaseDataObject*>::iterator iiite = m.begin();
                                iiite != m.end();
                                ++iiite)
                            if (iiite->second == nowField)
                                iiite->second = newValue;
                    }
                    else if (preField->type == SEQUENCE) {
                        SequenceDataObject *seqVer = (SequenceDataObject*)preField;
                        vector<BaseDataObject*> &vec = seqVer->vec;
                        for (vector<BaseDataObject*>::iterator iiite = vec.begin();
                                iiite != vec.end();
                                ++iiite)
                            if (*iiite == nowField)
                                *iiite = newValue;
                    }
                }

                delete nowField;

            }

        }
        /** input-constraints end **/

        (*ans)[paramName] = nowData;
    }
    return ans;
}

bool ModuleRequester::init(
        ScenarioModuleObject *module,
        map<string, vector<DocElement *>> *sets) {
    if (module == NULL) return false;
    this->module = module;
    this->sets = sets;
}