//
// Created by lly on 17/07/2017.
//

#include "NumberSchema.h"


NumberSchema::NumberSchema(): DataSchemaObject() {
    type = TYPE_NUMBER;
    maximum = DBL_MAX;
    minimum = DBL_MIN;
    exclusiveMaximum = false;
    exclusiveMinimum = false;
    hasMultipleOf = false;
    multipleOf = 1.0;
}

BaseDataObject *NumberSchema::toDataObject() {
    BaseDataObject *obj = DataSchemaObject::toDataObject();
    // --- above are routine ---
    if (obj) {
        ObjectDataObject *o = (ObjectDataObject*)obj;
        (*o)["maximum"] = new NumberDataObject(maximum);
        (*o)["minimum"] = new NumberDataObject(minimum);
        (*o)["exclusiveMaximum"] = new BooleanDataObject(exclusiveMaximum);
        (*o)["exclusiveMinimum"] = new BooleanDataObject(exclusiveMinimum);
        if (hasMultipleOf)
            (*o)["multipleOf"] = new NumberDataObject(multipleOf);
    }
    return obj;
}

bool NumberSchema::check(BaseDataObject *obj) {
    if (DataSchemaObject::check(obj)) {
        if (obj == NULL) return true;
        // allow empty value
    // --- above are routine ---
        if (obj->type == NUMBER) {
            double &now = ((NumberDataObject*)obj)->value;
            if ((now + EPS < minimum) || (now - EPS > maximum))
                return false;
            else {
                if (hasMultipleOf) {
                    if (abs(fmod(now, multipleOf)) > EPS)
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

BaseDataObject *NumberSchema::transform(DocElement *ele) {
    if (!this->valid) return NULL;
    if (ele == NULL) return NULL;
    // --- above are routine ---
    pair<double, bool> parseRes = DocElementHelper::parseToDouble(ele);
    if (parseRes.second) {
        double &now = parseRes.first;
        NumberDataObject *obj = new NumberDataObject(now);
        if (check(obj))
            return obj;
        delete obj;
    }
    return NULL;
}

BaseDataObject *NumberSchema::generate() {

    BaseDataObject *enumGen = DataSchemaObject::generate();
    if (enumGen) return enumGen;
    if (!this->valid) return NULL;
    if (DataSchemaObject::randomReal() < emptyProbability) return NULL;
    // --- above are routine ---

    /**
     * To prevent overflow,
     *  when the range is too large and comparable to double range,
     *      we generate by bit-wise random.
     *  Otherwise use uniform distribution in the interval.
     */
    srand((unsigned int)time(0));

    bool large = false;

    if ((maximum > DBL_MAX / 4.0) && (minimum < DBL_MIN / 4.0)) large = true;
    if ((minimum <= 0.0) && (maximum >= 0.0) &&
            ((minimum < DBL_MIN / 4.0) || (maximum > DBL_MAX / 4.0)))
        large = true;
    if (!large)
        if (maximum - minimum > DBL_MAX / 2.0) large = true;
    if (large) {
        union {long long v1; double v2;} u;
        do {
            long long t1 = rand() & 0xFFFF, t2 = rand() & 0xFFFF,
                    t3 = rand() & 0xFFFF, t4 = rand() & 0xFFFF;
            u.v1 = (t1 << 48LL) + (t2 << 32LL) + (t3 << 16LL) + t4;
            if (hasMultipleOf)
                u.v2 = u.v2 - fmod(u.v2, multipleOf);
        } while ((u.v2 > minimum) && (u.v2 < maximum));
        return new NumberDataObject(u.v2);
    } else {
        if (maximum - minimum < EPS) {
            return new NumberDataObject((minimum + maximum) / 2.0);
        } else {
            double identity, v;
            do {
                identity = double(rand()) / double(RAND_MAX);
                v = minimum + identity * (maximum - minimum);
                if (hasMultipleOf)
                    v = v - fmod(v, multipleOf);
            }  while ((v > minimum) && (v < maximum));
            return new NumberDataObject(v);
        }
    }
}

bool NumberSchema::init(string filePath, DocObjectElement *obj, int schemaType) {

    /* format */
    /* format content doesn't matter, but we inspect its value to be in domain set */
    DocElement *formatEle = obj->get("format");
    if (formatEle) {
        pair<string, bool> res = DocElementHelper::parseToString(formatEle);
        if (res.second) {
            bool il = false;
            if (res.first == "float") {
                this->maximum = FLT_MAX;
                this->minimum = FLT_MIN;
                il = true;
            }
            if (res.first == "double") {
                this->maximum = DBL_MAX;
                this->minimum = DBL_MIN;
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
        pair<double, bool> res = DocElementHelper::parseToDouble(maximumEle);
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
        pair<double, bool> res = DocElementHelper::parseToDouble(minimumEle);
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
        pair<double, bool> res = DocElementHelper::parseToDouble(multipleOfEle);
        if ((res.second) && (res.first > EPS)) {
            this->multipleOf = res.first;
        } else {
            Error::addError(
                    new FieldIllegalError(filePath, multipleOfEle->line, multipleOfEle->col, "schema.multipleOf")
            );
            return false;
        }
    }

    if (maximum - minimum < EPS) {
        if ((maximum - minimum > -EPS) && (!exclusiveMinimum) && (!exclusiveMaximum))
            /* that's ok, domain = {minimum(maximum)} */;
        else {
            Error::addError(
                    new IllegalIntervalError(filePath, obj->line, obj->col, "schema", minimum, maximum)
            );
            return false;
        }
    }

    if (hasMultipleOf) {
        /* Check whether the domain set is empty */
        double v1 = minimum - fmod(minimum, multipleOf);
        double v2 = maximum - fmod(maximum, multipleOf);
        if ((abs(v1 - v2) < EPS) && ((v1 < minimum) || (v2 > maximum))) {
            Error::addError(
                    new FieldIllegalError(filePath, multipleOfEle->line, multipleOfEle->col, "scheme.multipleOf")
            );
            return false;
        }
    }

    return true;
}