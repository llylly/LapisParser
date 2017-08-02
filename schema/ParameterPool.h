//
// Created by lly on 29/05/2017.
//

#ifndef VPARSER_PARAMETERPOOL_H
#define VPARSER_PARAMETERPOOL_H


#include <set>
#include "../doc/DocElement.h"
#include "ParameterObject.h"

class ParameterPool {
/**
 * This class processes all the parameters of scripts
 * Similar to DataSchemaPool
 */

public:
    /**
     * Constructor
     */
    ParameterPool(DataSchemaPool *schemaPool);

    /**
     * Destructor
     */
    ~ParameterPool();

    /**
     * Parse a doc node which represents a complete Parameter Object
     *  Return whether successfully parsed
     *  If it has parsed, also return true, and increment the ref count
     * @param ele: doc node to be parsed
     * @param nodeName: enabling customized schema name
     * @return success: the parsed object; failed: NULL
     */
    ParameterObject *parseParameter(string filePath, DocElement *ele, string paramName = "");

    /**
     * Rename a parameter index name
     * Fail if oldName doesn't exist, or newName conflicts
     * @param oldName: oldName
     * @param newName: newName
     * @return
     */
    bool rename(string oldName, string newName);

    /**
     * Remove one from the pool by name
     * It will delete ParameterObject, but not affect doc node
     * @param name: name to delete
     * @return bool: success or not
     */
    bool remove(string name);

    /**
     * Inspect the existence by doc node
     * @param ele: doc node
     * @return bool: exist or not
     */
    bool touch(DocElement *ele);

    /**
     * Inspect the existence by name
     * @param name: name
     * @return bool: exist or not
     */
    bool touch(string name);

    /**
     * Get ref count by doc node
     * @param ele
     * @return -1 means failed
     */
    int getRefCount(DocElement *ele);

    /**
     * Get ref count by name
     * @param name
     * @return -1 means failed
     */
    int getRefCount(string name);

    /**
     * Set ref count by doc node
     * @param ele
     * @param ref
     * @return success or not
     */
    bool setRefCount(DocElement *ele, int ref);

    /**
     * Set ref count by name
     * @param name
     * @param ref
     * @return success or not
     */
    bool setRefCount(string name, int ref);

    /**
     * Get the name map
     * @return the name map
     */
    const map<string, DocElement*> &getNameMap();

    /**
     * Get parameter by name
     * @param name
     * @return find: return parameter; not find: return NULL
     */
    ParameterObject *getParameterByName(string name);

private:
    DataSchemaPool *schemaPool;

    set<DocElement*> paramSet;
    map<DocElement*, ParameterObject*> pool;
    map<DocElement*, int> refMap;
    map<DocElement*, string> nameMap;
    map<string, DocElement*> revNameMap;

    int accuCnt;

    string makeName();

};


#endif //VPARSER_PARAMETERPOOL_H
