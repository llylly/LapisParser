//
// Created by lly on 23/07/2017.
//

#ifndef VPARSER_RESPONSEOBJECT_H
#define VPARSER_RESPONSEOBJECT_H

#include "../AbstractResponseObject.h"

class ResponseObject: public AbstractResponseObject {

public:
    ResponseObject();
    ResponseObject(const ResponseObject &source);
    bool createRaw(string filePath, DocObjectElement *ele, DataSchemaPool *pool) override;
    bool checkResponse(DocElement *data) override;
};


#endif //VPARSER_RESPONSEOBJECT_H
