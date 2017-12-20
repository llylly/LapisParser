//
// Created by lly on 17/07/2017.
//

#ifndef VPARSER_FILESCHEMA_H
#define VPARSER_FILESCHEMA_H


#include "../DataSchemaObject.h"

class FileSchema: public DataSchemaObject {
/**
 * There's no extra fields and extra constraints for 'file' type
 * So many functions do nothing :)
 */

public:
    FileSchema();
    BaseDataObject *toDataObject() override;
    bool check(BaseDataObject *obj) override;
    BaseDataObject *transform(DocElement *ele) override;
    BaseDataObject *generate() override;
    bool init(string filePath, DocObjectElement *obj, int schemaType, ObjectSerialType inherentType) override;

    DataSchemaObject *findField(const vector<string> &fieldVec, int index) override;
};


#endif //VPARSER_FILESCHEMA_H
