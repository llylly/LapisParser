//
// Created by lly on 23/09/2017.
//

#include "ModuleTransStateObject.h"

ModuleTransStateObject::ModuleTransStateObject() {
    this->preState = "";
    this->nexState = "";
    this->excluding.clear();
    this->including.clear();
}

ModuleTransStateObject::~ModuleTransStateObject() {
    this->excluding.clear();
    this->including.clear();
}

BaseDataObject *ModuleTransStateObject::toDataObject() {
    ObjectDataObject *obj = new ObjectDataObject();
    (*obj)["preState"] = new StringDataObject(this->preState);
    (*obj)["nexState"] = new StringDataObject(this->nexState);
    if (this->excluding.size() > 0) {
        SequenceDataObject *seq = new SequenceDataObject();
        for (vector<string>::iterator ite = this->excluding.begin();
                ite != this->excluding.end();
                ++ite) {
            seq->push(new StringDataObject(*ite));
        }
        (*obj)["excluding"] = seq;
    }
    if (this->including.size() > 0) {
        SequenceDataObject *seq = new SequenceDataObject();
        for (vector<string>::iterator ite = this->including.begin();
                ite != this->including.end();
                ++ite) {
            seq->push(new StringDataObject(*ite));
        }
        (*obj)["including"] = seq;
    }
    return obj;
}

ModuleTransStateObject *ModuleTransStateObjectFactory::create(string nexState) {
    if (nexState == "") return NULL;
    ModuleTransStateObject *res = new ModuleTransStateObject();
    res->preState = "default";
    res->nexState = nexState;
    return res;
}

ModuleTransStateObject *ModuleTransStateObjectFactory::create(string filePath, DocElement *ele) {
    if (ele == NULL) return NULL;
    if (ele->type != DOC_OBJECT) {
        Error::addError(new FieldInvalidError(filePath, ele->line, ele->col, "x-scenario.modules.transState",
                                              ele->type, DOC_OBJECT));
        return NULL;
    }
    DocObjectElement *objEle = (DocObjectElement*)ele;
    ModuleTransStateObject *res = new ModuleTransStateObject();

    /** preState (required) **/
    DocElement *preStateEle = objEle->get("preState");
    if (preStateEle == NULL) {
        Error::addError(new FieldMissError(filePath, ele->line, ele->col, "x-scenario.modules.transState.preState"));
        delete res;
        return NULL;
    } else {
        pair<string, bool> parseRes = DocElementHelper::parseToString(preStateEle);
        if (!parseRes.second) {
            Error::addError(new FieldInvalidError(filePath, preStateEle->line, preStateEle->col, "x-scenario.modules.transState.preState",
                                                  preStateEle->type, DOC_SCALAR));
            delete res;
            return NULL;
        } else {
            res->preState = parseRes.first;
        }
    }

    /** nexState (required) **/
    DocElement *nexStateEle = objEle->get("nexState");
    if (nexStateEle == NULL) {
        Error::addError(new FieldMissError(filePath, ele->line, ele->col, "x-scenario.modules.transState.nexState"));
        delete res;
        return NULL;
    } else {
        pair<string, bool> parseRes = DocElementHelper::parseToString(nexStateEle);
        if (!parseRes.second) {
            Error::addError(new FieldInvalidError(filePath, nexStateEle->line, nexStateEle->col, "x-sceanrio.modules.transState.nexState",
                                                  nexStateEle->type, DOC_SCALAR));
            delete res;
            return NULL;
        } else {
            res->nexState = parseRes.first;
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