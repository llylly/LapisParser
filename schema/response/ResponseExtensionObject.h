//
// Created by lly on 23/07/2017.
//

#ifndef VPARSER_RESPONSEEXTENSIONOBJECT_H
#define VPARSER_RESPONSEEXTENSIONOBJECT_H


#include "../AbstractResponseObject.h"

class ResponseExtensionObject: public AbstractResponseObject {

public:
    ResponseExtensionObject();
    ResponseExtensionObject(const ResponseExtensionObject &source);
    BaseDataObject* toDataObject() override;
    bool createRaw(string filePath, DocObjectElement *ele, DataSchemaPool *pool) override;
    bool checkResponse(DocElement *data) override;

    string name;
    string field;
    vector<vector<string>> relatedParameters;

    vector<string> fieldVec;
};


#endif //VPARSER_RESPONSEEXTENSIONOBJECT_H
