//
// Created by lly on 22/09/2017.
//

#include "ScenarioStateObject.h"
#include "../error/FieldInvalidError.h"
#include "../doc/DocObjectElement.h"
#include "../doc/DocScalarElement.h"
#include "../data/ObjectDataObject.h"
#include "../data/StringDataObject.h"
#include "../data/BooleanDataObject.h"
#include "../error/FieldIllegalError.h"
#include "../error/FieldMissError.h"

ScenarioStateObject::ScenarioStateObject() {
    this->name = "";
    this->description = "";
    this->isInitial = false;
}

BaseDataObject *ScenarioStateObject::toDataObject() {
    ObjectDataObject *ans = new ObjectDataObject();
    (*ans)["name"] = new StringDataObject(this->name);
    if (this->description != "") {
        (*ans)["description"] = new StringDataObject(this->description);
    }
    (*ans)["isInitial"] = new BooleanDataObject(this->isInitial);
    return ans;
}

ScenarioStateObject *ScenarioStateObjectFactory::create(string filePath, DocElement *ele) {
    if (ele == NULL) {
        return NULL;
    }
    if (ele->type != DOC_OBJECT) {
        Error::addError(
                new FieldInvalidError(filePath, ele->line, ele->col, "x-scenario.states",
                                      ele->type, DOC_OBJECT)
        );
        return NULL;
    }
    DocObjectElement *objEle = (DocObjectElement*)ele;

    ScenarioStateObject *res = new ScenarioStateObject();

    /** name (required) **/
    DocElement *nameEle = objEle->get("name");
    if ((nameEle == NULL) || (nameEle->type != DOC_SCALAR)) {
        Error::addError(
                new FieldMissError(filePath, objEle->line, objEle->col, "x-scenario.states.name")
        );
        delete res;
        return NULL;
    }
    res->name = ((DocScalarElement*)nameEle)->getValue();

    /** description (optional) **/
    DocElement *desEle = objEle->get("description");
    if (desEle != NULL) {
        if (desEle->type != DOC_SCALAR) {
            Error::addError(
                    new FieldInvalidError(filePath, desEle->line, desEle->col, "x-scenario.states.description",
                                          desEle->type, DOC_SCALAR)
            );
            delete res;
            return NULL;
        } else {
            res->description = ((DocScalarElement*)desEle)->getValue();
        }
    }

    /** isInitial (optional) **/
    DocElement *isiniEle = objEle->get("isInitial");
    if (isiniEle !=NULL) {
        pair<bool, bool> boolRes = DocElementHelper::parseToBool(isiniEle);
        if (boolRes.second) {
            res->isInitial = boolRes.first;
        } else {
            Error::addError(
                    new FieldIllegalError(filePath, isiniEle->line, isiniEle->col, "x-scenario.states.isInitial")
            );
            delete res;
            return NULL;
        }
    }

    return res;
}