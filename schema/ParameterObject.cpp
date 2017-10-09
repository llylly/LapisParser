//
// Created by lly on 22/07/2017.
//

#include "ParameterObject.h"

ParameterObject::ParameterObject() {
    this->valid = false;
    name = "";
    in = IN_INVALID;
    description = "";
    required = false;
    nullProbability = 0.25;
    schema = NULL;
    schemaInPool = false;
}

ParameterObject::~ParameterObject() {
    if (this->valid)  {
        if (!this->schemaInPool)
            delete schema;
        // Otherwise the schema will be deleted by the schema pool
        schema = NULL;
    }
}

BaseDataObject *ParameterObject::toDataObject() {
    if (!valid) return NULL;
    ObjectDataObject *o = new ObjectDataObject();
    (*o)["name"] = new StringDataObject(this->name);
    StringDataObject *in = NULL;
    switch (this->in) {
        case IN_INVALID:
            in = new StringDataObject("invalid");
            break;
        case QUERY:
            in = new StringDataObject("query");
            break;
        case HEADER:
            in = new StringDataObject("header");
            break;
        case PATH:
            in = new StringDataObject("path");
            break;
        case FORMDATA:
            in = new StringDataObject("formData");
            break;
        case BODY:
            in = new StringDataObject("body");
            break;
    }
    (*o)["in"] = in;
    if (this->description != "")
        (*o)["description"] = new StringDataObject(this->description);
    (*o)["required"] = new BooleanDataObject(this->required);
    if ((this->in != PATH) && (!this->required)) {
        (*o)["x-nullProbability"] = new NumberDataObject(this->nullProbability);
    }
    if (this->in == BODY) {
        (*o)["schema"] = this->schema->toDataObject();
        if ((*o)["schema"] == NULL) {
            delete o;
            return NULL;
        }
    } else {
        (*o)["x-schema"] = this->schema->toDataObject();
        if ((*o)["x-schema"] == NULL) {
            delete o;
            return NULL;
        }
    }
    return o;
}

ParameterObject* ParameterObjectFactory::create(string filePath, DocElement *ele, DataSchemaPool *schemaPool) {
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
    DocObjectElement *o = (DocObjectElement*)ele;

    ParameterObject *res = new ParameterObject();

    /* name */
    DocElement *nameEle = o->get("name");
    if (nameEle == NULL) {
        Error::addError(
                new FieldMissError(filePath, ele->line, ele->col, "parameter.name")
        );
        delete res;
        return NULL;
    }
    pair<string, bool> nameRes = DocElementHelper::parseToString(nameEle);
    if (nameRes.second) {
        res->name = nameRes.first;
    } else {
        Error::addError(
                new FieldInvalidError(filePath, nameEle->line, nameEle->col, "parameter.name", nameEle->type, DOC_SCALAR)
        );
        delete res;
        return NULL;
    }

    /* in */
    DocElement *inEle = o->get("in");
    if (inEle == NULL) {
        Error::addError(
                new FieldMissError(filePath, ele->line, ele->col, "parameter.in")
        );
        delete res;
        return NULL;
    }
    pair<string, bool> inRes = DocElementHelper::parseToString(inEle);
    if (inRes.second) {
        if (inRes.first == "query") {
            res->in = QUERY;
        }
        else if (inRes.first == "header") {
            res->in  = HEADER;
        }
        else if (inRes.first == "path") {
            res->in = PATH;
        }
        else if (inRes.first == "formData") {
            res->in = FORMDATA;
        }
        else if (inRes.first == "body") {
            res->in = BODY;
        }
        else {
            Error::addError(
                    new FieldIllegalError(filePath, inEle->line, inEle->col, "parameter.in")
            );
            delete res;
            return NULL;
        }
    } else {
        Error::addError(
                new FieldInvalidError(filePath, inEle->line, inEle->col, "parameter.in", inEle->type, DOC_SCALAR)
        );
        delete res;
        return NULL;
    }

    /* description */
    DocElement *descEle = o->get("description");
    if (descEle) {
        pair<string, bool> descRes = DocElementHelper::parseToString(descEle);
        if (descRes.second) {
            res->description = descRes.first;
        } else {
            Error::addError(
                    new FieldInvalidError(filePath, descEle->line, descEle->col, "parameter.description", descEle->type, DOC_SCALAR)
            );
            delete res;
            return NULL;
        }
    }

    /* required */
    DocElement *reqEle = o->get("required");
    if (reqEle) {
        pair<bool, bool> reqRes = DocElementHelper::parseToBool(reqEle);
        if (reqRes.second) {
            res->required = reqRes.first;
        } else {
            Error::addError(
                    new FieldIllegalError(filePath, reqEle->line, reqEle->col, "parameter.required")
            );
            delete res;
            return NULL;
        }
    }

    /* x-nullProbability */
    DocElement *nullProbEle = o->get("x-nullProbability");
    if ((res->in != PATH) && (!res->required)) {
        if (nullProbEle) {
            pair<double, bool> nullProbRes = DocElementHelper::parseToDouble(nullProbEle);
            if ((nullProbRes.second) && (nullProbRes.first > -1e-6) && (nullProbRes.second < 1. + 1e-6)) {
                res->nullProbability = nullProbRes.first;
            } else {
                Error::addError(
                        new FieldIllegalError(filePath, nullProbEle->line, nullProbEle->col, "parameter.x-nullProbability")
                );
                delete res;
                return NULL;
            }
        }
    } else {
        res->nullProbability = 0.0;
        if (nullProbEle) {
            Error::addError(
                    new FieldIllegalError(filePath, nullProbEle->line, nullProbEle->col, "parameter.x-nullProbability")
            );
            delete res;
            return NULL;
        }
    }

    /* schema */
    DocElement *schemaEle = o->get("schema");
    if (res->in == BODY) {
        if (schemaEle == NULL) {
            Error::addError(
                    new FieldMissError(filePath, ele->line, ele->col, "parameter.schema")
            );
            delete res;
            return NULL;
        } else {
            if (schemaEle->type == DOC_OBJECT) {
                DocObjectElement *oo = (DocObjectElement*)schemaEle;
                DataSchemaObject *out;
                if (schemaPool) {
                    out = schemaPool->parseDataSchema(filePath, oo);
                    res->schemaInPool = true;
                } else
                    out = DataSchemaObjectFactory::create(filePath, oo, DataSchemaObjectFactory::NORMAL_SCHEMA, false);
                if ((out == NULL) || (!out->valid)) {
                    delete res;
                    if (out != NULL) delete out;
                    return NULL;
                }
                res->schema = out;
            }
        }
    } else {
        if (schemaEle) {
            Error::addError(
                    new FieldIllegalError(filePath, schemaEle->line, schemaEle->col, "parameter.schema")
            );
            delete res;
            return NULL;
        } else {
            DataSchemaObject *out = DataSchemaObjectFactory::create(filePath, o, DataSchemaObjectFactory::PARAMETER_SCHEMA, false);
            if ((out == NULL) || (!out->valid)) {
                delete res;
                if (out != NULL) delete out;
                return NULL;
            }
            res->schema = out;
        }
    }

    res->valid = true;
    return res;
}