//
// Created by lly on 17/07/2017.
//

#ifndef VPARSER_BOOLEANSCHEMA_H
#define VPARSER_BOOLEANSCHEMA_H


#include "../DataSchemaObject.h"

class BooleanSchema: public DataSchemaObject {

public:

    string trueString;
    string falseString;

    BooleanSchema();
    BaseDataObject *toDataObject() override;
    bool check(BaseDataObject *obj) override;
    BaseDataObject *transform(DocElement *ele) override;
    BaseDataObject *generate() override;
    bool init(string filePath, DocObjectElement *obj, int schemaType) override;

    DataSchemaObject *findField(const vector<string> &fieldVec, int index) override;
};

#endif //VPARSER_BOOLEANSCHEMA_H
