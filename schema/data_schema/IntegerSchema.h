//
// Created by lly on 17/07/2017.
//

#ifndef VPARSER_INTEGERSCHEMA_H
#define VPARSER_INTEGERSCHEMA_H


#include "../DataSchemaObject.h"
#include "../../error/IllegalIntervalError.h"

class IntegerSchema: public DataSchemaObject {

public:

    long long maximum, minimum;
    bool exclusiveMaximum, exclusiveMinimum;
    bool hasMultipleOf;
    long long multipleOf;

    IntegerSchema();
    BaseDataObject *toDataObject() override;
    bool check(BaseDataObject *obj) override;
    BaseDataObject *transform(DocElement *ele) override;
    BaseDataObject *generate() override;
    bool init(string filePath, DocObjectElement *obj, int schemaType) override;
};


#endif //VPARSER_INTEGERSCHEMA_H
