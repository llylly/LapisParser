//
// Created by lly on 12/07/2017.
//

#ifndef VPARSER_NUMBERDATAOBJECT_H
#define VPARSER_NUMBERDATAOBJECT_H


#include "BaseDataObject.h"

struct NumberDataObject: public BaseDataObject {

    double value;

    NumberDataObject(double v = 0.0);
    NumberDataObject &operator=(const double &v);
    NumberDataObject(const NumberDataObject &source);

    bool equals(BaseDataObject *b) override;
};


#endif //VPARSER_NUMBERDATAOBJECT_H
