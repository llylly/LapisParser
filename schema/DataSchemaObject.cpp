//
// Created by lly on 12/07/2017.
//

#include <cstdio>
#include "DataSchemaObject.h"
#include "data_schema/IntegerSchema.h"
#include "data_schema/ArraySchema.h"
#include "data_schema/BooleanSchema.h"
#include "data_schema/CustomizedSchema.h"
#include "data_schema/FileSchema.h"
#include "data_schema/NumberSchema.h"
#include "data_schema/ObjectSchema.h"
#include "data_schema/StringSchema.h"

const int DataSchemaObject::TYPE_BASE = 0;
const int DataSchemaObject::TYPE_STRING = 1;
const int DataSchemaObject::TYPE_NUMBER = 2;
const int DataSchemaObject::TYPE_INTEGER = 3;
const int DataSchemaObject::TYPE_BOOLEAN = 4;
const int DataSchemaObject::TYPE_ARRAY = 5;
const int DataSchemaObject::TYPE_FILE = 6;
const int DataSchemaObject::TYPE_OBJECT = 7;
const int DataSchemaObject::TYPE_CUSTOMIZED = 8;


const int DataSchemaObjectFactory::PARAMETER_SCHEMA = 1;
const int DataSchemaObjectFactory::ITEM_SCHEMA = 2;
const int DataSchemaObjectFactory::NORMAL_SCHEMA = 3;


DataSchemaObject::DataSchemaObject() {
    valid = false;
    title = "";
    description = "";
    type = TYPE_BASE;
    allowEmptyValue = false;
        // default: false
    _default = NULL;
    rawEnum = NULL;
    emptyProbability = 0.25;
        // default: 0.25
    externalDocs = NULL;
    example = NULL;
    xml = NULL;
    _enum.clear();
}

DataSchemaObject::~DataSchemaObject() {
    if (_default)
        delete _default;
    if (externalDocs)
        delete externalDocs;
    if (example)
        delete example;
    if (xml)
        delete xml;
    for (vector<BaseDataObject*>::iterator ite = _enum.begin(); ite != _enum.end(); ++ite) {
        delete *ite;
    }
}

BaseDataObject *DataSchemaObject::toDataObject() {
    if (!this->valid)
        return NULL;
    ObjectDataObject *obj = new ObjectDataObject();
    (*obj)["title"] = new StringDataObject(this->title);
    (*obj)["description"] = new StringDataObject(this->description);
    switch (this->type) {
        case TYPE_STRING:
            (*obj)["type"] = new StringDataObject("string");
            break;
        case TYPE_NUMBER:
            (*obj)["type"] = new StringDataObject("number");
            break;
        case TYPE_INTEGER:
            (*obj)["type"] = new StringDataObject("integer");
            break;
        case TYPE_BOOLEAN:
            (*obj)["type"] = new StringDataObject("boolean");
            break;
        case TYPE_ARRAY:
            (*obj)["type"] = new StringDataObject("array");
            break;
        case TYPE_FILE:
            (*obj)["type"] = new StringDataObject("file");
            break;
        case TYPE_OBJECT:
            (*obj)["type"] = new StringDataObject("object");
            break;
        case TYPE_CUSTOMIZED:
            break;
        default:
            (*obj)["type"] = new StringDataObject("ERROR TYPE");
    }
    (*obj)["allowEmptyValue"] = new BooleanDataObject(this->allowEmptyValue);
    if (this->_default)
        (*obj)["default"] = DataObjectAdapter::deepCopy(this->_default);
    if (this->_enum.size() > 0) {
        SequenceDataObject *seq = new SequenceDataObject();
        int len = (int)this->_enum.size();
        for (int i=0; i < len; ++i) {
            seq->push(DataObjectAdapter::deepCopy(this->_enum[i]));
        }
        (*obj)["enum"] = seq;
    }
    (*obj)["emptyProbability"] = new NumberDataObject(this->emptyProbability);
    if (this->externalDocs)
        (*obj)["externalDocs"] = this->externalDocs->toDataObject();
    if (this->example)
        (*obj)["example"] = DataObjectAdapter::deepCopy(this->example);
    if (this->xml)
        (*obj)["xml"] = this->xml->toDataObject();
    return obj;
}

DataSchemaObject *DataSchemaObjectFactory::create(string filePath, DocObjectElement *obj, int schemaType, bool inProperty) {
    if (obj == NULL) return NULL;

    if (obj->type != DOC_OBJECT) {
        Error::addError(new FieldInvalidError(filePath, obj->line, obj->col, "schema", obj->type, DOC_OBJECT));
        return NULL;
    }

    DataSchemaObject *res = NULL;

    if ((schemaType == NORMAL_SCHEMA) && (obj->get("x-customize"))) {
        // When customized field exists, other fields are ignored
        res = new CustomizedSchema();
    } else
    if (((schemaType == PARAMETER_SCHEMA) || (schemaType == ITEM_SCHEMA)) && (obj->get("x-schema"))) {
        // When x-schema exists, view it as a new parse beginning
        DocElement *innerEle = obj->get("x-schema");
        if (innerEle->type != DOC_OBJECT) {
            Error::addError(new FieldInvalidError(filePath, innerEle->line, innerEle->col, "schema.x-schema", innerEle->type, DOC_OBJECT));
            return NULL;
        } else
            return DataSchemaObjectFactory::create(filePath, (DocObjectElement*)innerEle, NORMAL_SCHEMA, false);
    } else {
        DocElement *typeEle = obj->get("type");
        if (typeEle == NULL) {
            Error::addError(new FieldMissError(filePath, obj->line, obj->col, "schema.type"));
            return NULL;
        }
        if (typeEle->type != DOC_SCALAR) {
            Error::addError(new FieldInvalidError(filePath, obj->line, obj->col, "schema.type", obj->type, DOC_SCALAR));
            return NULL;
        }
        string typeStr = ((DocScalarElement *) typeEle)->getValue();
        if (typeStr == "string") {
            res = new StringSchema();
        }
        if (typeStr == "number") {
            res = new NumberSchema();
        }
        if (typeStr == "integer") {
            res = new IntegerSchema();
        }
        if (typeStr == "boolean") {
            res = new BooleanSchema();
        }
        if (typeStr == "array") {
            res = new ArraySchema();
        }
        if ((typeStr == "file") && (schemaType != DataSchemaObjectFactory::ITEM_SCHEMA)) {
            res = new FileSchema();
        }
        if ((typeStr == "object") && (schemaType == DataSchemaObjectFactory::NORMAL_SCHEMA)) {
            res = new ObjectSchema();
        }
        if (res == NULL) {
            Error::addError(new FieldIllegalError(filePath, typeEle->line, typeEle->col, "schema.type"));
            return NULL;
        }
    }

    /**
     * Handle common fields
     */
    /* title */
    DocElement *titleEle = NULL;
    if (schemaType == NORMAL_SCHEMA) titleEle = obj->get("title");
    if (schemaType == PARAMETER_SCHEMA) titleEle = obj->get("name");
    if (titleEle) {
        if (titleEle->type != DOC_SCALAR) {
            Error::addError(new FieldInvalidError(filePath, titleEle->line, titleEle->col, "schema.title", titleEle->type, DOC_SCALAR));
            delete res;
            return NULL;
        } else {
            res->title = ((DocScalarElement*)titleEle)->getValue();
        }
    }

    /* description */
    DocElement *descriptionEle = NULL;
    if ((schemaType == NORMAL_SCHEMA) || (schemaType == PARAMETER_SCHEMA)) descriptionEle = obj->get("description");
    if (descriptionEle) {
        if (descriptionEle->type != DOC_SCALAR) {
            Error::addError(new FieldInvalidError(filePath, descriptionEle->line, descriptionEle->col, "schema.description", descriptionEle->type, DOC_SCALAR));
            delete res;
            return NULL;
        } else {
            res->description = ((DocScalarElement*)descriptionEle)->getValue();
        }
    }

    /* allowEmptyValue */
    if ((schemaType == NORMAL_SCHEMA) || (schemaType == PARAMETER_SCHEMA)) {
        DocElement *allowEmptyValueEle = obj->get("allowEmptyValue");
        if (allowEmptyValueEle) {
            pair<bool, bool> parseRes = DocElementHelper::parserToBool(allowEmptyValueEle);
            bool allowEmptyValue = parseRes.first, legal = parseRes.second;
            if (!legal) {
                Error::addError(
                        new FieldIllegalError(filePath, allowEmptyValueEle->line, allowEmptyValueEle->col,
                                              "schema.allowEmptyValue"));
                delete res;
                return NULL;
            } else {
                res->allowEmptyValue = allowEmptyValue;
            }
        }
    }

    /* default */
    DocElement *defaultEle = obj->get("default");
    if (defaultEle != NULL) {
        res->_default = DataObjectAdapter::defaultFromDocElement(defaultEle);
    }

    /* enum */
    DocElement *enumEle = obj->get("enum");
    if (enumEle != NULL) {
        res->rawEnum = enumEle;
    }

    /* x-emptyProbability */
    if ((schemaType == NORMAL_SCHEMA) || (schemaType == PARAMETER_SCHEMA)) {
        DocElement *emptyProbabilityEle = obj->get("x-emptyProbability");
        if (emptyProbabilityEle) {
            pair<double, bool> parseRes = DocElementHelper::parseToDouble(emptyProbabilityEle);
            double emptyProbability = parseRes.first;
            bool legal = parseRes.second;
            if ((!legal) || (emptyProbability < -1e-6) || (emptyProbability > 1.0 + 1e-6)) {
                Error::addError(
                        new FieldIllegalError(filePath, emptyProbabilityEle->line, emptyProbabilityEle->col,
                                              "schema.x-emptyProbability")
                );
                delete res;
                return NULL;
            } else {
                res->emptyProbability = emptyProbability;
            }
        }
    } else {
        res->emptyProbability = 0.0;
    }

    /* externalDocs */
    if (schemaType == NORMAL_SCHEMA) {
        DocElement *externalDocsEle = obj->get("externalDocs");
        if (externalDocsEle) {
            if (externalDocsEle->type != DOC_OBJECT) {
                Error::addError(
                        new FieldInvalidError(filePath, externalDocsEle->line, externalDocsEle->col,
                                              "schema.externalDocs", externalDocsEle->type, DOC_OBJECT)
                );
                delete res;
                return NULL;
            } else {
                ExternalDocObject *externalDocsObj = ExternalDocObjectFactory::create(filePath, "schema.externalDocs",
                                                                                      (DocObjectElement *) externalDocsEle);
                if (externalDocsObj) {
                    res->externalDocs = externalDocsObj;
                } else {
                    delete res;
                    return NULL;
                }
            }
        }
    }

    /* example */
    if (schemaType == NORMAL_SCHEMA) {
        DocElement *exampleEle = obj->get("example");
        if (exampleEle != NULL)
            res->example = DataObjectAdapter::defaultFromDocElement(exampleEle);
    }

    /* xml */
    if (schemaType == NORMAL_SCHEMA) {
        DocElement *xmlEle = obj->get("xml");
        if (xmlEle != NULL) {
            if (xmlEle->type != DOC_OBJECT) {
                Error::addError(
                        new FieldInvalidError(filePath, xmlEle->line, xmlEle->col,
                        "schema.xml", xmlEle->type, DOC_OBJECT)
                );
                delete res;
                return NULL;
            } else {
                XMLObject *xmlObj = XMLObjectFactory::create(filePath, "schema.xml", (DocObjectElement*)xmlEle);
                if (xmlObj) {
                    if (res->type == DataSchemaObject::TYPE_ARRAY)
                        xmlObj->availWrapped = true;
                    if (inProperty)
                        xmlObj->availAttribute = true;
                    res->xml = xmlObj;
                } else {
                    delete res;
                    return NULL;
                }
            }
        }
    }

    /**
     * Handle type-specified fields
     */
    if (!res->init(filePath, obj, schemaType)) {
        delete res;
        return NULL;
    }

    /**
     * Transform and check enums
     */
    if (res->rawEnum) {
        if (res->rawEnum->type != DOC_SEQUENCE) {
            Error::addError(
                    new FieldInvalidError(filePath, res->rawEnum->line, res->rawEnum->col,
                    "schema.enum", res->rawEnum->type, DOC_SEQUENCE)
            );
            delete res;
            return NULL;
        } else {
            DocSequenceElement *inq = (DocSequenceElement*)(res->rawEnum);
            int len = inq->getLength();
            BaseDataObject **tmp = new BaseDataObject*[len + 1];
            memset(tmp, 0, sizeof(BaseDataObject*) * (len + 1));
            bool noErr = true;
            for (int i=0; i<len; ++i) {
                DocElement *item = inq->get(i);
                res->valid = true;
                BaseDataObject *out = res->transform(item);
                res->valid = false;
                if (out) {
                    tmp[i] = out;
                } else {
                    noErr = false;
                }
            }
            if (noErr) {
                for (int i=0; i<len; ++i)
                    res->_enum.push_back(tmp[i]);
                delete[] tmp;
            } else {
                delete[] tmp;
                Error::addError(
                        new IllegalEnumObjectError(filePath, inq->line, inq->col, "schema.enum")
                );
                delete res;
                return NULL;
            }
        }
    }

    res->valid = true;
    return res;
}

bool DataSchemaObject::check(BaseDataObject *obj) {
    /**
     * For base, check whether empty & whether in enum
     */
    if (!this->valid) return false;

    if ((!this->allowEmptyValue) || (this->emptyProbability < 1e-6)) {
        if (obj == NULL) return false;
    }

    if (this->_enum.size() > 0) {
        bool match = false;
        for (int i=0; i<this->_enum.size(); ++i) {
            if (this->_enum[i]->equals(obj)) {
                match = true;
                break;
            }
        }
        if (!match) return false;
    }

    return true;
}

BaseDataObject *DataSchemaObject::generate() {
    if ((!this->valid) || (this->_enum.size() == 0)) return NULL;
    int pick_no = (rand() & 0x7FFFFFFF) % (int)this->_enum.size();
    return DataObjectAdapter::deepCopy(this->_enum[pick_no]);
}

double DataSchemaObject::randomReal() {
    return (double)(rand()) / (double)(RAND_MAX);
}