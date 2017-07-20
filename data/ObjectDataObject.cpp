//
// Created by lly on 12/07/2017.
//

#include "ObjectDataObject.h"
#include "DataObjectAdapter.h"

ObjectDataObject::ObjectDataObject(): BaseDataObject() {
    type = OBJECT;
    m.clear();
}

ObjectDataObject::~ObjectDataObject() {
    for (map<string, BaseDataObject*>::iterator ite = m.begin(); ite != m.end(); ++ite) {
        delete ite->second;
    }
}

BaseDataObject *&ObjectDataObject::operator[](string s) {
    return this->m[s];
}

bool ObjectDataObject::hasKey(string s) {
    return this->m.count(s) > 0;
}

bool ObjectDataObject::erase(string s) {
    return this->m.erase(s) > 0;
}

int ObjectDataObject::size() {
    return (int)this->m.size();
}

ObjectDataObject::ObjectDataObject(const ObjectDataObject &source): BaseDataObject(source) {
    this->type = OBJECT;
    this->m.clear();
    for (map<string, BaseDataObject*>::const_iterator ite = source.m.begin(); ite != source.m.end(); ++ite) {
        this->m[ite->first] = DataObjectAdapter::deepCopy(ite->second);
    }
}

bool ObjectDataObject::equals(BaseDataObject *b) {
    bool pre = BaseDataObject::equals(b);
    if (pre) {
        ObjectDataObject *bb = (ObjectDataObject*)b;
        bool eq = true;
        for (map<string, BaseDataObject*>::iterator ite = this->m.begin();
                ite != this->m.end(); ++ite) {
            if (bb->m.count(ite->first)) {
                eq &= ite->second->equals(bb->m[ite->first]);
            } else
                eq = false;
        }
        return eq;
    } else
        return false;
}