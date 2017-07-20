//
// Created by lly on 17/07/2017.
//

#ifndef VPARSER_CUSTOMIZEDSCHEMA_H
#define VPARSER_CUSTOMIZEDSCHEMA_H


#include "../DataSchemaObject.h"

class CustomizedSchema: public DataSchemaObject {

public:
    CustomizedSchema();
    BaseDataObject *toDataObject() override;
    bool check(BaseDataObject *obj) override;
    BaseDataObject *transform(DocElement *ele) override;
    BaseDataObject *generate() override;
    bool init(string filePath, DocObjectElement *obj, int schemaType) override;
};


#endif //VPARSER_CUSTOMIZEDSCHEMA_H
