//
// Created by lly on 22/09/2017.
//

#include "ScenarioInfoObject.h"
#include "../data/StringDataObject.h"
#include "../data/ObjectDataObject.h"
#include "../error/FieldInvalidError.h"
#include "../doc/DocObjectElement.h"
#include "../doc/DocScalarElement.h"
#include "../error/FieldMissError.h"

ScenarioInfoObject::ScenarioInfoObject() {
    this->title = "";
    this->summary = "";
    this->description = "";
    this->version = "";
}

BaseDataObject *ScenarioInfoObject::toDataObject() {
    ObjectDataObject *obj = new ObjectDataObject();
    (*obj)["title"] = new StringDataObject(this->title);
    if (this->summary != "") {
        (*obj)["summary"] = new StringDataObject(this->summary);
    }
    if (this->description != "") {
        (*obj)["description"] = new StringDataObject(this->description);
    }
    (*obj)["version"]= new StringDataObject(this->version);
    return obj;
}

ScenarioInfoObject *ScenarioInfoObjectFactory::create(string filePath, DocElement *ele) {
    if (ele == NULL) {
        return NULL;
    }
    if (ele->type != DOC_OBJECT) {
        Error::addError(
                new FieldInvalidError(filePath, ele->line, ele->col, "x-scenario.info",
                                      ele->type, DOC_OBJECT)
        );
        return NULL;
    }
    DocObjectElement *objEle = (DocObjectElement*)ele;

    ScenarioInfoObject *res = new ScenarioInfoObject();

    /** title (required) **/
    DocElement *titleEle = objEle->get("title");
    if ((titleEle == NULL) || (titleEle->type != DOC_SCALAR)) {
        Error::addError(
                new FieldMissError(filePath, objEle->line, objEle->col, "x-scenario.info.title")
        );
        delete res;
        return NULL;
    }
    res->title = ((DocScalarElement*)titleEle)->getValue();

    /** summary (optional) **/
    DocElement *summaryEle = objEle->get("summary");
    if (summaryEle != NULL) {
        if (summaryEle->type != DOC_SCALAR) {
            Error::addError(
                    new FieldInvalidError(filePath, summaryEle->line, summaryEle->col, "x-scenario.info.summmary",
                                          summaryEle->type, DOC_SCALAR)
            );
            delete res;
            return NULL;
        } else {
            res->summary = ((DocScalarElement*)summaryEle)->getValue();
        }
    }

    /** description (optional) **/
    DocElement *desEle = objEle->get("description");
    if (desEle != NULL) {
        if (desEle->type != DOC_SCALAR) {
            Error::addError(
                    new FieldInvalidError(filePath, desEle->line, desEle->col, "x-scenario.info.description",
                                          desEle->type, DOC_SCALAR)
            );
            delete res;
            return NULL;
        } else {
            res->description = ((DocScalarElement*)desEle)->getValue();
        }
    }

    /** version (required) **/
    DocElement *verEle = objEle->get("version");
    if ((verEle == NULL) || (verEle->type != DOC_SCALAR)) {
        Error::addError(
                new FieldMissError(filePath, objEle->line, objEle->col, "x-scenario.version")
        );
        delete res;
        return NULL;
    }
    res->version = ((DocScalarElement*)verEle)->getValue();

    return res;
}