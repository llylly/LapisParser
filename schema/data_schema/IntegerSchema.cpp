//
// Created by lly on 17/07/2017.
//

#include "IntegerSchema.h"


IntegerSchema::IntegerSchema(): DataSchemaObject() {
    type = TYPE_INTEGER;
    maximum = LONG_LONG_MAX;
    minimum = LONG_LONG_MIN;
    exclusiveMaximum = false;
    exclusiveMinimum = false;
    hasMultipleOf = false;
    multipleOf = 1LL;
}

BaseDataObject *IntegerSchema::toDataObject() {
    BaseDataObject *obj = DataSchemaObject::toDataObject();
    // --- above are routine ---
    if (obj) {
        ObjectDataObject *o = (ObjectDataObject*)obj;
        (*o)["maximum"] = new IntegerDataObject(maximum);
        (*o)["minimum"] = new IntegerDataObject(minimum);
        (*o)["exclusiveMaximum"] = new BooleanDataObject(exclusiveMaximum);
        (*o)["exclusiveMinimum"] = new BooleanDataObject(exclusiveMinimum);
        if (hasMultipleOf)
            (*o)["multipleOf"] = new IntegerDataObject(multipleOf);
    }
    return obj;
}

bool IntegerSchema::check(BaseDataObject *obj) {
    if (DataSchemaObject::check(obj)) {
        if (obj == NULL) return true;
        // allow empty value
        // --- above are routine ---
        if (obj->type == INTEGER) {
            long long &now = ((IntegerDataObject*)obj)->value;
            long long s = minimum + (long long)exclusiveMinimum, t = maximum - (long long)exclusiveMaximum;
            if ((now < s) || (now > t))
                return false;
            else {
                if (hasMultipleOf) {
                    if ((now % multipleOf) != 0)
                        return false;
                    else
                        return true;
                } else
                    return true;
            }
        }
    }
    return false;
}

BaseDataObject *IntegerSchema::transform(DocElement *ele) {
    if (!this->valid) return NULL;
    if (ele == NULL) return NULL;
    // --- above are routine ---
    pair<long long, bool> parseRes = DocElementHelper::parseToInt(ele);
    if (parseRes.second) {
        long long &now = parseRes.first;
        IntegerDataObject *obj = new IntegerDataObject(now);
        if (check(obj))
            return obj;
        delete obj;
    }
    return NULL;
}

BaseDataObject *IntegerSchema::generate() {
    BaseDataObject *enumGen = DataSchemaObject::generate();
    if (enumGen) return enumGen;
    if (!this->valid) return NULL;
    if ((DataSchemaObject::randomReal() < emptyProbability) && (this->allowEmptyValue)) return NULL;
    // --- above are routine ---
    long long v;
    long long s = minimum + (long long)exclusiveMinimum, t = maximum - (long long)exclusiveMaximum;
    long long gap = t - s + 1;
    bool large = false;
    if ((s <= 0) && (t > 0) && ((s < LONG_LONG_MIN / 4L) || (t > LONG_LONG_MAX / 4L))) large = true;
    do {
        long long t1 = rand() & 0xFFFF, t2 = rand() & 0xFFFF, t3 = rand() & 0xFFFF, t4 = rand() & 0xFFFF;
        long long rd = (t1 << 48LL) + (t2 << 32LL) + (t3 << 16LL) + t4;
        if (large) v = rd; else v = s + rd % gap;
        if (hasMultipleOf)
            v = v - v % multipleOf;
    } while ((v < s) || (v > t));
    return new IntegerDataObject(v);
}

bool IntegerSchema::init(string filePath, DocObjectElement *obj, int schemaType) {

    /* format */
    /* format content doesn't matter, but we inspect its value to be in domain set */
    DocElement *formatEle = obj->get("format");
    if (formatEle) {
        pair<string, bool> res = DocElementHelper::parseToString(formatEle);
        if (res.second) {
            bool il = false;
            if (res.first == "int32") {
                this->maximum = INT_MAX;
                this->minimum = INT_MIN;
                il = true;
            }
            if (res.first == "int64") {
                this->maximum = LONG_LONG_MAX;
                this->minimum = LONG_LONG_MIN;
                il = true;
            }
            if (!il) {
                Error::addError(
                        new IllegalEnumObjectError(filePath, formatEle->line, formatEle->col, "schema.format")
                );
                return false;
            }
        } else {
            Error::addError(
                    new FieldInvalidError(filePath, formatEle->line, formatEle->col, "schema.format", formatEle->type, DOC_SCALAR)
            );
            return false;
        }
    }

    /* maximum */
    DocElement *maximumEle = obj->get("maximum");
    if (maximumEle) {
        pair<long long, bool> res = DocElementHelper::parseToInt(maximumEle);
        if (res.second) {
            this->maximum = res.first;
        } else {
            Error::addError(
                    new FieldIllegalError(filePath, maximumEle->line, maximumEle->col, "schema.maximum")
            );
            return false;
        }
    }

    /* minimum */
    DocElement *minimumEle = obj->get("minimum");
    if (minimumEle) {
        pair<long long, bool> res = DocElementHelper::parseToInt(minimumEle);
        if (res.second) {
            this->minimum = res.first;
        } else {
            Error::addError(
                    new FieldIllegalError(filePath, minimumEle->line, minimumEle->col, "schema.minimum")
            );
            return false;
        }
    }

    /* exclusiveMaximum */
    DocElement *exclusiveMaxEle = obj->get("exclusiveMaximum");
    if (exclusiveMaxEle) {
        pair<bool, bool> res = DocElementHelper::parserToBool(exclusiveMaxEle);
        if (res.second) {
            this->exclusiveMaximum = res.first;
        } else {
            Error::addError(
                    new FieldIllegalError(filePath, exclusiveMaxEle->line, exclusiveMaxEle->col, "schema.exclusiveMaximum")
            );
            return false;
        }
    }

    /* exclusiveMinimum */
    DocElement *exclusiveMinEle = obj->get("exclusiveMinimum");
    if (exclusiveMinEle) {
        pair<bool, bool> res = DocElementHelper::parserToBool(exclusiveMinEle);
        if (res.second) {
            this->exclusiveMinimum = res.first;
        } else {
            Error::addError(
                    new FieldIllegalError(filePath, exclusiveMinEle->line, exclusiveMinEle->col, "schema.exclusiveMinimum")
            );
            return false;
        }
    }

    /* multipleOf */
    DocElement *multipleOfEle = obj->get("multipleOf");
    if (multipleOfEle) {
        pair<long long, bool> res = DocElementHelper::parseToInt(multipleOfEle);
        if ((res.second) && (res.first > 0)) {
            this->multipleOf = res.first;
        } else {
            Error::addError(
                    new FieldIllegalError(filePath, multipleOfEle->line, multipleOfEle->col, "schema.multipleOf")
            );
            return false;
        }
    }

    if (exclusiveMaximum & exclusiveMinimum)
        if ((minimum > maximum) || ((minimum <= maximum) && (maximum - minimum < 2))) {
            Error::addError(
                    new IllegalIntervalError(filePath, obj->line, obj->col, "schema", (double)minimum, (double)maximum)
            );
            return false;
        }

    if (exclusiveMaximum || exclusiveMinimum)
        if ((minimum > maximum) || ((minimum <= maximum) && (maximum - minimum < 1))) {
            Error::addError(
                    new IllegalIntervalError(filePath, obj->line, obj->col, "schema", (double)minimum, (double)maximum)
            );
            return false;
        }

    if (minimum > maximum) {
        Error::addError(
                new IllegalIntervalError(filePath, obj->line, obj->col, "schema", (double)minimum, (double)maximum)
        );
        return false;
    }

    if (hasMultipleOf) {
        /* Check whether the domain set is empty */
        long long s = minimum + (long long)exclusiveMinimum, t = maximum - (long long)exclusiveMaximum;
        long long v1 = s - s % multipleOf;
        long long v2 = t - t % multipleOf;
        if ((v1 == v2) && ((v1 < s) || (v2 > t))) {
            Error::addError(
                    new FieldIllegalError(filePath, multipleOfEle->line, multipleOfEle->col, "scheme.multipleOf")
            );
            return false;
        }
    }

    return true;
}

DataSchemaObject *IntegerSchema::findField(const vector<string> &fieldVec, int index) {
    if (index == fieldVec.size())
        return this;
    else
        return NULL;
}