//
// Created by lly on 12/07/2017.
//

#ifndef VPARSER_INTEGERDATAOBJECT_H
#define VPARSER_INTEGERDATAOBJECT_H


#include "BaseDataObject.h"

struct IntegerDataObject: public BaseDataObject {

    long long value;

    IntegerDataObject(long long v = 0);
    IntegerDataObject &operator=(const long long &v);

    IntegerDataObject(const IntegerDataObject &source);

    bool equals(BaseDataObject *b) override;
};


#endif //VPARSER_INTEGERDATAOBJECT_H
