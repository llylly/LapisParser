//
// Created by lly on 26/07/2017.
//

#include "APIConstraintObject.h"
#include "../APIObject.h"
#include "../APIPool.h"

APIConstraintObject::APIConstraintObject() {
    stage = INVALID_CONS;
    guestDoc = NULL;
    guest = NULL;
    guestName = "";
    guestMethod = GET;
    type = BEFORE;
    minInterval = 0;
    maxInterval = -1;
    minTimes = 1;
    maxTimes = -1;
}

bool APIConstraintObject::create(string filePath, DocElement *ele) {
    if (ele == NULL) {
        Error::addError(
                new FieldMissError(filePath, 1, 1, "operation.x-constraint")
        );
        return false;
    }
    if (ele->type != DOC_OBJECT) {
        Error::addError(
                new FieldInvalidError(filePath, ele->line, ele->col, "operation.x-constraint", ele->type, DOC_OBJECT)
        );
        return false;
    }
    DocObjectElement *o = (DocObjectElement*)ele;

    /** operation **/
    DocElement *operationEle = o->get("operation");
    if (operationEle == NULL) {
        Error::addError(
                new FieldMissError(filePath, ele->line, ele->col, "operation.x-constraint.operation")
        );
        return false;
    }
    if (operationEle->type != DOC_OBJECT) {
        Error::addError(
                new FieldInvalidError(filePath, operationEle->line, operationEle->col, "operation.x-consrtaint.operation", operationEle->type, DOC_OBJECT)
        );
        return false;
    }
    this->guestDoc = (DocObjectElement*)operationEle;

    /** type **/
    DocElement *typeEle = o->get("type");
    if (typeEle == NULL) {
        Error::addError(
                new FieldMissError(filePath, ele->line, ele->col, "operation.x-constraint.type")
        );
        return false;
    }
    pair<string, bool> typeRes = DocElementHelper::parseToString(typeEle);
    if (typeRes.second) {
        const string &str = typeRes.first;
        if (str == "before")
            this->type = BEFORE;
        else if (str == "after")
            this->type = AFTER;
        else {
            Error::addError(
                    new FieldIllegalError(filePath, typeEle->line, typeEle->col, "operation.x-constraint.type")
            );
            return false;
        }
    } else {
        Error::addError(
                new FieldInvalidError(filePath, typeEle->line, typeEle->col, "operation.x-constraint.type", typeEle->type, DOC_SCALAR)
        );
        return false;
    }

    /** minInterval **/
    DocElement *minInterEle = o->get("minInterval");
    if (minInterEle) {
        pair<long long, bool> res = DocElementHelper::parseToInt(minInterEle);
        if ((res.second) && (res.first >= 0L)) {
            this->minInterval = (int)res.first;
        } else {
            Error::addError(
                    new FieldIllegalError(filePath, minInterEle->line, minInterEle->col, "operation.x-constraint.minInterval")
            );
            return false;
        }
    }

    /** maxInterval **/
    DocElement *maxInterEle = o->get("maxInterval");
    if (maxInterEle) {
        pair<long long, bool> res = DocElementHelper::parseToInt(maxInterEle);
        if ((res.second) && (res.first >= 1L)) {
            this->maxInterval = (int)res.first;
        } else {
            Error::addError(
                    new FieldIllegalError(filePath, maxInterEle->line, maxInterEle->col, "operation.x-constraint.maxInterval")
            );
            return false;
        }
    }

    if (this->minInterval > this->maxInterval) {
        Error::addError(
                new IllegalIntervalError(filePath, ele->line, ele->col, "operation.x-constraint",
                                         (double)this->minInterval, (double)this->maxInterval)
        );
        return false;
    }

    /** minTimes **/
    DocElement *minTimEle = o->get("minTimes");
    if (minTimEle) {
        pair<long long, bool> res = DocElementHelper::parseToInt(minTimEle);
        if ((res.second) && (res.first >= 0L)) {
            this->minTimes = (int)res.first;
        } else {
            Error::addError(
                    new FieldIllegalError(filePath, minTimEle->line, minTimEle->col, "operation.x-constraint.minTimes")
            );
            return false;
        }
    }

    /** maxTimes **/
    DocElement *maxTimEle = o->get("maxTimes");
    if (maxTimEle) {
        pair<long long, bool> res = DocElementHelper::parseToInt(maxTimEle);
        if ((res.second) && (res.first >= 0L)) {
            this->maxTimes = (int)res.first;
        } else {
            Error::addError(
                    new FieldIllegalError(filePath, maxTimEle->line, maxTimEle->col, "operation.x-constraint.maxTimes")
            );
            return false;
        }
    }

    this->stage = BAKED_CONS;
    return true;
}

bool APIConstraintObject::findGuest(APIPool *pool) {
    if (stage == VALID_CONS) {
        return true;
    } else
    if (stage == BAKED_CONS) {
        if (guestDoc == NULL) return false;
        bool have = pool->touch(guestDoc);
        if (!have) return false;
        APIObject *obj = pool->getObjectByElement(guestDoc);
        if (obj == NULL) return false;
        this->guest = obj;
        pair<string, APIRequestMethod> names = pool->getNameByElement(guestDoc);
        guestName = names.first;
        guestMethod = names.second;
        this->stage = VALID_CONS;
        return true;
    } else
        return false;
}

BaseDataObject *APIConstraintObject::toDataObject() {
    if (stage != VALID_CONS) {
        return NULL;
    }
    ObjectDataObject *ans = new ObjectDataObject();

    ObjectDataObject *operation = new ObjectDataObject();
    string operationStr = "#/paths//";
    operationStr += this->guest->name;
    operationStr += "/";
    if (this->guest->requestMethod == POST) operationStr += "post";
    if (this->guest->requestMethod == GET) operationStr += "get";
    (*operation)["$ref"] = new StringDataObject(operationStr);

    (*ans)["operation"] = operation;

    StringDataObject *type;
    if (this->type == BEFORE) type = new StringDataObject("before");
    if (this->type == AFTER) type = new StringDataObject("after");
    (*ans)["type"] = type;

    (*ans)["minInterval"] = new IntegerDataObject(this->minInterval);

    if (this->maxInterval >= 0)
        (*ans)["maxInterval"] = new IntegerDataObject(this->maxInterval);

    (*ans)["minTimes"] = new IntegerDataObject(this->minTimes);

    if (this->maxTimes >= 0)
        (*ans)["maxTimes"] = new IntegerDataObject(this->maxTimes);

    return ans;
}