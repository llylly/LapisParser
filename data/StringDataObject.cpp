//
// Created by lly on 12/07/2017.
//

#include "StringDataObject.h"

StringDataObject::StringDataObject(string s): BaseDataObject() {
    type = STRING;
    str = s;
}

StringDataObject::StringDataObject(const StringDataObject &source): BaseDataObject(source) {
    this->type = STRING;
    this->str = source.str;
}

bool StringDataObject::equals(BaseDataObject *b) {
    bool pre = BaseDataObject::equals(b);
    if (pre) {
        StringDataObject *bb = (StringDataObject*)b;
        return this->str == bb->str;
    } else
        return false;
}