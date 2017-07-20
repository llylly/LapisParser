//
// Created by lly on 29/05/2017.
//

#include "ExternalDocObject.h"
#include "../error/Error.h"
#include "../error/FieldInvalidError.h"
#include "../error/FieldMissError.h"
#include "../doc/DocScalarElement.h"
#include "../data/ObjectDataObject.h"
#include "../data/StringDataObject.h"

ExternalDocObject::ExternalDocObject() {
    this->hasDescription = false;
}

ExternalDocObject * ExternalDocObjectFactory::create(string filePath, string nodeName, DocObjectElement *ele) {
    ExternalDocObject *obj = new ExternalDocObject();
    obj->hasDescription = false;
    DocElement *url = ele->get("url");
    if (url == NULL) {
        Error::addError(new FieldMissError(filePath, ele->line, ele->col, nodeName + ".url"));
        delete obj;
        return NULL;
    }
    if (url->type != DOC_SCALAR) {
        Error::addError(new FieldInvalidError(filePath, url->line, url->col, nodeName + ".url", url->type, DOC_SCALAR));
        delete obj;
        return NULL;
    }
    obj->url = ((DocScalarElement*)url)->getValue();
    DocElement *description = ele->get("description");
    if (description) {
        if (description->type != DOC_SCALAR) {
            Error::addError(new FieldInvalidError(filePath, description->line, description->col, nodeName + ".description", url->type, DOC_SCALAR));
            delete obj;
            return NULL;
        } else {
            obj->hasDescription = true;
            obj->description = ((DocScalarElement*)description)->getValue();
        }
    }
    return obj;
}

BaseDataObject *ExternalDocObject::toDataObject() {
    ObjectDataObject *obj = new ObjectDataObject();
    (*obj)["url"] = new StringDataObject(this->url);
    if (this->hasDescription)
        (*obj)["description"] = new StringDataObject(this->description);
    return obj;
}