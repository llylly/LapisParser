//
// Created by lly on 26/07/2017.
//

#ifndef VPARSER_APIOBJECT_H
#define VPARSER_APIOBJECT_H

#include <string>
#include <vector>
#include "BaseObject.h"
#include "ExternalDocObject.h"
#include "ParameterObject.h"
#include "response/ResponseObject.h"
#include "response/ResponseExtensionObject.h"
#include "SchemesObject.h"
#include "ResponsePool.h"
#include "ParameterPool.h"

using namespace std;

enum APIRequestMethod {
    GET, POST
};

class APIConstraintObject;

class APIObject: public BaseObject {
public:

    /**
     * Constructor
     */
    APIObject();

    /**
     * Destructor
     */
    ~APIObject();

    /**
     * Parse a object doc element to APIObject
     *  Return whether successfully parsed
     * @param filePath: file name of the DocObjectElement
     * @param ele the DocObjectElement to be parsed
     * @param rootSchemes: the schemes object of the root node, to be appended here
     * @param commonParams: the parameter field of the outer node, to be appended here
     * @param paramPool: the parameter pool, used to parse parameters
     * @param responsePool: the response pool, used to parse responses
     * @return bool: whether successfully parsed
     */
    bool create(string filePath, DocObjectElement *ele, SchemesObject *rootSchemes, vector<ParameterObject*> *commonParams,
        ParameterPool *paramPool, ResponsePool *responsePool);

    /**
     * Output interface
     * @return
     */
    BaseDataObject* toDataObject() override;


    bool valid;
    vector<string> tags;
    string summary;
    string description;
    ExternalDocObject *externalDocs;
    /** path can be overwritten by operationID field **/
    string path;
    /** name is the path, i.e., the key of this item **/
    string name;
    APIRequestMethod requestMethod;
    vector<string> consumes;
    vector<string> produces;
    map<string, ParameterObject*> parameters;
    map<string, ResponseObject*> responses;
    map<string, vector<ResponseExtensionObject*>> responseExtensions;
    SchemesObject *schemes;
    bool deprecated;
    APIConstraintObject *constraint;
};


#endif //VPARSER_APIOBJECT_H
