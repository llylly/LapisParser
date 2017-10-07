//
// Created by lly on 06/10/2017.
//

#include "BaseRequester.h"

static size_t writeMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    char **recvP = (char**)userp;
    *recvP = new char[realsize + 1];
    memset(*recvP, 0, sizeof(char) * (realsize + 1));
    memcpy(*recvP, contents, realsize);
    return realsize;
}

/* --------------- */

BaseRequester::BaseRequester(
        string host,
        string basePath,
        map<string, string> *(*middleware_func)(map<string, string>*, void*),
        int timeout) {
    this->err = NULL;
    this->host = host;
    this->basePath = basePath;
    this->middleware_func = middleware_func;
    this->timeout = timeout;
    this->userP = NULL;
}

void BaseRequester::setUserP(void *p) {
    this->userP = p;
}

pair<string, BaseDataObject*> *BaseRequester::work() {
    if (this->err != NULL) {
        delete this->err;
    }
    map<string, BaseDataObject*> *dataParam = this->dataGen();

    cout << "***----stage 1--------" << endl;
    for (map<string, BaseDataObject*>::iterator ite = dataParam->begin();
            ite != dataParam->end();
            ++ite) {
        cout << "key: " << ite->first << endl;
        DataObjectAdapter::toDocElement(ite->second)->printTo(cout);
    }
    cout << "***----stage 1 end----" << endl;

    if (this->err != NULL) {
        if (dataParam != NULL) {
            for (map<string, BaseDataObject*>::iterator ite = dataParam->begin();
                    ite != dataParam->end();
                    ++ite) {
                delete ite->second;
            }
            delete dataParam;
        }
        return NULL;
    }
    map<string, string> *strParam = this->serialize(dataParam);
    if (this->err != NULL) {
        if (strParam != NULL) {
            delete strParam;
        }
        return NULL;
    }

    cout << "***----stage 2--------" << endl;
    for (map<string, string>::iterator ite = strParam->begin();
         ite != strParam->end();
         ++ite) {
        cout << "key  : " << ite->first << endl;
        cout << "value: " << ite->second << endl;
    }
    cout << "***----stage 2 end----" << endl;

    map<string, string> *middledStrParam = this->middleware(strParam);
    if (this->err != NULL) {
        if (middledStrParam != NULL) {
            delete middledStrParam;
        }
        return NULL;
    }
    pair<long long, string> *rawResponse = this->emit(middledStrParam);
    if (this->err != NULL) {
        if (rawResponse != NULL) {
            delete rawResponse;
        }
        return NULL;
    }

    cout << "***----stage 3--------" << endl;
    cout << "response Code  : " << rawResponse->first << endl;
    cout << "response string: " << rawResponse->second << endl;
    cout << "***----stage 3 end----" << endl;

    pair<long long, DocElement*> *docResponse = this->responsePartition(rawResponse);
    if (this->err != NULL) {
        if (docResponse != NULL) {
            delete docResponse->second;
            delete docResponse;
        }
        return NULL;
    }

    cout << "***----stage 4--------" << endl;
    cout << "response Code    : " << docResponse->first << endl;
    cout << "response Response: " << endl;
    docResponse->second->printTo(cout);
    cout << "***----stage 4 end----" << endl;

    pair<string, BaseDataObject*> *objResponse = this->responseParse(docResponse);
    if (this->err != NULL) {
        if (objResponse != NULL) {
            delete objResponse->second;
            delete objResponse;
        }
        return NULL;
    }
    return objResponse;
}

map<string, string> *BaseRequester::serialize(
        map<string, BaseDataObject *> *dataParam) {
    map<string, string> *ans = new map<string, string>();
    for (map<string, BaseDataObject *>::iterator ite = dataParam->begin();
            ite != dataParam->end();
            ++ite) {
        string s = ExecTransformer::serailize(ite->second, this->api->parameters[ite->first]->schema, ite->first);
        (*ans)[ite->first] = s;
    }
    return ans;
}

map<string, string> *BaseRequester::middleware(
        map<string, string> *strParam) {
    if (this->middleware_func == NULL)
        return strParam;
    else
        return this->middleware_func(strParam, this->userP);
}

pair<long long, string> *BaseRequester::emit(
        map<string, string> *strParam) {

    CURL *curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_ALL);

    curl = curl_easy_init();
    if (curl) {
        long long responseCode = 0;
        string responseStr = "";
        char *recvBuf = NULL;

        string url;

        curl_slist *headerlist = NULL;
        curl_httppost* post = NULL;
        curl_httppost* last = NULL;

        {
            /** figure out true URL **/
            string protocol = "";
            vector<string> support_protocol;
            if (api->schemes->http) support_protocol.push_back("http");
            if (api->schemes->https) support_protocol.push_back("https");
            protocol = support_protocol[rand() % support_protocol.size()];

            url = protocol + "://" + host + basePath + "/" + this->api->path;

            if (protocol == "https") {
                /** HTTPS needs these settings **/
                curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
                curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, true);
            }

            /** substitute "path" parameters **/
            for (map<string, ParameterObject*>::iterator ite = this->api->parameters.begin();
                    ite != this->api->parameters.end();
                    ++ite) {
                ParameterObject *nowParam = ite->second;
                if (nowParam->in == IN_TYPE::PATH) {
                    string pattern = "{" + nowParam->name + "}";
                    int pos = (int)url.find(pattern);
                    if (pos != string::npos) {
                        string newstr = url.substr(0, (unsigned)pos);
                        if (strParam->count(nowParam->name) > 0)
                            newstr += (*strParam)[nowParam->name];
                        newstr += url.substr(pos + pattern.length());
                        url = newstr;
                    }
                }
            }
        }

        {
            /** add "query" parameters to requesting URL **/
            string query_suf;
            bool has_query = false;
            for (map<string, ParameterObject*>::iterator ite = this->api->parameters.begin();
                    ite != this->api->parameters.end();
                    ++ite) {
                ParameterObject *nowParam = ite->second;
                if (nowParam->in == IN_TYPE::QUERY)
                    if (strParam->count(nowParam->name) > 0) {
                        if (!has_query) {
                            query_suf = "?";
                            has_query = true;
                        } else {
                            query_suf += "&";
                        }
                        if ((nowParam->schema->type == DataSchemaObject::TYPE_ARRAY)
                            && (((ArraySchema*)nowParam->schema)->collectionFormat == MULTI))
                            query_suf += (*strParam)[nowParam->name];
                        else
                            query_suf += nowParam->name + "=" + (*strParam)[nowParam->name];
                    }
            }
            if (!has_query) query_suf = "";
            url += query_suf;
        }

        /** random choose a content type **/
        string contentType = "";
        if (api->consumes.size() > 0)
            contentType = api->consumes[rand() % api->consumes.size()];
        string strContentType = "Content-Type: ";
        strContentType += contentType;
        headerlist = curl_slist_append(headerlist, strContentType.c_str());

        {
            /** add "header" parameters **/
            for (map<string, ParameterObject*>::iterator ite = this->api->parameters.begin();
                    ite != this->api->parameters.end();
                    ++ite) {
                ParameterObject *nowParam = ite->second;
                if (nowParam->in == IN_TYPE::HEADER)
                    if (strParam->count(nowParam->name) > 0) {
                        string str = nowParam->name + ": " + (*strParam)[nowParam->name];
                        headerlist = curl_slist_append(headerlist, str.c_str());
                    }
            }
        }
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);

        bool hasBodyParam = false;
        bool hasFormParams = false;
        if (api->requestMethod == APIRequestMethod::POST) {
            for (map<string, ParameterObject*>::iterator ite = this->api->parameters.begin();
                    ite != this->api->parameters.end();
                    ++ite) {
                ParameterObject *nowParam = ite->second;
                if ((nowParam->in == IN_TYPE::BODY) && (strParam->count(nowParam->name) > 0))
                    hasBodyParam = true;
                if ((nowParam->in == IN_TYPE::FORMDATA) && (strParam->count(nowParam->name) > 0))
                    hasFormParams = true;
            }
            if (hasFormParams) {
                /** add "formdata" parameters **/
                for (map<string, ParameterObject*>::iterator ite = this->api->parameters.begin();
                        ite != this->api->parameters.end();
                        ++ite) {
                    ParameterObject *nowParam = ite->second;
                    if ((nowParam->in == IN_TYPE::FORMDATA) && (strParam->count(nowParam->name) > 0)) {
                        curl_formadd(&post, &last, CURLFORM_COPYNAME, nowParam->name.c_str(),
                                     CURLFORM_COPYCONTENTS, (*strParam)[nowParam->name].c_str(), CURLFORM_END);
                    }
                }
                curl_easy_setopt(curl, CURLOPT_HTTPPOST, post);
            }
            if (hasBodyParam) {
                /** add "body" parameters **/
                for (map<string, ParameterObject*>::iterator ite = this->api->parameters.begin();
                     ite != this->api->parameters.end();
                     ++ite) {
                    ParameterObject *nowParam = ite->second;
                    if ((nowParam->in == IN_TYPE::BODY) && (strParam->count(nowParam->name) > 0)) {
                        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, (*strParam)[nowParam->name].c_str());
                    }
                }
            }
            curl_easy_setopt(curl, CURLOPT_POST, 1L);
        } else
            curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);

        curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, (unsigned)this->timeout);

        curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");

        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)(&recvBuf));

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeMemoryCallback);

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        res = curl_easy_perform(curl);

        cout << "URL: " << url << endl;

        if (res == CURLE_OK) {
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &responseCode);
            responseStr = string(recvBuf);
        } else {
            stringstream err_s;
            err_s.clear();
            err_s << "request failed: " << curl_easy_strerror(res);
            err_s.flush();
            this->err = new UnknownRequestError((int)res, err_s.str());
            return NULL;
        }

        if (recvBuf != NULL) free(recvBuf);
        curl_easy_cleanup(curl);
        curl_formfree(post);
        curl_slist_free_all(headerlist);
        curl_global_cleanup();

        return new pair<long long, string>(responseCode, responseStr);
    } else {
        curl_global_cleanup();
        this->err = new UnknownRequestError(-1, "requester init error");
        return NULL;
    }
}

pair<long long, DocElement*> *BaseRequester::responsePartition(
        pair<long long, string> *response) {
    if (response == NULL) {
        this->err = new IllegalResponseError(api->name);
        return NULL;
    }
    stringstream sstream;
    sstream.clear();
    sstream << response->first;
    sstream.flush();
    string strResponseCode = sstream.str();
    sstream.clear();

    DataSchemaObject *schema = NULL;
    if (api->responses.count(strResponseCode) > 0) {
        schema = api->responses[strResponseCode]->schema;
    } else {
        if (api->responses.count("default") > 0)
            schema = api->responses["default"]->schema;
    }
    if (schema == NULL) {
        this->err = new IllegalResponseCodeError(api->name, response->first);
        return NULL;
    }
    DocElement *ele = ExecTransformer::deserialize(response->second, schema);
    if (ele == NULL) {
        this->err = new IllegalResponseCodeError(api->name, response->first);
        return NULL;
    }
    return new pair<long long, DocElement*>(response->first, ele);
}

pair<string, BaseDataObject*> *BaseRequester::responseParse(
        pair<long long, DocElement*> *eleResponse) {
    if (eleResponse == NULL) {
        this->err = new IllegalResponseError(api->name);
        return NULL;
    }

    long long code = eleResponse->first;
    DocElement *docEle = eleResponse->second;

    stringstream sstream;
    sstream.clear();
    sstream << code;
    sstream.flush();
    string strCode = sstream.str();
    sstream.clear();

    DataSchemaObject *schema = NULL;
    if (api->responses.count(strCode) > 0) {
        schema = api->responses[strCode]->schema;
    } else {
        if (api->responses.count("default") > 0)
            schema = api->responses["default"]->schema;
    }
    if (schema == NULL) {
        this->err = new IllegalResponseCodeError(api->name, code);
        return NULL;
    }
    BaseDataObject *obj = schema->transform(docEle);
    if (obj == NULL) {
        this->err = new IllegalResponseFormatError(api->name, code);
        return NULL;
    }
    string responseType = strCode;

    if (api->responseExtensions.count(strCode) > 0) {
        vector<ResponseExtensionObject*> &candidateVec = api->responseExtensions[strCode];
        for (vector<ResponseExtensionObject*>::iterator ite = candidateVec.begin();
                ite != candidateVec.end();
                ++ite) {
            ResponseExtensionObject *nowExt = *ite;
            BaseDataObject *nowField = obj;
            vector<string> &fieldVec = nowExt->fieldVec;
            for (vector<string>::iterator iite = fieldVec.begin();
                    iite != fieldVec.end();
                    ++iite) {
                if (nowField->type == SEQUENCE) {
                    SequenceDataObject *seq = (SequenceDataObject*)nowField;
                    if (seq->length() == 0) {
                        nowField = NULL;
                        break;
                    } else {
                        int ind = 0;
                        try {
                            ind = stoi(*iite);
                        } catch (std::invalid_argument) {
                            nowField = NULL;
                            break;
                        } catch (std::out_of_range) {
                            nowField = NULL;
                            break;
                        }
                        if (ind >= 0)
                            nowField = (*seq)[ind % seq->length()];
                        else {
                            ind = abs(ind) % seq->length();
                            if (ind == 0) ind = seq->length();
                            nowField = (*seq)[seq->length() - ind];
                        }
                    }
                }
                else if (nowField->type == OBJECT) {
                    ObjectDataObject *objField = (ObjectDataObject*)nowField;
                    if (objField->hasKey(*iite)) {
                        nowField = (*objField)[*iite];
                    } else {
                        nowField = NULL;
                        break;
                    }
                }
                else {
                    nowField = NULL;
                    break;
                }
            }
            if (nowField == NULL) continue;
            if (nowExt->schema->check(nowField)) {
                responseType = nowExt->name;
                break;
            }
        }
    }

    return new pair<string, BaseDataObject*>(responseType, obj);
}