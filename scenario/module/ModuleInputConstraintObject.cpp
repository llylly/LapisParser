//
// Created by lly on 23/09/2017.
//

#include "ModuleInputConstraintObject.h"

ModuleInputConstraintObject::ModuleInputConstraintObject() {
    this->fieldVec.clear();
    this->type = INVALID_INPUTCONSTRAINT;
    this->from = NULL;
    this->setName = "";
    this->setPolicy = INVALID_SETPOLICY;
    this->expression = "$";
    this->nullPolicy = INVALID_NULLPOLICY;
}

ModuleInputConstraintObject::~ModuleInputConstraintObject() {
    this->fieldVec.clear();
}

string ModuleInputConstraintObject::getKeyName() {
    string fieldStr = "";
    for (int i=0; i < this->fieldVec.size(); ++i) {
        fieldStr += this->fieldVec[i];
        if (i != this->fieldVec.size() - 1) fieldStr += '.';
    }
    return fieldStr;
}

BaseDataObject *ModuleInputConstraintObject::toDataObject() {
    ObjectDataObject *ans = new ObjectDataObject();

    string typeStr = "";
    switch (this->type) {
        case RANDOM_INPUTCONSTRAINT:
            typeStr = "random";
            break;
        case FROMSET_INPUTCONSTRAINT:
            typeStr = "fromSet";
            break;
        default:
            ;
    }
    (*ans)["type"] = new StringDataObject(typeStr);

    if (this->type == RANDOM_INPUTCONSTRAINT) {
        // random
        if (from != NULL) {
            (*ans)["from"] = this->from->toDataObject();
        }
    } else {
        // fromSet
        (*ans)["setName"] = new StringDataObject(this->setName);

        string setPolicyStr = "";
        switch (this->setPolicy) {
            case MEMBER_SETPOLICY:
                setPolicyStr = "member";
                break;
            case SIZE_SETPOLICY:
                setPolicyStr = "size";
                break;
            default:
                ;
        }
        (*ans)["setPolicy"] = new StringDataObject(setPolicyStr);

        (*ans)["expression"] = new StringDataObject(this->expression);

        string nullPolicyStr = "";
        switch (this->nullPolicy) {
            case BREAK_NULLPOLICY:
                nullPolicyStr = "break";
                break;
            case RANDOM_NULLPOLICY:
                nullPolicyStr = "random";
                break;
            default:
                ;
        }
        if (this->setPolicy == MEMBER_SETPOLICY)
            (*ans)["nullPolicy"] = new StringDataObject(nullPolicyStr);
    }

    return ans;
}

ModuleInputConstraintObject *ModuleInputConstraintObjectFactory::create(string filePath, string key, DocElement *ele, Controller *controller) {
    if ((ele == NULL) || (controller == NULL) || (key == "")) return NULL;
    ModuleInputConstraintObject *res = new ModuleInputConstraintObject();

    /** key parse **/
    int key_s = 0;
    for (int i=0; i <= key.length(); ++i) {
        char c = (i == key.length())? '.': key[i];
        if (c == '.') {
            if (key_s < i)
                res->fieldVec.push_back(key.substr((unsigned)key_s, (unsigned)(i - key_s)));
            key_s = i + 1;
        }
    }
    if ((res->fieldVec[0] != "in") || (res->fieldVec.size() < 2)) {
        Error::addError(new FieldIllegalError(filePath, ele->line, ele->col, "x-scenario.modules.inputs.key"));
        delete res;
        return NULL;
    }

    if (ele->type != DOC_OBJECT) {
        Error::addError(new FieldInvalidError(filePath, ele->line, ele->col, "x-scenario.modules.inputs", ele->type, DOC_OBJECT));
        delete res;
        return NULL;
    }
    DocObjectElement *objEle = (DocObjectElement*)ele;

    /** type (required) **/
    DocElement *typeEle = objEle->get("type");
    if (typeEle == NULL) {
        Error::addError(new FieldMissError(filePath, ele->line, ele->col, "x-sceanrio.modules.inputs.type"));
        delete res;
        return NULL;
    } else {
        pair<string, bool> typeParse = DocElementHelper::parseToString(typeEle);
        if (!typeParse.second) {
            Error::addError(new FieldInvalidError(filePath, typeEle->line, typeEle->col, "x-scenario.modules.inputs.type",
            typeEle->type, DOC_SCALAR));
            delete res;
            return NULL;
        } else {
            string &typeStr = typeParse.first;
            if (typeStr == "random")
                res->type = RANDOM_INPUTCONSTRAINT;
            else if (typeStr == "fromSet")
                res->type = FROMSET_INPUTCONSTRAINT;
            else {
                Error::addError(new FieldIllegalError(filePath, typeEle->line, typeEle->col, "x-scenario.modules.inputs.type"));
                delete res;
                return NULL;
            }
        }
    }

    if (res->type == RANDOM_INPUTCONSTRAINT) {
        /** from (optional) **/
        DocElement *fromEle = objEle->get("from");
        if (fromEle != NULL) {
            DataSchemaObject* fromObj = controller->definitions->parseDataSchema(filePath, fromEle);
            if (fromObj == NULL) {
                delete res;
                return NULL;
            } else
                res->from = fromObj;
        }
    }

    if (res->type == FROMSET_INPUTCONSTRAINT) {
        /** setName (required) **/
        DocElement *setNameEle = objEle->get("setName");
        if (setNameEle == NULL) {
            Error::addError(new FieldMissError(filePath, ele->line, ele->col, "x-scenario.modules.inputs.setName"));
            delete res;
            return NULL;
        } else {
            pair<string, bool> setNameParse = DocElementHelper::parseToString(setNameEle);
            if (!setNameParse.second) {
                Error::addError(new FieldInvalidError(filePath, setNameEle->line, setNameEle->col, "x-scenario.modules.inputs.setName",
                setNameEle->type, DOC_SCALAR));
                delete res;
                return NULL;
            } else {
                res->setName = setNameParse.first;
            }
        }

        /** setPolicy (optional) **/
        res->setPolicy = MEMBER_SETPOLICY;
        DocElement *setPolicyEle = objEle->get("setPolicy");
        if (setPolicyEle != NULL) {
            pair<string, bool> setPolicyParse = DocElementHelper::parseToString(setPolicyEle);
            if (!setPolicyParse.second) {
                Error::addError(new FieldInvalidError(filePath, setPolicyEle->line, setPolicyEle->col, "x-scenario.modules.inputs.setPolicy",
                setPolicyEle->type, DOC_SCALAR));
                delete res;
                return NULL;
            } else {
                string &setPolicyStr = setPolicyParse.first;
                if (setPolicyStr == "member")
                    res->setPolicy = MEMBER_SETPOLICY;
                else if (setPolicyStr == "size")
                    res->setPolicy = SIZE_SETPOLICY;
                else {
                    Error::addError(new FieldIllegalError(filePath, setPolicyEle->line, setPolicyEle->col, "x-scenario.modules.inputs.setPolicy"));
                    delete res;
                    return NULL;
                }
            }
        }

        /** expression (optional) **/
        DocElement *expEle = objEle->get("expression");
        if (expEle != NULL) {
            pair<string, bool> expParse = DocElementHelper::parseToString(expEle);
            if (!expParse.second) {
                Error::addError(new FieldInvalidError(filePath, expEle->line, expEle->col, "x-scenario.modules.inputs.expression",
                expEle->type, DOC_SCALAR));
                delete res;
                return NULL;
            } else {
                /** TODO: expression validation **/
                if (!expressionValidate(expParse.first)) {
                    Error::addError(new InvalidExpressionError(filePath, expEle->line, expEle->col, "x-scenario.modules.inputs.expression"));
                    delete res;
                    return NULL;
                } else {
                    res->expression = expParse.first;
                }
            }
        }

        /** nullPolicy (optional) **/
        if (res->setPolicy == MEMBER_SETPOLICY) {
            res->nullPolicy = RANDOM_NULLPOLICY;
            DocElement *nullPolicyEle = objEle->get("nullPolicy");
            if (nullPolicyEle != NULL) {
                pair<string, bool> nullPolicyParse = DocElementHelper::parseToString(nullPolicyEle);
                if (!nullPolicyParse.second) {
                    Error::addError(new FieldInvalidError(filePath, nullPolicyEle->line, nullPolicyEle->col, "x-scenario.modules.inputs.nullPolicy",
                    nullPolicyEle->type, DOC_SCALAR));
                    delete res;
                    return NULL;
                } else {
                    string &nullPolicyStr = nullPolicyParse.first;
                    if (nullPolicyStr == "break")
                        res->nullPolicy = BREAK_NULLPOLICY;
                    else if (nullPolicyStr == "random")
                        res->nullPolicy = RANDOM_NULLPOLICY;
                    else {
                        Error::addError(new FieldIllegalError(filePath, nullPolicyEle->line, nullPolicyEle->col, "x-scenario.modules.inputs.nullPolicy"));
                        delete res;
                        return NULL;
                    }
                }
            }
        }
    }

    return res;
}

bool ModuleInputConstraintObjectFactory::expressionValidate(string exp) {
    /** TODO: expression validation **/
    return true;
}

BaseDataObject *ModuleInputConstraintObjectFactory::expEval(string exp, BaseDataObject *dollar) {
    /** TODO: expression eval **/
    return dollar;
}