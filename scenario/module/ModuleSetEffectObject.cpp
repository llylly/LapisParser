//
// Created by lly on 23/09/2017.
//

#include "ModuleSetEffectObject.h"

ModuleSetEffectObject::ModuleSetEffectObject() {
    this->type = INVALID_SETEFFECT;
    this->object.clear();
    this->setName = "";
    this->condition = "";
    this->excluding.clear();
    this->including.clear();
}

ModuleSetEffectObject::~ModuleSetEffectObject() {
    this->object.clear();
    this->excluding.clear();
    this->including.clear();
}

BaseDataObject *ModuleSetEffectObject::toDataObject() {
    ObjectDataObject *res = new ObjectDataObject();

    string typeStr = "";
    switch (this->type) {
        case ADD_SETEFFECT:
            typeStr = "add";
            break;
        case DELETE_SETEFFECT:
            typeStr = "delete";
            break;
        default:
            ;
    }
    (*res)["type"] = new StringDataObject(typeStr);

    if (this->type == ADD_SETEFFECT) {
        string objStr = "";
        int len = (int)this->object.size();
        for (int i=0; i<len; ++i) {
            objStr += this->object[i];
            if (i != (len - 1)) objStr += ".";
        }
        (*res)["object"] = new StringDataObject(objStr);
    }

    (*res)["set"] = new StringDataObject(this->setName);

    if (this->type == DELETE_SETEFFECT) {
        (*res)["condition"] = new StringDataObject(this->condition);
    }

    if (this->excluding.size() > 0) {
        SequenceDataObject *exSeqObj = new SequenceDataObject();
        int len = (int)this->excluding.size();
        for (int i=0; i<len; ++i) {
            exSeqObj->push(new StringDataObject(this->excluding[i]));
        }
        (*res)["excluding"] = exSeqObj;
    }

    if (this->including.size() > 0) {
        SequenceDataObject *inSeqObj = new SequenceDataObject();
        int len = (int)this->including.size();
        for (int i=0; i<len; ++i) {
            inSeqObj->push(new StringDataObject(this->including[i]));
        }
        (*res)["including"] = inSeqObj;
    }

    return res;
}

bool ModuleSetEffectObject::needDelete(string condition, BaseDataObject *in, BaseDataObject *out,
                                       BaseDataObject *now) {
    /** TODO **/
    /** Now random delete with 1/4 probability **/
    srand(time(0));
    if ((time(0) & 7)  <= 1) return true; else return false;
}

bool ModuleSetEffectObject::conditionCheck(string condition, APIObject *api) {
    /** TODO **/
    /** Now always legal **/
    return true;
}

bool ModuleSetEffectObject::inFieldCheck(const vector<string> &inObj, APIObject *api) {
    if (inObj.size() < 2) return false;
    if (inObj[0] != "in") return false;
    if (api == NULL) return false;
    const string &paramName = inObj[1];
    if (api->parameters.count(paramName) == 0) return false;
    DataSchemaObject *schema = api->parameters[paramName]->schema;
    for (int i=2; i<inObj.size(); ++i) {
        if (schema->type == DataSchemaObject::TYPE_OBJECT) {
            ObjectSchema *objSchema = (ObjectSchema*)schema;
            if (objSchema->properties.count(inObj[i]) == 0) return false;
            schema = objSchema->properties[inObj[i]];
        }
        else if (schema->type == DataSchemaObject::TYPE_ARRAY) {
            try {
                int index = stoi(inObj[i]);
            } catch (std::invalid_argument) {
                return false;
            } catch (std::out_of_range) {
                return false;
            }
            ArraySchema *arrSchema = (ArraySchema*)schema;
            schema = arrSchema->items;
        }
        else
            return false;
    }
    return true;
}

bool ModuleSetEffectObject::outFieldCheck(const vector<string> &outObj) {
    return (outObj.size() >= 1) && (outObj[0] == "out");
}

ModuleSetEffectObject *ModuleSetEffectObjectFactory::create(string filePath, DocElement *ele, APIObject *api) {
    if ((ele == NULL) || (api == NULL)) return NULL;
    ModuleSetEffectObject *res = new ModuleSetEffectObject();

    if (ele->type != DOC_OBJECT) {
        Error::addError(new FieldInvalidError(filePath, ele->line, ele->col, "x-scenario.modules.setEffects", ele->type, DOC_OBJECT));
        delete res;
        return NULL;
    }
    DocObjectElement *objEle = (DocObjectElement*)ele;

    /** type (required) **/
    DocElement *typeEle = objEle->get("type");
    if (typeEle == NULL) {
        Error::addError(new FieldMissError(filePath, ele->line, ele->col, "x-scenario.modules.setEffects.type"));
        delete res;
        return NULL;
    } else {
        pair<string, bool> typeParse = DocElementHelper::parseToString(typeEle);
        if (!typeParse.second) {
            Error::addError(new FieldInvalidError(filePath, typeEle->line, typeEle->col, "x-scenario.modules.setEffects.type",
                                                  typeEle->type, DOC_SCALAR));
            delete res;
            return NULL;
        } else {
            string &typeStr = typeParse.first;
            if (typeStr == "add")
                res->type = ADD_SETEFFECT;
            else if (typeStr == "delete")
                res->type = DELETE_SETEFFECT;
            else {
                Error::addError(new FieldIllegalError(filePath, typeEle->line, typeEle->col, "x-scenario.modules.setEffects.type"));
                delete res;
                return NULL;
            }
        }
    }

    if (res->type == ADD_SETEFFECT) {
        /** object (required) **/
        DocElement *objectEle = objEle->get("object");
        if (objectEle == NULL) {
            Error::addError(new FieldMissError(filePath, ele->line, ele->col, "x-scenario.modules.setEffects.object"));
            delete res;
            return NULL;
        } else {
            pair<string, bool> objectParse = DocElementHelper::parseToString(objectEle);
            if (!objectParse.second) {
                Error::addError(new FieldInvalidError(filePath, objectEle->line, objectEle->col, "x-scenario.modules.setEffects.object",
                                                      objectEle->type, DOC_SCALAR));
                delete res;
                return NULL;
            } else {
                string &objectStr = objectParse.first;
                int key_s = 0;
                for (int i=0; i <= objectStr.length(); ++i) {
                    char c = (i == objectStr.length())? '.': objectStr[i];
                    if (c == '.') {
                        if (key_s < i)
                            res->object.push_back(objectStr.substr((unsigned)key_s, (unsigned)(i - key_s)));
                        key_s = i + 1;
                    }
                }
                bool legal = ModuleSetEffectObject::inFieldCheck(res->object, api) || ModuleSetEffectObject::outFieldCheck(res->object);
                if (!legal) {
                    Error::addError(new FieldIllegalError(filePath, objectEle->line, objectEle->col, "x-scenario.modules.setEffects.object"));
                    delete res;
                    return NULL;
                }
            }
        }
    }

    /** set (required) **/
    DocElement *setEle = objEle->get("set");
    if (setEle == NULL) {
        Error::addError(new FieldMissError(filePath, ele->line, ele->col, "x-scenario.modules.setEffects.set"));
        delete res;
        return NULL;
    } else {
        pair<string, bool> setParse = DocElementHelper::parseToString(setEle);
        if (!setParse.second) {
            Error::addError(new FieldInvalidError(filePath, setEle->line, setEle->col, "x-scenario.modules.setEffects.set",
                                                  setEle->type, DOC_SCALAR));
            delete res;
            return NULL;
        } else {
            res->setName = setParse.first;
        }
    }

    if (res->type == DELETE_SETEFFECT) {
        /** condition (required) **/
        DocElement *condEle = objEle->get("condition");
        if (condEle == NULL) {
            Error::addError(new FieldMissError(filePath, ele->line, ele->col, "x-scenario.modules.setEffects.condition"));
            delete res;
            return NULL;
        } else {
            pair<string, bool> condParse = DocElementHelper::parseToString(condEle);
            if (!condParse.second) {
                Error::addError(new FieldInvalidError(filePath, condEle->line, condEle->col, "x-scenario.modules.setEffects.condition",
                                                      condEle->type, DOC_SCALAR));
                delete res;
                return NULL;
            } else {
                if (ModuleSetEffectObject::conditionCheck(condParse.first, api))
                    res->condition = condParse.first;
                else {
                    Error::addError(new FieldIllegalError(filePath, condEle->line, condEle->col, "x-scenario.modules.setEffects.condition"));
                    delete res;
                    return NULL;
                }
            }
        }
    }

    /** excluding (optional) **/
    DocElement *exEle = objEle->get("excluding");
    if (exEle != NULL) {
        if (exEle->type != DOC_SEQUENCE) {
            Error::addError(new FieldInvalidError(filePath, exEle->line, exEle->col, "x-scenario.modules.setEffects.excluding",
                                                  exEle->type, DOC_SEQUENCE));
            delete res;
            return NULL;
        } else {
            DocSequenceElement *seqExEle = (DocSequenceElement*)exEle;
            int len = seqExEle->getLength();
            for (int i=0; i<len; ++i) {
                DocElement *nowEle = seqExEle->get(i);
                if (nowEle->type != DOC_SCALAR) {
                    Error::addError(new FieldInvalidError(filePath, nowEle->line, nowEle->col, "x-scenario.modules.setEffects.excluding",
                                                          nowEle->type, DOC_SCALAR));
                    delete res;
                    return NULL;
                } else {
                    res->excluding.push_back(((DocScalarElement*)nowEle)->getValue());
                }
            }
        }
    }

    /** including (optional) **/
    DocElement *inEle = objEle->get("including");
    if (inEle != NULL) {
        if (inEle->type != DOC_SEQUENCE) {
            Error::addError(new FieldInvalidError(filePath, inEle->line, inEle->col, "x-scenario.modules.setEffects.including",
                                                  inEle->type, DOC_SEQUENCE));
            delete res;
            return NULL;
        } else {
            DocSequenceElement *seqInEle = (DocSequenceElement*)inEle;
            int len = seqInEle->getLength();
            for (int i=0; i<len; ++i) {
                DocElement *nowEle = seqInEle->get(i);
                if (nowEle->type != DOC_SCALAR) {
                    Error::addError(new FieldInvalidError(filePath, nowEle->line, nowEle->col, "x-scenario.modules.setEffects.including",
                                                          nowEle->type, DOC_SCALAR));
                    delete res;
                    return NULL;
                } else {
                    res->including.push_back(((DocScalarElement*)nowEle)->getValue());
                }
            }
        }
    }

    return res;
}