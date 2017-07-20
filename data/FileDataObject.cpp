//
// Created by lly on 12/07/2017.
//

#include "FileDataObject.h"

FileDataObject::FileDataObject(unsigned char* data, int len): BaseDataObject() {
    type = BYTEFILE;
    this->data = data;
    this->len = len;
}

FileDataObject::FileDataObject(const FileDataObject &source): BaseDataObject(source) {
    type = BYTEFILE;
    this->data = source.data;
    this->len = source.len;
}

bool FileDataObject::equals(BaseDataObject *b) {
    bool pre = BaseDataObject::equals(b);
    if (pre) {
        FileDataObject *bb = (FileDataObject*)b;
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