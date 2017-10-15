//
// Created by lly on 22/09/2017.
//

#include "ScenarioSetObject.h"
#include "../data/ObjectDataObject.h"
#include "../data/StringDataObject.h"
#include "../data/BooleanDataObject.h"
#include "../data/SequenceDataObject.h"
#include "../data/DataObjectAdapter.h"
#include "../doc/DocObjectElement.h"
#include "../error/Error.h"
#include "../error/FieldInvalidError.h"
#include "../error/FieldMissError.h"
#include "../error/FieldIllegalError.h"
#include "../doc/DocSequenceElement.h"

ScenarioSetObject::ScenarioSetObject() {
    this->name = "";
    this->unique = false;
    this->_default.clear();
}

BaseDataObject *ScenarioSetObject::toDataObject() {
    ObjectDataObject *ans = new ObjectDataObject();
    (*ans)["name"] = new StringDataObject(this->name);
    (*ans)["unique"] = new BooleanDataObject(this->unique);
    if (this->_default.size() > 0) {
        SequenceDataObject *defaultSeq = new SequenceDataObject();
        for (vector<DocElement *>::iterator ite = this->_default.begin();
             ite != this->_default.end();
             ++ite) {
            defaultSeq->push(DataObjectAdapter::defaultFromDocElement(*ite));
        }
        (*ans)["default"] = defaultSeq;
    }
    return ans;
}

ScenarioSetObject *ScenarioSetObjectFactory::create(string filePath, DocElement *ele) {
    if (ele == NULL) {
        return NULL;
    }
    if (ele->type != DOC_OBJECT) {
        Error::addError(
                new FieldInvalidError(filePath, ele->line, ele->col, "x-scenario.sets",
                                      ele->type, DOC_OBJECT)
        );
        return NULL;
    }
    DocObjectElement *objEle = (DocObjectElement*)ele;

    ScenarioSetObject *res = new ScenarioSetObject();

    /** name (required) **/
    DocElement *nameEle = objEle->get("name");
    if ((nameEle == NULL) || (nameEle->type != DOC_SCALAR)) {
        Error::addError(
                new FieldMissError(filePath, objEle->line, objEle->col, "x-scenario.sets.name")
        );
        delete res;
        return NULL;
    }
    res->name = ((DocScalarElement*)nameEle)->getValue();

    /** unique (optional) **/
    DocElement *uniqueEle = objEle->get("unique");
    if (uniqueEle != NULL) {
        pair<bool, bool> boolRes = DocElementHelper::parseToBool(uniqueEle);
        if (boolRes.second) {
            res->unique = boolRes.first;
        } else {
            Error::addError(
                    new FieldIllegalError(filePath, uniqueEle->line, uniqueEle->col, "x-scenario.sets.unique")
            );
            delete res;
            return NULL;
        }
    }

    /** default (optional) **/
    DocElement *priDefaultEle = objEle->get("default");
    if (priDefaultEle != NULL) {
        if (priDefaultEle->type != DOC_SEQUENCE) {
            Error::addError(
                    new FieldInvalidError(filePath, priDefaultEle->line, priDefaultEle->col, "x-scenario.sets.default",
                                          priDefaultEle->type, DOC_SEQUENCE)
            );
            delete res;
            return NULL;
        } else {
            DocSequenceElement *defaultEle = (DocSequenceElement*)priDefaultEle;
            int defaultCnt = defaultEle->getLength();
            for (int i=0; i<defaultCnt; ++i) {
                DocElement *now = defaultEle->get(i);
                bool exist = false;
                // unique constraint
                if (res->unique) {
                    for (vector<DocElement*>::iterator ite = res->_default.begin(); ite !=res->_default.end(); ++ite)
                        if (((*ite) != NULL) && ((*ite)->equals(now))) {
                            exist = true;
                            break;
                        }
                }
                if (!exist)
                    res->_default.push_back(now);
            }
        }
    }

    return res;
}