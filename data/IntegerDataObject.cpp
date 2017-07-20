//
// Created by lly on 12/07/2017.
//

#include "IntegerDataObject.h"

IntegerDataObject::IntegerDataObject(long long v): BaseDataObject() {
    type = INTEGER;
    value = v;
}

IntegerDataObject &IntegerDataObject::operator=(const long long &v) {
    value = v;
    return *this;
}

IntegerDataObject::IntegerDataObject(const IntegerDataObject &source): BaseDataObject(source) {
    type = INTEGER;
    this->value = source.value;
}

bool IntegerDataObject::equals(BaseDataObject *b) {
    bool pre = BaseDataObject::equals(b);
    if (pre) {
        IntegerDataObject *bb = (IntegerDataObject*)b;
        return this->value == bb->value;
    } else
        return false;
}