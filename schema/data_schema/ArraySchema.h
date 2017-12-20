//
// Created by lly on 17/07/2017.
//

#ifndef VPARSER_ARRAYSCHEMA_H
#define VPARSER_ARRAYSCHEMA_H


#include "../DataSchemaObject.h"
#include "../../error/FieldMissError.h"
#include "../../error/FieldIllegalError.h"
#include "../../error/FieldInvalidError.h"
#include "../../error/IllegalIntervalError.h"

enum ArrayCollectionFormat {
    CSV, SSV, TSV, PIPES, MULTI
};

class ArraySchema: public DataSchemaObject {
public:

    DataSchemaObject *items;
    ArrayCollectionFormat collectionFormat;
    int maxItems;
    int minItems;
    bool uniqueItems;

    ArraySchema();
    BaseDataObject *toDataObject() override;
    bool check(BaseDataObject *obj) override;
    BaseDataObject *transform(DocElement *ele) override;
    BaseDataObject *generate() override;
    bool init(string filePath, DocObjectElement *obj, int schemaType, ObjectSerialType inherentType) override;

    DataSchemaObject *findField(const vector<string> &fieldVec, int index) override;

    ~ArraySchema() override;
};


#endif //VPARSER_ARRAYSCHEMA_H
