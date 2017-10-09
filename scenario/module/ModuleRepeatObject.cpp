//
// Created by lly on 23/09/2017.
//

#include "ModuleRepeatObject.h"
#include "../../data/ObjectDataObject.h"
#include "../../data/IntegerDataObject.h"
#include "../../data/SequenceDataObject.h"
#include "../../data/StringDataObject.h"
#include "../../doc/DocObjectElement.h"
#include "../../doc/DocSequenceElement.h"
#include "../../doc/DocScalarElement.h"

ModuleRepeatObject::ModuleRepeatObject() {
    this->time = 1;
    this->excluding.clear();
    this->including.clear();
}

ModuleRepeatObject::~ModuleRepeatObject() {
    this->excluding.clear();
    this->including.clear();
}

BaseDataObject *ModuleRepeatObject::toDataObject() {
    ObjectDataObject *obj = new ObjectDataObject();
    (*obj)["time"] = new IntegerDataObject(this->time);
    if (this->excluding.size() > 0) {
        SequenceDataObject *excludingObj = new SequenceDataObject();
        for (vector<string>::iterator ite = this->excluding.begin();
                ite != this->excluding.end();
                ++ite) {
            excludingObj->push(new StringDataObject(*ite));
        }
        (*obj)["excluding"] = excludingObj;
    }
    if (this->including.size() > 0) {
        SequenceDataObject *includingObj = new SequenceDataObject();
        for (vector<string>::iterator ite = this->including.begin();
                ite != this->including.end();
                ++ite) {
            includingObj->push(new StringDataObject(*ite));
        }
        (*obj)["including"] = includingObj;
    }
    return obj;
}

ModuleRepeatObject *ModuleRepeatObjectFactory::createDefault() {
    return new ModuleRepeatObject();
}

ModuleRepeatObject *ModuleRepeatObjectFactory::create(string filePath, DocElement *ele) {
    if (ele == NULL) {
        return NULL;
    }
    ModuleRepeatObject *ans = new ModuleRepeatObject();
    if (ele->type == DOC_SCALAR) {
        /** if SCALAR type, then treat it as merely repeat times without state constraints **/
        pair<long long, bool> parseRes = DocElementHelper::parseToInt(ele);
        if (parseRes.second) {
            ans->time = (int)parseRes.first;
        } else {
            Error::addError(
                    new FieldIllegalError(filePath, ele->line, ele->col, "x-scenario.modules.repeat")
            );
            delete ans;
            return NULL;
        }
    }
    if (ele->type == DOC_SEQUENCE) {
        Error::addError(
                new FieldInvalidError(filePath, ele->line, ele->col, "x-scenario.modules.repeat",
                ele->type, DOC_OBJECT)
        );
        delete ans;
        return NULL;
    }
    if (ele->type == DOC_OBJECT) {
        DocObjectElement *objEle = (DocObjectElement*)ele;

        DocElement *timeEle = objEle->get("time");
        /** time (required) **/
        if (timeEle == NULL) {
            Error::addError(
                    new FieldMissError(filePath, objEle->line, objEle->col, "x-scenario.modules.repeat.time")
            );
            delete ans;
            return NULL;
        } else {
            pair<long long, bool> parseRes = DocElementHelper::parseToInt(timeEle);
            if (parseRes.second) {
                ans->time = (int)parseRes.first;
            } else {
                Error::addError(
                        new FieldIllegalError(filePath, timeEle->line, timeEle->col, "x-scenario.modules.repeat.time")
                );
                delete ans;
                return NULL;
            }
        }

        DocElement *excludingEle = objEle->get("excluding");
        /** excluding (optional) **/
        if (excludingEle != NULL) {
            if (excludingEle->type != DOC_SEQUENCE) {
                Error::addError(
                        new FieldInvalidError(filePath, excludingEle->line, excludingEle->col, "x-scenario.modules.repeat.excluding",
                        excludingEle->type, DOC_SEQUENCE)
                );
                delete ans;
                return NULL;
            } else {
                DocSequenceElement *seqExcludingEle = (DocSequenceElement*)excludingEle;
                int len = seqExcludingEle->getLength();
                for (int i=0; i<len; ++i) {
                    DocElement *item = seqExcludingEle->get(i);
                    if (item->type != DOC_SCALAR) {
                        Error::addError(
                                new FieldInvalidError(filePath, item->line, item->col, "x-scenario.modules.repeat.excluding",
                                item->type, DOC_SCALAR)
                        );
                        delete ans;
                        return NULL;
                    } else {
                        ans->excluding.push_back(((DocScalarElement*)item)->getValue());
                    }
                }
            }
        }

        DocElement *includingEle = objEle->get("including");
        /** including (optional) **/
        if (includingEle != NULL) {
            if (includingEle->type != DOC_SEQUENCE) {
                Error::addError(
                        new FieldInvalidError(filePath, includingEle->line, includingEle->col, "x-scenario.modules.repeat.including",
                        includingEle->type, DOC_SEQUENCE)
                );
                delete ans;
                return NULL;
            } else {
                DocSequenceElement *seqIncludingEle = (DocSequenceElement*)includingEle;
                int len = seqIncludingEle->getLength();
                for (int i=0; i<len; ++i) {
                    DocElement *item = seqIncludingEle->get(i);
                    if (item->type != DOC_SCALAR) {
                        Error::addError(
                                new FieldInvalidError(filePath, item->line, item->col, "x-scenario.modules.repeat.including",
                                item->type, DOC_SCALAR)
                        );
                        delete ans;
                        return NULL;
                    } else {
                        ans->including.push_back(((DocScalarElement*)item)->getValue());
                    }
                }
            }
        }
    }
    return ans;
}