//
// Created by lly on 29/05/2017.
//

#ifndef VPARSER_PATHPOOL_H
#define VPARSER_PATHPOOL_H

#include "ResponsePool.h"
#include "ParameterPool.h"
#include "APIObject.h"
#include "SchemesObject.h"

class APIPool {
/**
 * This class processes all the Operation Object of scripts
 * Operation Object are indexed by their name and request method
 * Similar to DataSchemaPool
 */

public:
    /**
     * Constructor
     */
    APIPool(ParameterPool* paramPool, ResponsePool* responsePool);

    /**
     * Destructor
     */
    ~APIPool();

    /**
     * Parse a doc node which represents a complete Operation Object
     *  Return whether successfully parsed
     *  If it has parsed, also return the object, and increment the ref count
     * @param filePath: file name of the DocElement
     * @param ele: the DocElement to be parsed
     * @param name: API name(for indexing)
     * @param method: request method(for indexing)
     * @param rootSchemes: the schemes object of the root node, to be appended here
     * @param commonParams: the parameter field of the outer node, to be appended here
     * @return
     */
    APIObject *parseAPI(string filePath, DocElement *ele, string name, APIRequestMethod method, SchemesObject *rootSchemes, vector<ParameterObject*> *commonParams);


private:
    ParameterPool *paramPool;
    ResponsePool *responsePool;

    set<pair<string, APIRequestMethod>> APINameSet;
    set<DocElement*> APINodeSet;
    map<DocElement*, pair<string, APIRequestMethod>> nodeNameMap;
    map<APIObject*, pair<string, APIRequestMethod>> objNameMap;
    map<pair<string, APIRequestMethod>, APIObject*> pool;
    map<pair<string, APIRequestMethod>, int> refMap;

};


#endif //VPARSER_PATHPOOL_H
