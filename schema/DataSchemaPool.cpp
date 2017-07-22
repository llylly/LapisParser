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
    accuCnt = 0;
}

bool DataSchemaPool::parseDataSchema(string filePath, DocElement *ele, string schemaName) {
    if (schemaName == "") schemaName = makeName();
    if ((ele == NULL) || (ele->type != DOC_OBJECT)) return false;

    if (schemaSet.count(ele) != 0) {
        ++refMap[ele];
        return true;
    }

    DocObjectElement *o = (DocObjectElement*)ele;
    DataSchemaObject *res = DataSchemaObjectFactory::create(filePath, o, DataSchemaObjectFactory::NORMAL_SCHEMA, false);
    if ((res == NULL) || (!res->valid)) return false;

    schemaSet.insert(ele);
    pool[ele] = res;
    refMap[ele] = 1;
    nameMap[ele] = schemaName;
    revNameMap[schemaName] = ele;
    ++accuCnt;
    return true;
}

bool DataSchemaPool::rename(string oldName, string newName) {
    if (revNameMap.count(oldName)) {
        DocElement *e = revNameMap[oldName];
        nameMap[e] = newName;
        revNameMap.erase(oldName);
        revNameMap[newName] = e;
        return true;
    }
    return false;
}

bool DataSchemaPool::remove(string name) {
    if (revNameMap.count(name)) {
        DocElement *e = revNameMap[name];
        schemaSet.erase(e);
        DataSchemaObject *o = pool[e];
        delete o;
        pool.erase(e);
        refMap.erase(e);
        nameMap.erase(e);
        revNameMap.erase(name);
        return true;
    }
    return false;
}

bool DataSchemaPool::touch(DocElement *ele) {
    return schemaSet.count(ele) > 0;
}

bool DataSchemaPool::touch(string name) {
    return revNameMap.count(name) > 0;
}

int DataSchemaPool::getRefCount(DocElement *ele) {
    if (schemaSet.count(ele))
        return refMap[ele];
    else
        return -1;
}

int DataSchemaPool::getRefCount(string name) {
    if (revNameMap.count(name))
        return refMap[revNameMap[name]];
    else
        return -1;
}

bool DataSchemaPool::setRefCount(DocElement *ele, int ref) {
    if (schemaSet.count(ele)) {
        refMap[ele] = ref;
        return true;
    }
    return false;
}

bool DataSchemaPool::setRefCount(string name, int ref) {
    if (revNameMap.count(name)) {
        refMap[revNameMap[name]] = ref;
        return true;
    }
    return false;
}

const map<string, DocElement*> &DataSchemaPool::getNameMap() {
    return revNameMap;
};

DataSchemaObject *DataSchemaPool::getSchemaByName(string name) {
    if (revNameMap.count(name))
        return pool[revNameMap[name]];
    return NULL;
}

string DataSchemaPool::makeName() {
    int t = accuCnt + 1;
    string s = "";
    stringstream sin(s);
    sin << "schema" << t;
    sin.flush();
    return s;
}