//
// Created by lly on 12/07/2017.
//

#include "SequenceDataObject.h"
#include "DataObjectAdapter.h"

SequenceDataObject::SequenceDataObject() {
    type = SEQUENCE;
    vec.clear();
}

SequenceDataObject::~SequenceDataObject() {
    for (vector<BaseDataObject*>::iterator ite = vec.begin(); ite != vec.end(); ++ite)
        delete *ite;
}

BaseDataObject *&SequenceDataObject::operator[](int index) {
    return this->vec[index];
}

int SequenceDataObject::length() {
    return (int)(this->vec.size());
}

void SequenceDataObject::pop() {
    this->vec.pop_back();
}

void SequenceDataObject::push(BaseDataObject *ele) {
    this->vec.push_back(ele);
}

SequenceDataObject::SequenceDataObject(const SequenceDataObject &source): BaseDataObject(source) {
    this->type = SEQUENCE;
    this->vec.clear();
    for (vector<BaseDataObject*>::const_iterator ite = source.vec.begin(); ite != source.vec.end(); ++ite) {
        this->vec.push_back(DataObjectAdapter::deepCopy(*ite));
    }
}

bool SequenceDataObject::equals(BaseDataObject *b) {
    bool pre = BaseDataObject::equals(b);
    if (pre) {
        SequenceDataObject *bb = (SequenceDataObject*)b;
        bool eq = this->length() == bb->length();
        if (!eq) return false;
        for (int i=0; i<this->length(); ++i)
            eq &= this->vec[i]->equals(bb->vec[i]);
        return eq;
    } else
        return false;
}