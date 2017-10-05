//
// Created by lly on 03/10/2017.
//

#include "ScenarioConnectionObject.h"

ScenarioConnectionObject::ScenarioConnectionObject() {
    this->from = "";
    this->to = "";
    this->including.clear();
    this->excluding.clear();
    this->weigh = 1.0;
}

ScenarioConnectionObject::~ScenarioConnectionObject() {
    this->including.clear();
    this->excluding.clear();
}

BaseDataObject *ScenarioConnectionObject::toDataObject() {
    ObjectDataObject *obj = new ObjectDataObject();
    (*obj)["from"] = new StringDataObject(this->from);
    (*obj)["to"] = new StringDataObject(this->to);
    if (this->including.size() > 0) {
        SequenceDataObject *seq = new SequenceDataObject();
        for (vector<string>::iterator ite = this->including.begin();
                ite != this->including.end();
                ++ite) {
            seq->push(new StringDataObject(*ite));
        }
        (*obj)["including"] = seq;
    }
    if (this->excluding.size() > 0) {
        SequenceDataObject *seq = new SequenceDataObject();
        for (vector<string>::iterator ite = this->excluding.begin();
                ite != this->excluding.end();
                ++ite) {
            seq->push(new StringDataObject(*ite));
        }
        (*obj)["excluding"] = seq;
    }
    (*obj)["weigh"] = new NumberDataObject(this->weigh);
    return obj;
}

ScenarioConnectionObject *ScenarioConnectionObjectFactory::create(string filePath, DocElement *ele) {
    if (ele == NULL) return NULL;
    if (ele->type != DOC_OBJECT) {
        Error::addError(new FieldInvalidError(filePath, ele->line, ele->col, "x-scenario.connections",
                                              ele->type, DOC_OBJECT));
        return NULL;
    }

    ScenarioConnectionObject *res = new ScenarioConnectionObject();
    DocObjectElement *objEle = (DocObjectElement*)ele;

    /** from (required) **/
    DocElement *fromEle = objEle->get("from");
    if (fromEle == NULL) {
        Error::addError(new FieldMissError(filePath, ele->line, ele->col, "x-scenario.connections.from"));
        delete res;
        return NULL;
    } else {
        pair<string, bool> parseRes = DocElementHelper::parseToString(fromEle);
        if (!parseRes.second) {
            Error::addError(new FieldInvalidError(filePath, fromEle->line, fromEle->col, "x-scenario.connections.from",
            fromEle->type, DOC_SCALAR));
            delete res;
            return NULL;
        } else {
            res->from = parseRes.first;
        }
    }

    /** to (required) **/
    DocElement *toEle = objEle->get("to");
    if (toEle == NULL) {
        Error::addError(new FieldMissError(filePath, ele->line, ele->col, "x-scenario.connections.to"));
        delete res;
        return NULL;
    } else {
        pair<string, bool> parseRes = DocElementHelper::parseToString(toEle);
        if (!parseRes.second) {
            Error::addError(new FieldInvalidError(filePath, toEle->line, toEle->col, "x-scenario.connections.to",
            toEle->type, DOC_SCALAR));
            delete res;
            return NULL;
        } else {
            res->to = parseRes.first;
        }
    }

    /** including (optional) **/
    DocElement *includingEle = objEle->get("including");
    if (includingEle != NULL) {
        if (includingEle->type != DOC_SEQUENCE) {
            Error::addError(
                    new FieldInvalidError(filePath, includingEle->line, includingEle->col, "x-scenario.connections.including",
                                          includingEle->type, DOC_SEQUENCE)
            );
            delete res;
            return NULL;
        } else {
            DocSequenceElement *seqIncludingEle = (DocSequenceElement*)includingEle;
            int len = seqIncludingEle->getLength();
            for (int i=0; i<len; ++i) {
                DocElement *item = seqIncludingEle->get(i);
                if (item->type != DOC_SCALAR) {
                    Error::addError(
                            new FieldInvalidError(filePath, item->line, item->col, "x-scenario.connections.including",
                                                  item->type, DOC_SCALAR)
                    );
                    delete res;
                    return NULL;
                } else {
                    res->including.push_back(((DocScalarElement*)item)->getValue());
                }
            }
        }
    }

    /** excluding (optional) **/
    DocElement *excludingEle = objEle->get("excluding");
    if (excludingEle != NULL) {
        if (excludingEle->type != DOC_SEQUENCE) {
            Error::addError(
                    new FieldInvalidError(filePath, excludingEle->line, excludingEle->col, "x-scenario.connections.excluding",
                                          excludingEle->type, DOC_SEQUENCE)
            );
            delete res;
            return NULL;
        } else {
            DocSequenceElement *seqExcludingEle = (DocSequenceElement*)excludingEle;
            int len = seqExcludingEle->getLength();
            for (int i=0; i<len; ++i) {
                DocElement *item = seqExcludingEle->get(i);
                if (item->type != DOC_SCALAR) {
                    Error::addError(
                            new FieldInvalidError(filePath, item->line, item->col, "x-scenario.connections.excluding",
                                                  item->type, DOC_SCALAR)
                    );
                    delete res;
                    return NULL;
                } else {
                    res->excluding.push_back(((DocScalarElement*)item)->getValue());
                }
            }
        }
    }

    /* weigh (optional) **/
    DocElement *weighEle = objEle->get("weigh");
    if (weighEle != NULL) {
        pair<double, bool> parseRes = DocElementHelper::parseToDouble(weighEle);
        if (!parseRes.second) {
            Error::addError(
                    new FieldIllegalError(filePath, weighEle->line, weighEle->col, "x-scenario.modules.connections.weigh")
            );
            delete res;
            return NULL;
        } else {
            res->weigh = parseRes.first;
        }
    }

    return res;
}
