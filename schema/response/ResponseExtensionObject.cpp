//
// Created by lly on 23/07/2017.
//

#include "ResponseExtensionObject.h"

BaseDataObject *ResponseExtensionObject::toDataObject() {
    BaseDataObject *obj = AbstractResponseObject::toDataObject();
    if (obj == NULL) return NULL;
    ObjectDataObject *o = (ObjectDataObject*)obj;
    (*o)["name"] = new StringDataObject(this->name);
    (*o)["field"] = new StringDataObject(this->field);
    SequenceDataObject *seq = new SequenceDataObject();
    for (vector<vector<string>>::iterator ite = this->relatedParameters.begin();
            ite != this->relatedParameters.end();
            ++ite) {
        string s = "in.";
        vector<string> &now = *ite;
        for (int i=0; i<now.size(); ++i) {
            s += now[i];
            if (i < now.size() - 1)
                s += ".";
        }
        seq->push(new StringDataObject(s));
    }
    (*o)["relatedParameters"] = seq;
    return o;
}

ResponseExtensionObject::ResponseExtensionObject(): AbstractResponseObject() {
    type = EXTENSION;
    name = "";
    field = "";
    relatedParameters.clear();
    fieldVec.clear();
}

ResponseExtensionObject::ResponseExtensionObject(const ResponseExtensionObject &source): AbstractResponseObject(source) {
    this->fieldVec.clear();
    for (vector<string>::const_iterator ite = source.fieldVec.cbegin();
            ite != source.fieldVec.cend();
            ++ite) {
        this->fieldVec.push_back(*ite);
    }

    this->relatedParameters.clear();
    for (vector<vector<string>>::const_iterator ite = source.relatedParameters.cbegin();
            ite != source.relatedParameters.cend();
            ++ite) {
        vector<string> nowVec;
        nowVec.clear();
        for (vector<string>::const_iterator iite = ite->cbegin();
                iite != ite->cend();
                ++iite)
            nowVec.push_back(*iite);
        this->relatedParameters.push_back(nowVec);
    }
}

bool ResponseExtensionObject::createRaw(string filePath, DocObjectElement *ele, DataSchemaPool *pool) {
    if (ele == NULL) {
        Error::addError(
                new FieldMissError(filePath, 1, 1, "response")
        );
        return false;
    }

    /* name */
    DocElement *nameEle = ele->get("name");
    if (nameEle == NULL) {
        Error::addError(
                new FieldMissError(filePath, ele->line, ele->col, "response.name")
        );
        return false;
    }
    pair<string, bool> nameRes = DocElementHelper::parseToString(nameEle);
    if (nameRes.second) {
        this->name = nameRes.first;
    } else {
        Error::addError(
                new FieldInvalidError(filePath, nameEle->line, nameEle->col, "response.name", nameEle->type, DOC_SCALAR)
        );
        return false;
    }

    /* description */
    DocElement *descEle = ele->get("description");
    if (descEle) {
        pair<string, bool> descRes = DocElementHelper::parseToString(descEle);
        if (descRes.second) {
            this->description = descRes.first;
        } else {
            Error::addError(
                    new FieldInvalidError(filePath, descEle->line, descEle->col, "response.description", descEle->type, DOC_SCALAR)
            );
            return false;
        }
    }

    /* code */
    DocElement *codeEle = ele->get("code");
    if (codeEle == NULL) {
        Error::addError(
                new FieldMissError(filePath, ele->line, ele->col, "response.code")
        );
        return false;
    }
    pair<long long int, bool> codeRes = DocElementHelper::parseToInt(codeEle);
    if ((!codeRes.second) || (codeRes.first <= 0L)) {
        Error::addError(
                new FieldIllegalError(filePath, codeEle->line, codeEle->col, "response.code")
        );
        return false;
    } else {
        this->code = (int)codeRes.first;
    }

    /* field */
    DocElement *fieldEle = ele->get("field");
    if (fieldEle == NULL) {
        Error::addError(
                new FieldMissError(filePath, ele->line, ele->col, "response.field")
        );
        return false;
    }
    pair<string, bool> fieldRes = DocElementHelper::parseToString(fieldEle);
    if (fieldRes.second) {
        this->field = fieldRes.first;
    } else {
        Error::addError(
                new FieldInvalidError(filePath, fieldEle->line, fieldEle->col, "response.field", fieldEle->type, DOC_SCALAR)
        );
        return false;
    }
    bool fieldLegal = (this->field.substr(0, 4) == "out.");
    if (fieldLegal) {
        const string &s = this->field;
        int l = 3, len = (int)s.length();
        for (int i=4; i < len; ++i) {
            if (s[i] == '.') {
                if (l + 1 < i) {
                    fieldVec.push_back(s.substr((unsigned int)l + 1, (unsigned int)i - l - 1));
                    l = i;
                } else {
                    fieldLegal = false;
                    break;
                }
            }
        }
        if (l + 1 < len) {
            fieldVec.push_back(s.substr((unsigned int)l + 1, (unsigned int)len - l - 1));
        } else {
            fieldLegal = false;
        }
    }
    if (!fieldLegal) {
        Error::addError(
                new FieldIllegalError(filePath, fieldEle->line, fieldEle->col, "response.field")
        );
        return false;
    }

    /* schema */
    DocElement *schemaEle = ele->get("schema");
    if (schemaEle == NULL) {
        Error::addError(
                new FieldMissError(filePath, ele->line, ele->col, "response.schema")
        );
        return false;
    }
    this->schema = pool->parseDataSchema(filePath, schemaEle);
    if (this->schema == NULL)
        return false;

    /* relatedParameters */
    DocElement *rpEle = ele->get("relatedParameters");
    if (rpEle != NULL) {
        if (rpEle->type != DOC_SEQUENCE) {
            Error::addError(
                    new FieldInvalidError(filePath, rpEle->line, rpEle->col, "response.relatedParameters", rpEle->type, DOC_SEQUENCE)
            );
            return false;
        }
        DocSequenceElement *e = (DocSequenceElement*)rpEle;
        int len = e->getLength();
        for (int i=0; i<len; ++i) {
            DocElement *nowEle = e->get(i);
            if (nowEle->type != DOC_SCALAR) {
                Error::addError(
                        new FieldInvalidError(filePath, nowEle->line, nowEle->col, "response.relatedParameters", nowEle->type, DOC_SCALAR)
                );
                return false;
            }
            const string &s = ((DocScalarElement*)nowEle)->getValue();
            relatedParameters.push_back(vector<string>());
            vector<string> &nowVec = relatedParameters[i];

            bool legal = (s.substr(0, 3) == "in.");
            if (legal) {
                int l = 2, slen = (int)s.length();
                for (int j=3; j < slen; ++j) {
                    if (s[j] == '.') {
                        if (l + 1 < j) {
                            nowVec.push_back(s.substr((unsigned int)l + 1, (unsigned int)j - l - 1));
                            l = j;
                        } else {
                            legal = false;
                            break;
                        }
                    }
                }
                if (l + 1 < slen) {
                    nowVec.push_back(s.substr((unsigned int)l + 1, (unsigned int)slen - l - 1));
                } else {
                    legal = false;
                }
            }
            if (!legal) {
                Error::addError(
                        new FieldIllegalError(filePath, nowEle->line, nowEle->col, "response.relatedParameters")
                );
                return false;
            }
        }
    }

    this->stage = RAW;

    return true;
}

bool ResponseExtensionObject::checkResponse(DocElement *data) {
    if (this->stage == INVALID_RESPONSE)
        return false;
    if (this->schema == NULL)
        return false;
    else
        return (this->schema->transform(data) != NULL);
}