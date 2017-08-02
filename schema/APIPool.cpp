//
// Created by lly on 29/05/2017.
//

#include "APIPool.h"

APIPool::APIPool(ParameterPool *paramPool, ResponsePool *responsePool) {
    this->paramPool = paramPool;
    this->responsePool = responsePool;
    APINameSet.clear();
    APINodeSet.clear();
    nodeNameMap.clear();
    objNameMap.clear();
    pool.clear();
    refMap.clear();
}

APIPool::~APIPool() {
    for (map<pair<string, APIRequestMethod>, APIObject*>::iterator ite = pool.begin();
            ite != pool.end();
            ++ite)
        delete ite->second;
}

APIObject *APIPool::parseAPI(string filePath, DocElement *ele, string name, APIRequestMethod method,
                             SchemesObject *rootSchemes, vector<ParameterObject *> *commonParams) {
    if (ele == NULL) {
        Error::addError(
                new FieldMissError(filePath, 1, 1, "operation")
        );
        return NULL;
    }
    if (ele->type != DOC_OBJECT) {
        Error::addError(
                new FieldInvalidError(filePath, ele->line, ele->col, "operation", ele->type, DOC_OBJECT)
        );
        return NULL;
    }

    pair<string, APIRequestMethod> key = make_pair(name, method);
    if (APINameSet.count(key) > 0) {
        ++refMap[key];
        return pool[key];
    }

    DocObjectElement *o = (DocObjectElement*)ele;
    APIObject *ans = new APIObject();
    ans->create(filePath, o, name, method, rootSchemes, commonParams, paramPool, responsePool);
    if (!ans->valid) {
        delete ans;
        return NULL;
    }

    APINameSet.insert(key);
    APINodeSet.insert(ele);
    nodeNameMap[ele] = key;
    objNameMap[ans] = key;
    pool[key] = ans;
    refMap[key] = 1;
    return ans;
}

bool APIPool::touch(DocElement *ele) {
    return APINodeSet.count(ele) > 0;
}

pair<string, APIRequestMethod> APIPool::getNameByElement(DocElement *ele) {
    if (nodeNameMap.count(ele) > 0)
        return nodeNameMap[ele];
    else
        return make_pair(string(""), GET); // illegal return
}

DocElement *APIPool::getElementByName(pair<string, APIRequestMethod> name) {
    if(APINameSet.count(name)) {
        // cost time!
        for (map<DocElement*, pair<string, APIRequestMethod>>::iterator ite = nodeNameMap.begin();
                ite != nodeNameMap.end();
                ++ite) {
            if (ite->second == name)
                return ite->first;
        }
    }
    return NULL;
}

APIObject *APIPool::getObjectByName(pair<string, APIRequestMethod> name) {
    if (APINameSet.count(name))
        return pool[name];
    else
        return NULL;
}

APIObject *APIPool::getObjectByElement(DocElement *ele) {
    if (nodeNameMap.count(ele) > 0)
        return pool[nodeNameMap[ele]];
    else
        return NULL; // not find
}

map<pair<string, APIRequestMethod>, APIObject*> &APIPool::getNameObjectMap() {
    return pool;
}