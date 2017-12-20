//
// Created by lly on 17/07/2017.
//

#include "BooleanSchema.h"

BooleanSchema::BooleanSchema(): DataSchemaObject() {
    type = TYPE_BOOLEAN;
    trueString = "true";
    falseString = "false";
}

BaseDataObject *BooleanSchema::toDataObject() {
    BaseDataObject *obj = DataSchemaObject::toDataObject();
    // --- above are routine ---
    if (obj) {
        ObjectDataObject *o = (ObjectDataObject*)obj;
        if (trueString != "true") {
            (*o)["x-trueString"] = new StringDataObject(trueString);
        }
        if (falseString != "false") {
            (*o)["x-falseString"] = new StringDataObject(falseString);
        }
    }
    return obj;
}

bool BooleanSchema::check(BaseDataObject *obj) {
    if (DataSchemaObject::check(obj)) {
        if (obj == NULL) return true;
        // allow empty value
        // --- above are routine ---
        if (obj->type == STRING) {
            StringDataObject *o = (StringDataObject*)obj;
            if ((o->str == trueString) || (o->str == falseString))
                return true;
        }
        if (obj->type == BOOLEAN) {
            return true;
        }
    }
    return false;
}

BaseDataObject *BooleanSchema::transform(DocElement *ele) {
    if (!this->valid) return NULL;
    if (ele == NULL) return NULL;
    // --- above are routine ---
    pair<bool, bool> parseRes = DocElementHelper::parseToBool(ele, this->trueString, this->falseString);
    if (parseRes.second) {
        bool x = parseRes.first;
        if (x) {
            if (trueString == "true")
                return new BooleanDataObject(true);
            else
                return new StringDataObject(trueString);
        } else {
            if (falseString == "false")
                return new BooleanDataObject(false);
            else
                return new StringDataObject(falseString);
        }
    }
    return NULL;
}

BaseDataObject *BooleanSchema::generate() {
    BaseDataObject *enumGen = DataSchemaObject::generate();
    if (enumGen) return enumGen;
    if (!this->valid) return NULL;
    if ((DataSchemaObject::randomReal() < emptyProbability) && (this->allowEmptyValue)) return NULL;
    // --- above are routine ---
    bool x = (bool)(rand() & 1);
    if (x) {
        if (trueString == "true")
            return new BooleanDataObject(true);
        else
            return new StringDataObject(trueString);
    } else {
        if (falseString == "false")
            return new BooleanDataObject(false);
        else
            return new StringDataObject(falseString);
    }
}

bool BooleanSchema::init(string filePath, DocObjectElement *obj, int schemaType, ObjectSerialType inherentType) {

    /* x-trueString */
    DocElement *trueStrEle = obj->get("x-trueString");
    if (trueStrEle) {
        pair<string, bool> res = DocElementHelper::parseToString(trueStrEle);
        if (res.second) {
            this->trueString = res.first;
        } else {
            Error::addError(
                    new FieldInvalidError(filePath, trueStrEle->line, trueStrEle->col, "schema.x-trueString",
                                          trueStrEle->type, DOC_SCALAR)
            );
            return false;
        }
    }

    /* x-falseString */
    DocElement *falseStrEle = obj->get("x-falseString");
    if (falseStrEle) {
        pair<string, bool> res = DocElementHelper::parseToString(falseStrEle);
        if (res.second) {
            this->falseString = res.first;
        } else {
            Error::addError(
                    new FieldInvalidError(filePath, falseStrEle->line, falseStrEle->col, "schema.x-falseString",
                                          falseStrEle->type, DOC_SCALAR)
            );
            return false;
        }
    }

    return true;
}

DataSchemaObject *BooleanSchema::findField(const vector<string> &fieldVec, int index) {
    if (index == fieldVec.size())
        return this;
    else
        return NULL;
}