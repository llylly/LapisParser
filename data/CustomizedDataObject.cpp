//
// Created by lly on 12/07/2017.
//

#include "CustomizedDataObject.h"

CustomizedDataObject::CustomizedDataObject(unsigned char *data, int len): BaseDataObject() {
    type = CUSTOMIZED;
    this->data = new unsigned char[len];
    memcpy((void*)this->data, (const void*)data, (size_t)len);
    this->len = len;
}

CustomizedDataObject::CustomizedDataObject(const CustomizedDataObject &source): BaseDataObject(source) {
    type = CUSTOMIZED;
    this->data = new unsigned char[len];
    memcpy((void*)this->data, (const void*)source.data, (size_t)source.len);
    this->len = source.len;
}

bool CustomizedDataObject::equals(BaseDataObject *b) {
    bool pre = BaseDataObject::equals(b);
    if (pre) {
        CustomizedDataObject *bb = (CustomizedDataObject*)b;
        bool eq = true;
        for (int i=0; i<this->len; ++i) {
            if (bb->data[i] != this->data[i]) {
                eq = false;
                break;
            }
        }
        return eq;
    } else
        return false;
}

CustomizedDataObject::~CustomizedDataObject() {
    delete this->data;
}