//
// Created by lly on 17/07/2017.
//

#include "ArraySchema.h"
#include "IntegerSchema.h"

ArraySchema::ArraySchema(): DataSchemaObject() {
    type = TYPE_ARRAY;
    items = NULL;
    collectionFormat = CSV;
    maxItems = 255;
    minItems = 0;
    uniqueItems = false;
}

BaseDataObject *ArraySchema::toDataObject() {
    BaseDataObject *obj = DataSchemaObject::toDataObject();
    // --- above are routine ---
    if (obj) {
        ObjectDataObject *o = (ObjectDataObject*)obj;
        (*o)["items"] = this->items->toDataObject();
        (*o)["maxItems"] = new IntegerDataObject(this->maxItems);
        (*o)["minItems"] = new IntegerDataObject(this->minItems);
        (*o)["uniqueItems"] = new BooleanDataObject(this->uniqueItems);
        switch (this->collectionFormat) {
            case CSV:
                (*o)["collectionFormat"] = new StringDataObject("csv");
                break;
            case SSV:
                (*o)["collectionFormat"] = new StringDataObject("ssv");
                break;
            case TSV:
                (*o)["collectionFormat"] = new StringDataObject("tsv");
                break;
            case PIPES:
                (*o)["collectionFormat"] = new StringDataObject("pipes");
                break;
            case MULTI:
                (*o)["collectionFormat"] = new StringDataObject("multi");
                break;
        }
    }
    return obj;
}

bool ArraySchema::check(BaseDataObject *obj) {
    if (DataSchemaObject::check(obj)) {
        if (obj == NULL) return true;
        // allow empty value
        // --- above are routine ---
        if (obj->type == SEQUENCE) {
            SequenceDataObject *o = (SequenceDataObject*)obj;
            int len = o->length();
            if ((len > maxItems) || (len < minItems)) return false;
            bool ok = true;
            for (int i=0; i<len; ++i)
                if (((*o)[i] == NULL) || (!items->check((*o)[i]))) {
                    ok = false;
                    break;
                }
            if (!ok) return false;
            if (uniqueItems) {
                for (int i=0; i<len; ++i) {
                    for (int j = 0; j < i - 1; ++j)
                        if ((*o)[i]->equals((*o)[j])) {
                            ok = false;
                            break;
                        }
                    if (!ok) break;
                }
                if (!ok) return false;
            }
            return true;
        }
    }
    return false;
}

BaseDataObject *ArraySchema::transform(DocElement *ele) {
    if (!this->valid) return NULL;
    if (ele == NULL) return NULL;
    // --- above are routine ---
    if (ele->type == DOC_SEQUENCE) {
        DocSequenceElement *e = (DocSequenceElement*)ele;
        SequenceDataObject *seq = new SequenceDataObject();
        bool legal = true;
        int len = e->getLength();
        for (int i=0; i<len; ++i) {
            DocElement *item = e->get(i);
            BaseDataObject *obj = this->items->transform(item);
            if (obj == NULL) {
                legal = false;
                break;
            }
            seq->push(obj);
        }
        if ((!legal) || (!check(seq))) {
            delete seq;
            return NULL;
        }
        return seq;
    }
    return NULL;
}

BaseDataObject *ArraySchema::generate() {
    BaseDataObject *enumGen = DataSchemaObject::generate();
    if (enumGen) return enumGen;
    if (!this->valid) return NULL;
    if ((DataSchemaObject::randomReal() < emptyProbability) && (this->allowEmptyValue)) return NULL;
    // --- above are routine ---
    int n = rand() % (maxItems - minItems + 1) + minItems;
    SequenceDataObject *seq = new SequenceDataObject();
    for (int i=0; i<n; ++i) {
        BaseDataObject *now = NULL;
        bool repeated = false;
        int cnt = 0;
        do {
            /** If requires uniqueness,
             * We'll try 100 times searching for a new one.
             * If can't find, just give up.
             */
            now = items->generate();
            if (this->uniqueItems)
                for (int j=0; j<i-1; ++j)
                    if ((*seq)[j]->equals(now)) {
                        repeated = true;
                        delete now;
                        now = NULL;
                        break;
                    }
            ++cnt;
        } while ((cnt <= 100) && (repeated));
        if (now)
            seq->push(now);
    }
    return seq;
}

bool ArraySchema::init(string filePath, DocObjectElement *obj, int schemaType, ObjectSerialType inherentType) {

    /* items */
    DocElement *itemsObj = obj->get("items");
    if (itemsObj == NULL) {
        Error::addError(
                new FieldMissError(filePath, obj->line, obj->col, "schema.items")
        );
        return false;
    }
    if (itemsObj->type != DOC_OBJECT) {
        Error::addError(
                new FieldInvalidError(filePath, itemsObj->line, itemsObj->col, "schema.items", itemsObj->type, DOC_OBJECT)
        );
        return false;
    }
    DataSchemaObject *itemsSchema = DataSchemaObjectFactory::create(filePath, (DocObjectElement*)itemsObj,
                                                                    DataSchemaObjectFactory::ITEM_SCHEMA, false, inherentType);
    if (itemsSchema == NULL)
        return false;
    this->items = itemsSchema;

    /* maxItems */
    DocElement *maxItemsObj = obj->get("maxItems");
    if (maxItemsObj) {
        pair<long long, bool> ret = DocElementHelper::parseToInt(maxItemsObj);
        if (ret.second && (ret.first >= 0LL)) {
            this->maxItems = (int)ret.first;
        } else {
            Error::addError(
                    new FieldIllegalError(filePath, maxItemsObj->line, maxItemsObj->col, "schema.maxItems")
            );
            return false;
        }
    }

    /* minItems */
    DocElement *minItemsObj = obj->get("minItems");
    if (minItemsObj) {
        pair<long long, bool> ret = DocElementHelper::parseToInt(minItemsObj);
        if (ret.second && (ret.first >= 0LL)) {
            this->minItems = (int)ret.first;
        } else {
            Error::addError(
                    new FieldIllegalError(filePath, minItemsObj->line, minItemsObj->col, "schema.minItems")
            );
            return false;
        }
    }

    /* uniqueItems */
    DocElement *uniqueObj = obj->get("uniqueItems");
    if (uniqueObj) {
        pair<bool, bool> ret = DocElementHelper::parseToBool(uniqueObj);
        if (ret.second) {
            this->uniqueItems = ret.first;
        } else {
            Error::addError(
                    new FieldIllegalError(filePath, uniqueObj->line, uniqueObj->col, "schema.uniqueItems")
            );
            return false;
        }
    }

    /* collectionFormat */
    if ((DataSchemaObjectFactory::PARAMETER_SCHEMA == schemaType) ||
            (DataSchemaObjectFactory::ITEM_SCHEMA == schemaType)) {
        DocElement *cObj = obj->get("collectionFormat");
        if (cObj) {
            pair<string, bool> ret = DocElementHelper::parseToString(cObj);
            if (ret.second) {
                string &s = ret.first;
                if (s == "csv") {
                    this->collectionFormat = CSV;
                } else if (s == "ssv") {
                    this->collectionFormat = SSV;
                } else if (s == "tsv") {
                    this->collectionFormat = TSV;
                } else if (s == "pipes") {
                    this->collectionFormat = PIPES;
                } else if (s == "multi") {
                    this->collectionFormat = MULTI;
                } else {
                    Error::addError(
                            new FieldIllegalError(filePath, cObj->line, cObj->col, "schema.collectionFormat")
                    );
                    return false;
                }
            } else {
                Error::addError(
                        new FieldInvalidError(filePath, cObj->line, cObj->col, "schema.collectionFormat", cObj->type,
                                              DOC_SCALAR)
                );
                return false;
            }
        }
    }

    if (this->minItems > this->maxItems) {
        Error::addError(
                new IllegalIntervalError(filePath, obj->line, obj->col, "schema", (double)this->minItems, (double)this->maxItems)
        );
        return false;
    }

    return true;
}

ArraySchema::~ArraySchema() {
    delete items;
}

DataSchemaObject *ArraySchema::findField(const vector<string> &fieldVec, int index) {
    if (index == fieldVec.size()) return this;
    // "size" extension
    if (fieldVec[index] == "size")
        return IntegerSchema::getSizeFieldSchema()->findField(fieldVec, index + 1);
    size_t *errP = new size_t();
    stoi(fieldVec[index], errP, 10);
    if (*errP != fieldVec[index].length()) {
        delete errP;
        return NULL;
    }
    delete errP;
    return items->findField(fieldVec, index + 1);
}