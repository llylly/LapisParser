//
// Created by lly on 29/05/2017.
//

#include "ParameterPool.h"

ParameterPool::ParameterPool(DataSchemaPool *schemaPool) {
    paramSet.clear();
    pool.clear();
    refMap.clear();
    nameMap.clear();
    revNameMap.clear();
    accuCnt = 0;
    this->schemaPool = schemaPool;
}

ParameterPool::~ParameterPool() {
    for (map<DocElement*, ParameterObject*>::iterator ite = pool.begin();
            ite != pool.end();
            ++ite)
        delete ite->second;
}

ParameterObject *ParameterPool::parseParameter(string filePath, DocElement *ele, string paramName) {
    if (paramName == "") paramName = makeName();
    if (ele == NULL) {
        Error::addError(
                new FieldMissError(filePath, 1, 1, "parameter")
        );
        return NULL;
    }
    if (ele->type != DOC_OBJECT) {
        Error::addError(
                new FieldInvalidError(filePath, ele->line, ele->col, "parameter", ele->type, DOC_OBJECT)
        );
        return NULL;
    }

    if (paramSet.count(ele) != 0) {
        ++refMap[ele];
        return pool[ele];
    }

    DocObjectElement *o = (DocObjectElement*)ele;
    ParameterObject *res = ParameterObjectFactory::create(filePath, o, schemaPool);
    if ((res == NULL) || (!res->valid)) return NULL;

    paramSet.insert(ele);
    pool[ele] = res;
    refMap[ele] = 1;
    nameMap[ele] = paramName;
    revNameMap[paramName] = ele;
    ++accuCnt;
    return res;
}

bool ParameterPool::rename(string oldName, string newName) {
    if (revNameMap.count(oldName)) {
        DocElement *e = revNameMap[oldName];
        nameMap[e] = newName;
        revNameMap.erase(oldName);
        revNameMap[newName] = e;
        return true;
    }
    return false;
}

bool ParameterPool::remove(string name) {
    if (revNameMap.count(name)) {
        DocElement *e = revNameMap[name];
        paramSet.erase(e);
        ParameterObject *o = pool[e];
        delete o;
        pool.erase(e);
        refMap.erase(e);
        nameMap.erase(e);
        revNameMap.erase(name);
        return true;
    }
    return false;
}

bool ParameterPool::touch(DocElement *ele) {
    return paramSet.count(ele) > 0;
}

bool ParameterPool::touch(string name) {
    return revNameMap.count(name) > 0;
}

int ParameterPool::getRefCount(DocElement *ele) {
    if (paramSet.count(ele))
        return refMap[ele];
    else
        return -1;
}

int ParameterPool::getRefCount(string name) {
    if (revNameMap.count(name))
        return refMap[revNameMap[name]];
    else
        return -1;
}

bool ParameterPool::setRefCount(DocElement *ele, int ref) {
    if (paramSet.count(ele)) {
        refMap[ele] = ref;
        return true;
    }
    return false;
}

bool ParameterPool::setRefCount(string name, int ref) {
    if (revNameMap.count(name)) {
        refMap[revNameMap[name]] = ref;
        return true;
    }
    return false;
}

const map<string, DocElement*> &ParameterPool::getNameMap() {
    return revNameMap;
};

ParameterObject *ParameterPool::getParameterByName(string name) {
    if (revNameMap.count(name))
        return pool[revNameMap[name]];
    return NULL;
}

string ParameterPool::makeName() {
    int t = accuCnt + 1;
    string s = "";
    stringstream sin(s);
    sin << "parameter" << t;
    sin.flush();
    return s;
}