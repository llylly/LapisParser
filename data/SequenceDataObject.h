//
// Created by lly on 12/07/2017.
//

#ifndef VPARSER_SEQUENCEDATAOBJECT_H
#define VPARSER_SEQUENCEDATAOBJECT_H

#include <vector>
#include "BaseDataObject.h"

using namespace std;

struct SequenceDataObject: public BaseDataObject {

    vector<BaseDataObject*> vec;

    SequenceDataObject();
    ~SequenceDataObject() override;

    BaseDataObject *&operator[](int index);
    int length();
    void pop();
    void push(BaseDataObject *ele);

    SequenceDataObject(const SequenceDataObject &source);

    bool equals(BaseDataObject *b) override;
};


#endif //VPARSER_SEQUENCEDATAOBJECT_H
