//
// Created by lly on 12/07/2017.
//

#include "BaseDataObject.h"

BaseDataObject::BaseDataObject() {
    isTop = false;
    schema = NULL;
    type = BASE;
}

BaseDataObject::BaseDataObject(const BaseDataObject &source) {
    isTop = source.isTop;
    schema = source.schema;
    type = source.type;
}

bool BaseDataObject::equals(BaseDataObject *b) {
    return (b) && (b->type == this->type);
}

BaseDataObject::~BaseDataObject() { }