//
// Created by lly on 23/07/2017.
//

#ifndef VPARSER_RESPONSEOBJECT_H
#define VPARSER_RESPONSEOBJECT_H

#include "../AbstractResponseObject.h"
#include "../ParameterObject.h"

class ResponseObject: public AbstractResponseObject {

public:
    ResponseObject();
    ResponseObject(const ResponseObject &source);
    virtual BaseDataObject *toDataObject();
    bool createRaw(string filePath, DocObjectElement *ele, DataSchemaPool *pool) override;
    bool checkResponse(DocElement *data) override;
    virtual ~ResponseObject() override;

    map<string, ParameterObject*> headers;
};


#endif //VPARSER_RESPONSEOBJECT_H
