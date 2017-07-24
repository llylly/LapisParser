//
// Created by lly on 29/05/2017.
//

#include "ResponsePool.h"

ResponsePool::ResponsePool(DataSchemaPool *schemaPool) {
    responseSet.clear();
    pool.clear();
    refMap.clear();
    nameMap.clear();
    revNameMap.clear();
    accuCnt = 0;
    this->schemaPool = schemaPool;
}

ResponsePool::~ResponsePool() {
    for (map<DocElement*, AbstractResponseObject*>::iterator ite = pool.begin();
         ite != pool.end();
         ++ite)
        delete ite->second;
}

ResponseObject *ResponsePool::parseResponse(string filePath, DocElement *ele, string responseName) {
    if (responseName == "") responseName = makeName();
    if (ele == NULL) {
        Error::addError(
                new FieldMissError(filePath, 1, 1, "response")
        );
        return NULL;
    }
    if (ele->type != DOC_OBJECT) {
        Error::addError(
                new FieldInvalidError(filePath, ele->line, ele->col, "response", ele->type, DOC_OBJECT)
        );
        return NULL;
    }

    if (responseSet.count(ele) != 0) {
        if (pool[ele]->type == NORMAL) {
            ++refMap[ele];
            return (ResponseObject*)pool[ele];
        } else
            return NULL;
    }

    DocObjectElement *o = (DocObjectElement*)ele;
    ResponseObject *ans = new ResponseObject();
    ans->createRaw(filePath, o, schemaPool);
    if (ans->stage != RAW) {
        delete ans;
        return NULL;
    }

    responseSet.insert(ele);
    pool[ele] = ans;
    refMap[ele] = 1;
    nameMap[ele] = responseName;
    revNameMap[responseName] = ele;
    ++accuCnt;
    return ans;
}

ResponseExtensionObject *ResponsePool::parseResponseExtension(string filePath, DocElement *ele, string responseName) {
    if (responseName == "") responseName = makeName();
    if (ele == NULL) {
        Error::addError(
                new FieldMissError(filePath, 1, 1, "responseExtension")
        );
        return NULL;
    }
    if (ele->type != DOC_OBJECT) {
        Error::addError(
                new FieldInvalidError(filePath, ele->line, ele->col, "responseExtension", ele->type, DOC_OBJECT)
        );
        return NULL;
    }

    if (responseSet.count(ele) != 0) {
        if (pool[ele]->type == EXTENSION) {
            ++refMap[ele];
            return (ResponseExtensionObject*)pool[ele];
        } else
            return NULL;
    }

    DocObjectElement *o = (DocObjectElement*)ele;
    ResponseExtensionObject *ans = new ResponseExtensionObject();
    ans->createRaw(filePath, o, schemaPool);
    if (ans->stage != RAW) {
        delete ans;
        return NULL;
    }

    responseSet.insert(ele);
    pool[ele] = ans;
    refMap[ele] = 1;
    nameMap[ele] = responseName;
    revNameMap[responseName] = ele;
    ++accuCnt;
    return ans;
}

bool ResponsePool::rename(string oldName, string newName) {
    if (revNameMap.count(oldName)) {
        DocElement *e = revNameMap[oldName];
        nameMap[e] = newName;
        revNameMap.erase(oldName);
        revNameMap[newName] = e;
        return true;
    }
    return false;
}

bool ResponsePool::remove(string name) {
    if (revNameMap.count(name)) {
        DocElement *e = revNameMap[name];
        responseSet.erase(e);
        AbstractResponseObject *o = pool[e];
        delete o;
        pool.erase(e);
        refMap.erase(e);
        nameMap.erase(e);
        revNameMap.erase(name);
        return true;
    }
    return false;
}

bool ResponsePool::touch(DocElement *ele) {
    return responseSet.count(ele) > 0;
}

bool ResponsePool::touch(string name) {
    return revNameMap.count(name) > 0;
}

int ResponsePool::getRefCount(DocElement *ele) {
    if (responseSet.count(ele))
        return refMap[ele];
    else
        return -1;
}

int ResponsePool::getRefCount(string name) {
    if (revNameMap.count(name))
        return refMap[revNameMap[name]];
    else
        return -1;
}

bool ResponsePool::setRefCount(DocElement *ele, int ref) {
    if (responseSet.count(ele)) {
        refMap[ele] = ref;
        return true;
    }
    return false;
}

bool ResponsePool::setRefCount(string name, int ref) {
    if (revNameMap.count(name)) {
        refMap[revNameMap[name]] = ref;
        return true;
    }
    return false;
}

const map<string, DocElement*> &ResponsePool::getNameMap() {
    return revNameMap;
};

AbstractResponseObject *ResponsePool::getResponseByName(string name) {
    if (revNameMap.count(name))
        return pool[revNameMap[name]];
    return NULL;
}

string ResponsePool::makeName() {
    int t = accuCnt + 1;
    string s = "";
    stringstream sin(s);
    sin << "response" << t;
    sin.flush();
    return s;
}