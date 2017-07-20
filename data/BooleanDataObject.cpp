//
// Created by lly on 12/07/2017.
//

#include "BooleanDataObject.h"

BooleanDataObject::BooleanDataObject(bool v): BaseDataObject() {
    type = BOOLEAN;
    value = v;
}

BooleanDataObject& BooleanDataObject::operator=(const bool &v) {
    value = v;
    return *this;
}

BooleanDataObject::BooleanDataObject(const BooleanDataObject &source): BaseDataObject(source) {
    type = BOOLEAN;
    value = source.value;
}

bool BooleanDataObject::equals(BaseDataObject* b) {
    bool pre = BaseDataObject::equals(b);
    if (pre) {
        BooleanDataObject *bb = (BooleanDataObject*)b;
        return this->value == bb->value;
    } else
        return false;
}