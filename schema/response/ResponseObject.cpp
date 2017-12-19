//
// Created by lly on 23/07/2017.
//

#include "ResponseObject.h"

ResponseObject::ResponseObject(): AbstractResponseObject() {
    type = NORMAL;
    headers.clear();
}

ResponseObject::ResponseObject(const ResponseObject &source): AbstractResponseObject(source) {
    this->headers.clear();
    for (map<string, ParameterObject*>::const_iterator ite = source.headers.cbegin(); ite != source.headers.cend(); ++ite) {
        this->headers[(*ite).first] = (*ite).second;
    }
}

BaseDataObject *ResponseObject::toDataObject() {
    BaseDataObject *obj = AbstractResponseObject::toDataObject();
    if (obj == NULL) return NULL;
    ObjectDataObject *o = (ObjectDataObject*)obj;
    if (this->headers.size() > 0) {
        ObjectDataObject *headersObj = new ObjectDataObject();
        for (map<string, ParameterObject*>::iterator ite = this->headers.begin();
                ite != this->headers.end();
                ++ite) {
            (*headersObj)[ite->first] = ite->second->toDataObject();
        }
        (*o)["headers"] = headersObj;
    }
    return o;
}

bool ResponseObject::createRaw(string filePath, DocObjectElement *ele, DataSchemaPool *pool) {
    if (ele == NULL) {
        Error::addError(
                new FieldMissError(filePath, 1, 1, "response")
        );
        return false;
    }

    /* description */
    DocElement *descEle = ele->get("description");
    if (descEle == NULL) {
        Error::addError(
                new FieldMissError(filePath, ele->line, ele->col, "response.description")
        );
        return false;
    }
    pair<string, bool> descRes = DocElementHelper::parseToString(descEle);
    if (descRes.second) {
        this->description = descRes.first;
    } else {
        Error::addError(
                new FieldInvalidError(filePath, descEle->line, descEle->col, "response.description", descEle->type, DOC_SCALAR)
        );
        return false;
    }

    /* schema */
    DocElement *schema = ele->get("schema");
    if (schema != NULL) {
        DataSchemaObject *o_schema = pool->parseDataSchema(filePath, schema);
        if (o_schema == NULL)
            return false;
        else
            this->schema = o_schema;
    }

    /* headers */
    DocElement *headersEle = ele->get("headers");
    if (headersEle != NULL) {
        if (headersEle->type != DOC_OBJECT) {
            Error::addError(
                    new FieldInvalidError(filePath, headersEle->line, headersEle->col, "response.headers", headersEle->type, DOC_OBJECT)
            );
            return false;
        } else {
            DocObjectElement *objHeadersEle = (DocObjectElement*)headersEle;
            map<string, DocElement*> *headersMap = objHeadersEle->getMemberMap();
            for (map<string, DocElement*>::iterator ite = headersMap->begin(); ite != headersMap->end(); ++ite) {
                ParameterObject *nowParam = ParameterObjectFactory::create(filePath, ite->second, pool, true);
                if (nowParam != NULL) {
                    this->headers[ite->first] = nowParam;
                    nowParam->name = ite->first;
                } else {
                    return false;
                }
            }
        }
    }

    this->code = -1;
    this->stage = RAW;

    return true;
}

bool ResponseObject::checkResponse(DocElement *data) {
    if (this->stage == INVALID_RESPONSE)
        return false;

    if (this->schema == NULL)
        return true;
    else {
        if (data->type != DOC_OBJECT)
            // should be a Object containing "body" field and "headers" field
            return false;
        else {
            DocObjectElement *objData = (DocObjectElement*)data;
            bool schemaOk = false, headersOk = true;

            if ((this->schema == NULL) || (this->schema->transform(objData->get("body")) != NULL))
                schemaOk = true;

            DocElement *headersData = objData->get("headers");
            // when headers is not NULL, it must be of Object type
            if ((headersData != NULL) && (headersData->type != DOC_OBJECT)) {
                headersOk = false;
            } else
            for (map<string, ParameterObject*>::iterator ite = this->headers.begin();
                    ite != this->headers.end();
                    ++ite) {
                ParameterObject *nowParam = ite->second;
                if (nowParam->required) {
                    // if the item is required, it should appear
                    if ((headersData == NULL) || ((DocObjectElement*)headersData)->get(ite->first) == NULL)
                        headersOk = false;
                }
                if (headersData != NULL) {
                    DocElement *nowObj = ((DocObjectElement*)headersData)->get(ite->first);
                    if (nowObj != NULL)
                        headersOk = headersOk && (nowParam->schema->transform(nowObj) != NULL);
                }
                if (!headersOk) break;
            }
            return schemaOk && headersOk;
        }
    }
}

ResponseObject::~ResponseObject() {
    if (this->headers.size() > 0) {
        for (map<string, ParameterObject *>::iterator ite = this->headers.begin();
             ite != this->headers.end();
             ++ite) {
            delete ite->second;
        }
        this->headers.clear();
    }
}