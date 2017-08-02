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

    /**
     * Inspect the existence by doc node
     * @param ele: doc node
     * @return bool: exist or not
     */
    bool touch(DocElement *ele);

    /**
     * Get the API Name & Method by the doc element
     * @param ele: the doc element
     * @return a pair composed of the name and method of the API;
     *  If not exist, return <"", GET>
     */
    pair<string, APIRequestMethod> getNameByElement(DocElement* ele);

    /**
     * CAUTION: Time-Consuming!
     *  CAN BE OPTIMIZED if needed
     * Get the doc element from API Name & Method
     * @param name: the pair of API Name & Method
     * @return the doc element
     *  If not exist, return NULL
     */
    DocElement *getElementByName(pair<string, APIRequestMethod> name);

    /**
     * Get the API object by the doc element
     * @param name: the pair of API Name & Method
     * @return a pointer of the API Object;
     *   If not exist, return NULL
     */
    APIObject *getObjectByName(pair<string, APIRequestMethod> name);

    /**
     * Get the API Object by the doc element
     * @param ele: the doc element
     * @return a pointer of the API Object;
     *  If not exist, return NULL
     */
    APIObject *getObjectByElement(DocElement *ele);

    /**
     * Get the pool
     * @return the pool
     */
    map<pair<string, APIRequestMethod>, APIObject*> &getNameObjectMap();


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
