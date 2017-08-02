//
// Created by lly on 17/07/2017.
//

#ifndef VPARSER_OBJECTSCHEMA_H
#define VPARSER_OBJECTSCHEMA_H


#include "../DataSchemaObject.h"
#include "../../error/FieldMissError.h"
#include "../../error/FieldIllegalError.h"
#include "../../error/FieldInvalidError.h"
#include "../../error/IllegalIntervalError.h"

enum ObjectSerialType {
    JSON, YAML, XML
};

class ObjectSchema: public DataSchemaObject {

public:

    map<string, DataSchemaObject*> properties;
    ObjectSerialType serialType;

    ObjectSchema();
    BaseDataObject *toDataObject() override;
    bool check(BaseDataObject *obj) override;
    BaseDataObject *transform(DocElement *ele) override;
    BaseDataObject *generate() override;
    bool init(string filePath, DocObjectElement *obj, int schemaType) override;

    DataSchemaObject *findField(const vector<string> &fieldVec, int index) override;

    ~ObjectSchema() override;
};


#endif //VPARSER_OBJECTSCHEMA_H
