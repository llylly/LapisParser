//
// Created by lly on 12/07/2017.
//

#ifndef VPARSER_BOOLEANDATAOBJECT_H
#define VPARSER_BOOLEANDATAOBJECT_H


#include "BaseDataObject.h"

struct BooleanDataObject: public BaseDataObject {
    bool value;

    BooleanDataObject(bool v);
    BooleanDataObject &operator=(const bool &v);
    BooleanDataObject(const BooleanDataObject &source);

    bool equals(BaseDataObject *b) override;
};


#endif //VPARSER_BOOLEANDATAOBJECT_H
