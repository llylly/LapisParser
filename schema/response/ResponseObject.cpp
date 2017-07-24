//
// Created by lly on 23/07/2017.
//

#include "ResponseObject.h"

ResponseObject::ResponseObject(): AbstractResponseObject() {
    type = NORMAL;
}

ResponseObject::ResponseObject(const ResponseObject &source): AbstractResponseObject(source) { }

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

    this->code = -1;
    this->stage = RAW;

    return true;
}

bool ResponseObject::checkResponse(DocElement *data) {
    if (this->stage == INVALID_RESPONSE)
        return false;
    if (this->schema == NULL)
        return true;
    else
        return (this->schema->transform(data) != NULL);
}