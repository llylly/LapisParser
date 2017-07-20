//
// Created by lly on 17/07/2017.
//

#ifndef VPARSER_NUMBERSCHEMA_H
#define VPARSER_NUMBERSCHEMA_H

#define EPS 1e-6

#include <cfloat>
#include <cmath>
#include "../DataSchemaObject.h"
#include "../../error/IllegalIntervalError.h"

class NumberSchema: public DataSchemaObject {

public:

    double maximum, minimum;
    bool exclusiveMaximum, exclusiveMinimum;
    bool hasMultipleOf;
    double multipleOf;

    NumberSchema();
    BaseDataObject *toDataObject() override;
    bool check(BaseDataObject *obj) override;
    BaseDataObject *transform(DocElement *ele) override;
    BaseDataObject *generate() override;
    bool init(string filePath, DocObjectElement *obj, int schemaType) override;
};


#endif //VPARSER_NUMBERSCHEMA_H
