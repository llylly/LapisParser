//
// Created by lly on 12/07/2017.
//

#include "NumberDataObject.h"

NumberDataObject::NumberDataObject(double v): BaseDataObject() {
    type = NUMBER;
    value = v;
}

NumberDataObject &NumberDataObject::operator=(const double &v) {
    value = v;
    return *this;
}

NumberDataObject::NumberDataObject(const NumberDataObject &source): BaseDataObject(source) {
    this->type = NUMBER;
    this->value = source.value;
}

bool NumberDataObject::equals(BaseDataObject *b) {
    bool pre = BaseDataObject::equals(b);
    if (pre) {
        NumberDataObject *bb = (NumberDataObject*)b;
        return this->value == bb->value;
    } else
        return false;
}