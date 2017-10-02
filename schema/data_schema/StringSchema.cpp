//
// Created by lly on 17/07/2017.
//

#include "StringSchema.h"

const int StringSchema::NONETYPE = 0;
const int StringSchema::DATE = 1;
const int StringSchema::DATETIME = 2;
const int StringSchema::PASSWORD = 3;

StringSchema::StringSchema(): DataSchemaObject() {
    type = TYPE_STRING;
    formatType = StringSchema::NONETYPE;
    maxLength = 255;
    minLength = 0;
    hasPattern = false;
    pattern = "";
}

BaseDataObject *StringSchema::toDataObject() {
    BaseDataObject *obj = DataSchemaObject::toDataObject();
    // --- above are routine ---
    if (obj) {
        ObjectDataObject *o = (ObjectDataObject*)obj;
        if (this->formatType != StringSchema::NONETYPE) {
            switch (this->formatType) {
                case StringSchema::DATE:
                    (*o)["format"] = new StringDataObject("date");
                    break;
                case StringSchema::DATETIME:
                    (*o)["format"] = new StringDataObject("dateTime");
                    break;
                case StringSchema::PASSWORD:
                    (*o)["format"] = new StringDataObject("password");
                    break;
            }
        }
        (*o)["maxLength"] = new IntegerDataObject(this->maxLength);
        (*o)["minLength"] = new IntegerDataObject(this->minLength);
        if (this->hasPattern)
            (*o)["pattern"] = new StringDataObject(this->pattern);
    }
    return obj;
}

bool StringSchema::check(BaseDataObject *obj) {
    if (DataSchemaObject::check(obj)) {
        if (obj == NULL) return true;
            // allow empty value
    // --- above are routine ---
        if (obj->type == STRING) {
            string &s = ((StringDataObject*)obj)->str;
            if ((s.length() < this->minLength) || (s.length() > this->maxLength))
                return false;
            if (this->hasPattern)
                if (!StringSchema::regexCheck(s, pattern)) return false;
            if (this->formatType == StringSchema::DATE)
                if (!StringSchema::dateCheck(s)) return false;
            if (this->formatType == StringSchema::DATETIME)
                if (!StringSchema::dateTimeCheck(s)) return false;
            /**
             * TODO
             * We should also check whether the pattern, the min length constraint,
             * the max length constraint and the format are mutually compatible.
             * i.e. Exists at least one string for all the constraints
             */
            if (!compatibleCheck()) return false;
            return true;
        }
    }
    return false;
}

BaseDataObject *StringSchema::transform(DocElement *ele) {
    if (!this->valid) return NULL;
    if (ele == NULL) return NULL;
    // --- above are routine ---
    pair<string, bool> parseRes = DocElementHelper::parseToString(ele);
    if (parseRes.second) {
        string &now = parseRes.first;
        StringDataObject *obj = new StringDataObject(now);
        if (check(obj))
            return obj;
        delete obj;
    }
    return NULL;
}

BaseDataObject *StringSchema::generate() {
    BaseDataObject *enumGen = DataSchemaObject::generate();
    if (enumGen) return enumGen;
    if (!this->valid) return NULL;
    if ((DataSchemaObject::randomReal() < emptyProbability) && (this->allowEmptyValue)) return NULL;
    // --- above are routine ---
    if (formatType == StringSchema::DATE)
        return new StringDataObject(StringSchema::generateDate(this->pattern, this->minLength, this->maxLength));
    if (formatType == StringSchema::DATETIME)
        return new StringDataObject(StringSchema::generateDateTime(this->pattern, this->minLength, this->maxLength));
    if (this->hasPattern)
        return new StringDataObject(generateFromPattern(this->pattern, this->minLength, this->maxLength));

    int len = this->minLength + (rand() & 0x7FFFFFFF) % (this->maxLength - this->minLength + 1);
    string s;
    s.clear();
    for (int i=0; i<len; ++i)
        s.append(1, (char)(rand() & 0x7F));
    return new StringDataObject(s);
}

bool StringSchema::init(string filePath, DocObjectElement *obj, int schemaType) {
    /* format */
    DocElement *formatEle = obj->get("format");
    if (formatEle) {
        pair<string, bool> formatRes = DocElementHelper::parseToString(formatEle);
        if (formatRes.second) {
            string &format = formatRes.first;
            if (format == "date")
                formatType = StringSchema::DATE;
            else if (format == "dateTime")
                formatType = StringSchema::DATETIME;
            else if (format == "password")
                formatType = StringSchema::PASSWORD;
            else {
                Error::addError(
                        new FieldIllegalError(filePath, formatEle->line, formatEle->col, "schema.format")
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

    /* maxLength */
    DocElement *maxLengthEle = obj->get("maxLength");
    if (maxLengthEle) {
        pair<int, bool> maxLengthRes = DocElementHelper::parseToInt(maxLengthEle);
        if ((maxLengthRes.second) && (maxLengthRes.first >= 0)) {
            this->maxLength = maxLengthRes.first;
        } else {
            Error::addError(
                    new FieldIllegalError(filePath, maxLengthEle->line, maxLengthEle->col, "schema.maxLength")
            );
            return false;
        }
    }

    /* minLength */
    DocElement *minLengthEle = obj->get("minLength");
    if (minLengthEle) {
        pair<int, bool> minLengthRes = DocElementHelper::parseToInt(minLengthEle);
        if ((minLengthRes.second) && (minLengthRes.first >= 0)) {
            this->minLength = minLengthRes.first;
        } else {
            Error::addError(
                    new FieldIllegalError(filePath, minLengthEle->line, minLengthEle->col, "schema.minLength")
            );
            return false;
        }
    }

    /* minLength <= maxLength */
    if (this->minLength > this->maxLength) {
        Error::addError(
                new IllegalIntervalError(filePath, obj->line, obj->col, "schema", (double)this->minLength, (double)this->maxLength)
        );
        return false;
    }

    /* pattern */
    DocElement *patternEle = obj->get("pattern");
    if (patternEle) {
        pair<string, bool> patternRes = DocElementHelper::parseToString(patternEle);
        if (patternRes.second) {
            if (StringSchema::isLegalRegex(patternRes.first)) {
                this->hasPattern = true;
                this->pattern = patternRes.first;
            } else {
                Error::addError(
                        new IllegalRegexError(filePath, patternEle->line, patternEle->col, "schema.pattern")
                );
                return false;
            }
        } else {
            Error::addError(
                    new FieldInvalidError(filePath, patternEle->line, patternEle->col, "schema.pattern", patternEle->type, DOC_SCALAR)
            );
            return false;
        }
    }
    return true;
}

bool StringSchema::compatibleCheck() {
    /**
     * TODO
     */
    return true;
}

bool StringSchema::isLegalRegex(string str) {
    /**
     * TODO
     */
    return true;
}

bool StringSchema::regexCheck(string str, string pattern) {
    /**
     * TODO
     */
    return true;
}

bool StringSchema::dateCheck(string str) {
    /**
     * TODO
     */
    return true;
}

bool StringSchema::dateTimeCheck(string str) {
    /**
     * TODO
     */
    return true;
}

string StringSchema::generateDate(string pattern, int minLength, int maxLength) {
    /**
     * TODO
     */
    return "1970-1-1";
    return "TODO: generate Date";
}

string StringSchema::generateDateTime(string pattern, int minLength, int maxLength) {
    /**
     * TODO
     */
    return "1970-1-1 00:00:00";
    return "TODO: generate DateTime";
}

string StringSchema::generateFromPattern(string pattern, int minLength, int maxLength) {
    /**
     * TODO
     */
    return "";
    return "TODO: generate from pattern";
}

DataSchemaObject *StringSchema::findField(const vector<string> &fieldVec, int index) {
    if (index == fieldVec.size())
        return this;
    else
        return NULL;
}