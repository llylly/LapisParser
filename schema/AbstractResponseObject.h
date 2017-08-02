//
// Created by lly on 23/07/2017.
//

#ifndef VPARSER_ABSTRACTRESPONSEOBJECT_H
#define VPARSER_ABSTRACTRESPONSEOBJECT_H


#include "BaseObject.h"
#include "DataSchemaObject.h"
#include "DataSchemaPool.h"

enum ResponseClassType {
    BASE_RESPONSE, NORMAL, EXTENSION
};

enum ResponseStage {
    INVALID_RESPONSE, RAW, VALID
};

class AbstractResponseObject: public BaseObject {

public:
    AbstractResponseObject(const AbstractResponseObject &source);
    virtual BaseDataObject* toDataObject();
    virtual bool createRaw(string filePath, DocObjectElement *ele, DataSchemaPool *pool) = 0;
    /**
     * Check whether the response data conforms to the response schema
     * CAUTION: we don't check the response code here
     * @param data
     * @return True or False: conform or not
     */
    virtual bool checkResponse(DocElement *data) = 0;
    virtual ~AbstractResponseObject();


    ResponseClassType type;

    int code;
    string description;
    DataSchemaObject *schema;

    ResponseStage stage;

protected:
    AbstractResponseObject();

};


#endif //VPARSER_ABSTRACTRESPONSEOBJECT_H
