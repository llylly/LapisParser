//
// Created by lly on 12/07/2017.
//

#ifndef VPARSER_OBJECTDATAOBJECT_H
#define VPARSER_OBJECTDATAOBJECT_H


#include <map>
#include <string>
#include "BaseDataObject.h"

using namespace std;

struct ObjectDataObject: public BaseDataObject {

    map<string, BaseDataObject*> m;

    ObjectDataObject();

    ~ObjectDataObject() override;

    BaseDataObject *&operator[](string s);
    bool hasKey(string s);
    bool erase(string s);
    int size();

    ObjectDataObject(const ObjectDataObject &source);

    bool equals(BaseDataObject *b) override;
};


#endif //VPARSER_OBJECTDATAOBJECT_H
