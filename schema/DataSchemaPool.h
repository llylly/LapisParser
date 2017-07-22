//
// Created by lly on 29/05/2017.
//

#ifndef VPARSER_DATASCHEMAPOOL_H
#define VPARSER_DATASCHEMAPOOL_H


#include <set>
#include <sstream>
#include "../doc/DocElement.h"
#include "DataSchemaObject.h"

using namespace std;

class DataSchemaPool {
/**
 * This class processes all the Schemas of scripts
 * Schema only mean complete data schema, including 'definitions', 'x-schema', 'scheme',
 *  but excluding direct 'items', and direct 'parameters'(means not use 'x-schema' or 'scheme'
 */


public:
    /**
     * Constructor
     */
    DataSchemaPool();

    /**
     * Parse a doc node which represents a complete Schema Object
     *  Return whether successfully parsed
     *  If it has parsed, also return true, and increment the ref count
     * @param ele: doc node to be parsed
     * @param nodeName: enabling customized schema name
     * @return bool: success or not
     */
    bool parseDataSchema(string filePath, DocElement *ele, string schemaName = "");

    /**
     * Rename a schema
     * Fail if oldName doesn't exist, or newName conflicts
     * @param oldName: oldName
     * @param newName: newName
     * @return
     */
    bool rename(string oldName, string newName);

    /**
     * Remove one from the pool by name
     * It will delete DataSchemaObject, but not affect doc node
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
     * Get schema by name
     * @param name
     * @return find: return schema; not find: return NULL
     */
    DataSchemaObject *getSchemaByName(string name);

private:
    set<DocElement*> schemaSet;
    map<DocElement*, DataSchemaObject*> pool;
    map<DocElement*, int> refMap;
    map<DocElement*, string> nameMap;
    map<string, DocElement*> revNameMap;

    int accuCnt;

    string makeName();
};


#endif //VPARSER_DATASCHEMAPOOL_H
