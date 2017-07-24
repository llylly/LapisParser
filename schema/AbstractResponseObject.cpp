//
// Created by lly on 23/07/2017.
//

#include "AbstractResponseObject.h"

BaseDataObject *AbstractResponseObject::toDataObject() {
    if (this->stage == INVALID_RESPONSE)
        return NULL;
    ObjectDataObject *obj = new ObjectDataObject();
    (*obj)["code"] = new IntegerDataObject(this->code);
    if (this->type == NORMAL || this->description != "") {
        (*obj)["description"] = new StringDataObject(this->description);
    }
    if (this->schema) {
        (*obj)["schema"] = this->schema->toDataObject();
    }
    return obj;
}

AbstractResponseObject::AbstractResponseObject(const AbstractResponseObject &source) {
    this->type = source.type;
    this->code = source.code;
    this->description = source.description;
    this->schema = source.schema;
    this->stage = source.stage;
}

AbstractResponseObject::AbstractResponseObject() {
    type = BASE_RESPONSE;
    code = -1;
    description = "";
    schema = NULL;
    stage = INVALID_RESPONSE;
}

AbstractResponseObject::~AbstractResponseObject() { }