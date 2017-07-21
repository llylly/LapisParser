//
// Created by lly on 29/05/2017.
//

#include "DataSchemaPool.h"

DataSchemaPool::DataSchemaPool() {
    schemaSet.clear();
    pool.clear();
    refMap.clear();
    nameMap.clear();
    revNameMap.clear();
}

bool DataSchemaPool::parseDataSchema(DocElement *ele, string schemaName) {
    /**
     * TODO
     */
    return false;
}

bool DataSchemaPool::rename(string oldName, string newName) {
    /**
     * TODO
     */
    return false;
}

bool DataSchemaPool::remove(string name) {
    /**
     * TODO
     */
    return false;
}

bool DataSchemaPool::touch(DocElement *ele) {
    /**
     * TODO
     */
    return false;
}

bool DataSchemaPool::touch(string name) {
    /**
     * TODO
     */
    return false;
}

int DataSchemaPool::getRefCount(DocElement *ele) {
    /**
     * TODO
     */
    return -1;
}

int DataSchemaPool::getRefCount(string name) {
    /**
     * TODO
     */
    return -1;
}

bool DataSchemaPool::setRefCount(DocElement *ele) {
    /**
     * TODO
     */
    return false;
}

bool DataSchemaPool::setRefCount(string name) {
    /**
     * TODO
     */
    return false;
}