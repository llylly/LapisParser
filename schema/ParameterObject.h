//
// Created by lly on 22/07/2017.
//

#ifndef VPARSER_PARAMETEROBJECT_H
#define VPARSER_PARAMETEROBJECT_H


#include <string>
#include "DataSchemaObject.h"
#include "DataSchemaPool.h"

using namespace std;

enum IN_TYPE {
    IN_INVALID, QUERY, HEADER, PATH, FORMDATA, BODY, RESPONSE_HEADER
};

class ParameterObject: public BaseObject {
public:
    ParameterObject();
    ~ParameterObject();

    BaseDataObject* toDataObject() override;

    bool valid;
    string name;
    IN_TYPE in;
    string description;
    bool required;
    double nullProbability;
    DataSchemaObject *schema;

    bool schemaInPool;

};

struct ParameterObjectFactory {
    static ParameterObject *create(string filePath, DocElement *ele, DataSchemaPool *schemaPool, bool isResponseHeader = false);
};

#endif //VPARSER_PARAMETEROBJECT_H
