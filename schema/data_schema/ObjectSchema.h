//
// Created by lly on 17/07/2017.
//

#ifndef VPARSER_OBJECTSCHEMA_H
#define VPARSER_OBJECTSCHEMA_H


#include "../DataSchemaObject.h"

class ObjectSchema: public DataSchemaObject {

public:
    ObjectSchema();
    BaseDataObject *toDataObject() override;
    bool check(BaseDataObject *obj) override;
    BaseDataObject *transform(DocElement *ele) override;
    BaseDataObject *generate() override;
    bool init(string filePath, DocObjectElement *obj, int schemaType) override;
};


#endif //VPARSER_OBJECTSCHEMA_H
