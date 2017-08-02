//
// Created by lly on 26/07/2017.
//

#include "APIObject.h"
#include "api/APIConstraintObject.h"

APIObject::APIObject() {
    valid = false;
    tags.clear();
    summary = "";
    description = "";
    externalDocs = NULL;
    path = "";
    name = "";
    requestMethod = GET;
    consumes.clear();
    produces.clear();
    parameters.clear();
    responses.clear();
    responseExtensions.clear();
    schemes = NULL;
    deprecated = false;
    constraint.clear();
}

APIObject::~APIObject() {
    if (externalDocs)
        delete externalDocs;
    if (schemes)
        delete schemes;
    for (vector<APIConstraintObject*>::iterator ite = constraint.begin();
            ite != constraint.end();
            ++ite)
        delete *ite;
    for (map<string, ResponseObject*>::iterator ite = responses.begin();
            ite != responses.end();
            ++ite) {
        delete ite->second;
    }
    for (map<string, vector<ResponseExtensionObject*>>::iterator ite = responseExtensions.begin();
            ite != responseExtensions.end();
            ++ite) {
        for (vector<ResponseExtensionObject*>::iterator iite = ite->second.begin();
                iite != ite->second.end();
                ++iite)
            delete *iite;
    }
}

BaseDataObject *APIObject::toDataObject() {
    if (!valid) return NULL;
    ObjectDataObject *ans = new ObjectDataObject();

    if (tags.size() > 0) {
        SequenceDataObject *tags_seq = new SequenceDataObject();
        for (vector<string>::iterator ite = tags.begin(); ite != tags.end(); ++ite)
            tags_seq->push(new StringDataObject(*ite));
        (*ans)["tags"] = tags_seq;
    }

    if (summary != "")
        (*ans)["summary"] = new StringDataObject(summary);

    if (description != "")
        (*ans)["description"] = new StringDataObject(description);

    if (externalDocs != NULL)
        (*ans)["externalDocs"] = externalDocs->toDataObject();

    (*ans)["operationId"] = new StringDataObject(path);

    SequenceDataObject *consumes_seq = new SequenceDataObject();
    for (vector<string>::iterator ite = consumes.begin(); ite != consumes.end(); ++ite)
        consumes_seq->push(new StringDataObject(*ite));
    (*ans)["consumes"] = consumes_seq;

    SequenceDataObject *produces_seq = new SequenceDataObject();
    for (vector<string>::iterator ite = produces.begin(); ite != produces.end(); ++ite)
        produces_seq->push(new StringDataObject(*ite));
    (*ans)["produces"] = produces_seq;

    SequenceDataObject *param_seq = new SequenceDataObject();
    for (map<string, ParameterObject*>::iterator ite = parameters.begin();
            ite != parameters.end();
            ++ite) {
        param_seq->push(ite->second->toDataObject());
    }
    (*ans)["parameters"] = param_seq;

    ObjectDataObject *response_o = new ObjectDataObject();
    for (map<string, ResponseObject*>::iterator ite = responses.begin();
            ite != responses.end();
            ++ite) {
        (*response_o)[ite->first] = ite->second->toDataObject();
    }
    if (responseExtensions.size() > 0) {
        SequenceDataObject *ext = new SequenceDataObject();
        for (map<string, vector<ResponseExtensionObject*>>::iterator ite = responseExtensions.begin();
                ite != responseExtensions.end();
                ++ite) {
            for (vector<ResponseExtensionObject*>::iterator iite = ite->second.begin();
                    iite != ite->second.end();
                    ++iite) {
                ext->push((*iite)->toDataObject());
            }
        }
        (*response_o)["x-extension"] = ext;
    }
    (*ans)["responses"] = response_o;

    (*ans)["schemes"] = schemes->toDataObject();

    (*ans)["deprecated"] = new BooleanDataObject(deprecated);

    if (constraint.size() > 0) {
        SequenceDataObject *cons_seq = new SequenceDataObject();
        for (vector<APIConstraintObject*>::iterator ite = constraint.begin();
                ite != constraint.end();
                ++ite) {
            cons_seq->push((*ite)->toDataObject());
        }
        (*ans)["x-constraint"] = cons_seq;
    }

    return ans;
}

bool APIObject::create(string filePath, DocObjectElement *ele, string name, APIRequestMethod method,
                       SchemesObject *rootSchemes,
                       vector<ParameterObject *> *commonParams, ParameterPool *paramPool, ResponsePool *responsePool) {

    if (ele == NULL) {
        Error::addError(
                new FieldMissError(filePath, 1, 1, "operation")
        );
        return false;
    }

    /** phase I: parse doc node **/
    {
        /** tags **/
        DocElement *tagsEle = ele->get("tags");
        if (tagsEle != NULL) {
            if (tagsEle->type != DOC_SEQUENCE) {
                Error::addError(
                        new FieldInvalidError(filePath, tagsEle->line, tagsEle->col, "operation.tags", tagsEle->type, DOC_SEQUENCE)
                );
                return false;
            } else {
                DocSequenceElement *e = (DocSequenceElement*)tagsEle;
                int len = e->getLength();
                for (int i=0; i<len; ++i) {
                    DocElement *now = e->get(i);
                    pair<string, bool> res = DocElementHelper::parseToString(now);
                    if (res.second) {
                        this->tags.push_back(res.first);
                    } else {
                        Error::addError(
                                new FieldInvalidError(filePath, now->line, now->col, "operation.tags", now->type, DOC_SCALAR)
                        );
                        return false;
                    }
                }
            }
        }

        /** summary **/
        DocElement *summaryEle = ele->get("summary");
        if (summaryEle != NULL) {
            pair<string, bool> res = DocElementHelper::parseToString(summaryEle);
            if (res.second) {
                this->summary = res.first;
            } else {
                Error::addError(
                        new FieldInvalidError(filePath, summaryEle->line, summaryEle->col, "operation.summary", summaryEle->type, DOC_SCALAR)
                );
                return false;
            }
        }

        /** description **/
        DocElement *descEle = ele->get("description");
        if (descEle != NULL) {
            pair<string, bool> res = DocElementHelper::parseToString(descEle);
            if (res.second) {
                this->description = res.first;
            } else {
                Error::addError(
                        new FieldInvalidError(filePath, descEle->line, descEle->col, "operation.description", descEle->type, DOC_SCALAR)
                );
                return false;
            }
        }

        /** external Docs **/
        DocElement *externalDocsEle = ele->get("externalDocs");
        if (externalDocsEle != NULL) {
            if (externalDocsEle->type == DOC_OBJECT) {
                ExternalDocObject *o = ExternalDocObjectFactory::create(filePath, "operation.externalDocs", (DocObjectElement*)externalDocsEle);
                if (o == NULL) {
                    return false;
                } else {
                    this->externalDocs = o;
                }
            } else {
                Error::addError(
                        new FieldInvalidError(filePath, externalDocsEle->line, externalDocsEle->col, "operation.externalDocs", externalDocsEle->type, DOC_OBJECT)
                );
                return false;
            }
        }

        /** operationId -> path **/
        this->path = name;
        DocElement *operationIdEle = ele->get("operationId");
        if (operationIdEle != NULL) {
            pair<string, bool> res = DocElementHelper::parseToString(operationIdEle);
            if (res.second) {
                this->path = res.first;
            } else {
                Error::addError(
                        new FieldInvalidError(filePath, operationIdEle->line, operationIdEle->col, "operation.operationId", operationIdEle->type, DOC_SCALAR)
                );
                return false;
            }
        }

        /** name && requestMethod **/
        this->name = name;
        this->requestMethod = method;

        /** consumes **/
        DocElement *consumesEle = ele->get("consumes");
        if (consumesEle == NULL) {
            Error::addError(
                    new FieldMissError(filePath, ele->line, ele->col, "operation.consumes")
            );
            return false;
        }
        if (consumesEle->type != DOC_SEQUENCE) {
            Error::addError(
                    new FieldInvalidError(filePath, consumesEle->line, consumesEle->col, "operation.consumes", consumesEle->type, DOC_SEQUENCE)
            );
            return false;
        }
        DocSequenceElement *o_consumesEle = (DocSequenceElement*)consumesEle;
        int consumes_len = o_consumesEle->getLength();
        if (consumes_len == 0) {
            Error::addError(
                    new FieldIllegalError(filePath, consumesEle->line, consumesEle->col, "operation.consumes")
            );
            return false;
        }
        for (int i=0; i<consumes_len; ++i) {
            DocElement *item = o_consumesEle->get(i);
            pair<string, bool> res = DocElementHelper::parseToString(item);
            if (res.second) {
                this->consumes.push_back(res.first);
            } else {
                Error::addError(
                        new FieldInvalidError(filePath, item->line, item->col, "operation.consumes", item->type, DOC_SCALAR)
                );
                return false;
            }
        }

        /** produces **/
        DocElement *producesEle = ele->get("produces");
        if (producesEle == NULL) {
            Error::addError(
                    new FieldMissError(filePath, ele->line, ele->col, "operation.produces")
            );
            return false;
        }
        if (producesEle->type != DOC_SEQUENCE) {
            Error::addError(
                    new FieldInvalidError(filePath, producesEle->line, producesEle->col, "operation.produces", producesEle->type, DOC_SEQUENCE)
            );
            return false;
        }
        DocSequenceElement *o_producesEle = (DocSequenceElement*)producesEle;
        int produces_len = o_producesEle->getLength();
        for (int i=0; i<produces_len; ++i) {
            DocElement *item = o_producesEle->get(i);
            pair<string, bool> res = DocElementHelper::parseToString(item);
            if (res.second) {
                this->produces.push_back(res.first);
            } else {
                Error::addError(
                        new FieldInvalidError(filePath, item->line, item->col, "operation.produces", item->type, DOC_SCALAR)
                );
                return false;
            }
        }

        /** parameters **/
        DocElement *paramsEle = ele->get("parameters");
        if (paramsEle != NULL) {
            if (paramsEle->type != DOC_SEQUENCE) {
                Error::addError(
                        new FieldInvalidError(filePath, paramsEle->line, paramsEle->col, "operation.parameters", paramsEle->type, DOC_SEQUENCE)
                );
                return false;
            } else {
                DocSequenceElement *so = (DocSequenceElement*)paramsEle;
                int len = so->getLength();
                for (int i=0; i<len; ++i) {
                    DocElement *nowEle = so->get(i);
                    ParameterObject *nowObj = paramPool->parseParameter(filePath, nowEle);
                    if (nowObj == NULL)
                        return false;
                    string nowName = nowObj->name;
                    if (parameters.count(nowName) > 0) {
                        Error::addError(
                                new DuplicateParameterNameError(filePath, nowEle->line, nowEle->col, "operation.parameters", nowName)
                        );
                        return false;
                    } else {
                        parameters[nowName] = nowObj;
                    }
                }
            }
        }

        /** responses **/
        DocElement *responsesEle = ele->get("responses");
        if (responsesEle == NULL) {
            Error::addError(
                    new FieldMissError(filePath, ele->line, ele->col, "operation.responses")
            );
            return false;
        }
        if (responsesEle->type != DOC_OBJECT) {
            Error::addError(
                    new FieldInvalidError(filePath, responsesEle->line, responsesEle->col, "operation.responses", responsesEle->type, DOC_OBJECT)
            );
            return false;
        }
        DocObjectElement *o_responses = (DocObjectElement*)responsesEle;
        map<string, DocElement*> *responses_map = o_responses->getMemberMap();
        for (map<string, DocElement*>::iterator ite = responses_map->begin(); ite != responses_map->end();
                ++ite) {
            const string &key = ite->first;
            if (key != "x-extension") {
                int code = -1;
                // for default, code = -1; else code = real value;
                if (key == "default") code = -1; else {
                    bool legal = true;
                    for (int i=0; i < key.length(); ++i)
                        if ((key[i] < '0') || (key[i] > '9')) {
                            legal = false;
                            break;
                        }
                    if (legal) {
                        code = stoi(key, NULL, 10);
                        if (code < 0)
                            legal = false;
                    }
                    if (!legal) {
                        Error::addError(
                                new FieldIllegalError(filePath, ite->second->line, ite->second->col, "operation.responses.key")
                        );
                        return false;
                    }
                }
                ResponseObject *source = responsePool->parseResponse(filePath, ite->second);
                if (source == NULL)
                    return false;
                ResponseObject *target = new ResponseObject(*source);
                target->code = code;
                target->stage = VALID;
                stringstream ss;
                if (key == "default") ss << "default"; else ss << code;
                responses[ss.str()] = target;
            } else {
                DocElement *responseExtensions_ele = ite->second;
                if (responseExtensions_ele->type != DOC_SEQUENCE) {
                    Error::addError(
                            new FieldInvalidError(filePath, responseExtensions_ele->line, responseExtensions_ele->col, "operation.responses.x-extension", responseExtensions_ele->type, DOC_SEQUENCE)
                    );
                    return false;
                }
                DocSequenceElement *so = (DocSequenceElement*)responseExtensions_ele;
                int len = so->getLength();
                for (int i=0; i<len; ++i) {
                    DocElement *now = so->get(i);
                    ResponseExtensionObject *obj = responsePool->parseResponseExtension(filePath, now);
                    if (obj == NULL)
                        return false;
                    ResponseExtensionObject *target = new ResponseExtensionObject(*obj);
                    target->stage = VALID;
                    stringstream ss;
                    ss << target->code;
                    responseExtensions[ss.str()].push_back(target);
                }
            }
        }

        /** schemes **/
        DocElement *schemesEle = ele->get("schemes");
        if (schemesEle != NULL) {
            if (schemesEle->type != DOC_SEQUENCE) {
                Error::addError(
                        new FieldInvalidError(filePath, schemesEle->line, schemesEle->col, "operation.schemes", schemesEle->type, DOC_SEQUENCE)
                );
                return false;
            } else {
                SchemesObject *obj = SchemesObjectFactory::create(filePath, (DocSequenceElement*)schemesEle);
                if (obj == NULL)
                    return false;
                else
                    this->schemes = obj;
            }
        }

        /** deprecated **/
        DocElement *deprecatedEle = ele->get("deprecated");
        if (deprecatedEle != NULL) {
            pair<bool, bool> res = DocElementHelper::parserToBool(deprecatedEle);
            if (res.second) {
                this->deprecated = res.first;
            } else {
                Error::addError(
                        new FieldIllegalError(filePath, deprecatedEle->line, deprecatedEle->col, "operation.deprecated")
                );
                return false;
            }
        }

        /** x-constraint **/
        DocElement *consEle = ele->get("x-constraints");
        if (consEle != NULL) {
            if (consEle->type != DOC_SEQUENCE) {
                Error::addError(
                        new FieldInvalidError(filePath, consEle->line, consEle->col, "operation.x-constraint", consEle->type, DOC_SEQUENCE)
                );
                return false;
            }
            DocSequenceElement *consSeqEle = (DocSequenceElement*)consEle;
            int len = consSeqEle->getLength();
            for (int i=0; i < len; ++i) {
                DocElement *item = consSeqEle->get(i);
                APIConstraintObject *obj = new APIConstraintObject();
                if (obj->create(filePath, consEle)) {
                    this->constraint.push_back(obj);
                } else
                    return false;
            }
        }
    }

    /** phase II: add outer info **/
    if ((commonParams != NULL) && (commonParams->size() > 0)) {
        for (vector<ParameterObject*>::iterator ite = commonParams->begin();
                ite != commonParams->end();
                ++ite) {
            const string &key = (*ite)->name;
            if (parameters.count(key) != 0) {
                Error::addError(
                        new DuplicateParameterNameError(filePath, ele->line, ele->col, "operation.parameters", key)
                );
                return false;
            } else {
                parameters[key] = *ite;
            }
        }
    }

    if (schemes == NULL) schemes = new SchemesObject();
    if (rootSchemes != NULL) {
        if (rootSchemes->http) schemes->http = true;
        if (rootSchemes->https) schemes->https = true;
    }

    /** phase III: check parameters **/
    /**
     * 1. If in='path', path should include the parameter's name
     * 2. If in='formData', there should be only certain consume types
     * 3. If in='formData', there should not exist 'body' parameters
     * 4. There should be at most one 'body' parameter
     * 5. If any parameter's of file type, its 'in' should = 'formData'
     */

    // 1
    {
        set<string> availNames;
        availNames.clear();
        int pathLen = (int)path.length();
        int nearest = -1;
        for (int i=0; i < pathLen; ++i) {
            if (path[i] == '{') nearest = i;
            if (path[i] == '}')
                if ((nearest >= 0) && (nearest < (i-1))) {
                    availNames.insert(path.substr((unsigned int) (nearest + 1), (unsigned int)(i - nearest - 1)));
                    nearest = -1;
                }
        }
        for (map<string, ParameterObject*>::iterator ite = parameters.begin();
                ite != parameters.end();
                ++ite) {
            const string &key = ite->first;
            const ParameterObject* obj = ite->second;
            if (obj->in == IN_TYPE::PATH) {
                if (availNames.count(key) == 0) {
                    Error::addError(
                        new IllegalPathParameterError(filePath, ele->line, ele->col, key)
                    );
                    return false;
                }
            }
        }
    }

    // 2
    {
        set<string> supportMemes = {"application/x-www-form-urlencoded", "multipart/form-data"};
        bool hasFormData = false;
        for (map<string, ParameterObject*>::iterator ite = parameters.begin();
                ite != parameters.end();
                ++ite) {
            const ParameterObject* obj = ite->second;
            if (obj->in == IN_TYPE::FORMDATA) {
                hasFormData = true;
                break;
            }
        }
        if (hasFormData) {
            for (vector<string>::iterator ite = consumes.begin();
                    ite != consumes.end();
                    ++ite) {
                if (supportMemes.count(*ite) == 0) {
                    Error::addError(
                        new IllegalConsumesForFormData(filePath, ele->line, ele->col)
                    );
                    return false;
                }
            }
        }
    }

    // 3
    {
        bool hasFormData = false;
        bool hasBody = false;
        for (map<string, ParameterObject*>::iterator ite = parameters.begin();
                ite != parameters.end();
                ++ite) {
            const ParameterObject* obj = ite->second;
            if (obj->in == IN_TYPE::FORMDATA)
                hasFormData = true;
            if (obj->in == IN_TYPE::BODY)
                hasBody = true;
        }
        if (hasFormData && hasBody) {
            Error::addError(
                    new FormDataAndBodyConflictError(filePath, ele->line, ele->col)
            );
            return false;
        }
    }

    // 4
    {
        int bodyParamCnt = 0;
        for (map<string, ParameterObject*>::iterator ite = parameters.begin();
                ite != parameters.end();
                ++ite) {
            const ParameterObject* obj = ite->second;
            if (obj->in == IN_TYPE::BODY) ++bodyParamCnt;
        }
        if (bodyParamCnt > 1) {
            Error::addError(
                new TooManyBodyParamError(filePath, ele->line, ele->col)
            );
            return false;
        }
    }

    // 5
    {
        bool hasFile = false;
        for (map<string, ParameterObject*>::iterator ite = parameters.begin();
                ite != parameters.end();
                ++ite) {
            const ParameterObject *obj = ite->second;
            if (obj->schema->type == DataSchemaObject::TYPE_FILE)
                if (obj->in != IN_TYPE::FORMDATA) {
                    Error::addError(
                            new IllegalFileParameterError(filePath, ele->line, ele->col, ite->first)
                    );
                    return false;
                }
        }
    }

    /** phase IV: check responses extension reference **/
    for (map<string, vector<ResponseExtensionObject*>>::iterator ite = responseExtensions.begin();
            ite != responseExtensions.end();
            ++ite) {
        const string &code = ite->first;
        if ((responses.count(code) == 0) && (responses.count("default") == 0)) {
            Error::addError(
                    new FieldMissError(filePath, ele->line, ele->col, "operation.responses.default")
            );
            return false;
        }
        ParameterObject *fatherParam = NULL;
        if (responses.count(code)) fatherParam = parameters[code]; else fatherParam = parameters["default"];
        for (vector<ResponseExtensionObject*>::iterator iite = ite->second.begin();
                iite != ite->second.end();
                ++iite) {
            ResponseExtensionObject *nowObj = *iite;
            if (fatherParam->schema->findField(nowObj->fieldVec, 0) == NULL) {
                Error::addError(
                        new FieldIllegalError(filePath, ele->line, ele->col, "operation.responses.x-extension.field")
                );
                return false;
            }
            vector<vector<string>> &relatedParams = nowObj->relatedParameters;
            for (vector<vector<string>>::iterator rite = relatedParams.begin();
                    rite != relatedParams.end();
                    ++rite) {
                vector<string> &nowField = *rite;
                if ((nowField.size() > 0) && (parameters.count(nowField[0]) > 0)
                        && (parameters[nowField[0]]->schema->findField(nowField, 1) != NULL)) {
                    // pass
                } else {
                    Error::addError(
                            new FieldIllegalError(filePath, ele->line, ele->col,
                                                  "operation.responses.x-extension.relatedParameters")
                    );
                    return false;
                }
            }
        }
    }

    /** phase V: check consumes & produces **/
    if (!APIObject::mimeCheck(consumes)) {
        Error::addError(
                new IllegalMIMEListError(filePath, ele->line, ele->col, "operation.consumes")
        );
        return false;
    }
    if (!APIObject::mimeCheck(produces)) {
        Error::addError(
                new IllegalMIMEListError(filePath, ele->line, ele->col, "operation.produces")
        );
        return false;
    }

    this->valid = true;
    return true;
}


/**
 * TODO
 * This guy's too long...
 *
 */
set<string> APIObject::mimeList = {
        "TO_BE_FILLED"
};

bool APIObject::mimeCheck(const vector<string> &mimeVec) {
    int len = (int)mimeVec.size();
//    for (int i=0; i < len; ++i) {
//        if (APIObject::mimeList.count(mimeVec[i]) == 0)
//            return false;
//    }
    return true;
}