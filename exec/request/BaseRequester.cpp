//
// Created by lly on 06/10/2017.
//

#include <cassert>
#include "BaseRequester.h"

static size_t writeMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    char *recvP = new char[realsize + 1];
    memset(recvP, 0, sizeof(char) * (realsize + 1));
    memcpy(recvP, contents, realsize);
    string *str = (string*)userp;
    (*str) += recvP;
    delete[] recvP;
    return realsize;
}

static size_t writeHeaderCallback(char *buffer, size_t size, size_t nitems, void *userdata) {
    size_t realsize = size * nitems;
    map<string, string>* headerMap = (map<string, string>*)userdata;

    char *recvP = new char[realsize + 1];
    memset(recvP, 0, sizeof(char) * (realsize + 1));
    memcpy(recvP, buffer, realsize);
    string s;
    s = recvP;
    delete[] recvP;

    int state = 0, l = 0, r;
    string k = "", v = "";
    for (int i=0; i<=realsize; ++i) {
        if ((buffer[i] == '\n') || (buffer[i] == '\r')) continue;
        if (i < realsize) r = i;
        if ((state == 0) && (buffer[i] == ':')) {
            state = 1;
            char *tmp = new char[r-l+1];
            memcpy(tmp, buffer + l, (size_t)r-l);
            tmp[r-l] = '\0';
            k = string(tmp);
            delete[] tmp;
        } else
        if (state == 1) {
            state = 2;
            l = i+1;
        } else
        if (i >= realsize) {
            char *tmp = new char[r-l+1];
            memcpy(tmp, buffer + l, (size_t)r-l);
            tmp[r-l] = '\0';
            v = string(tmp);
            delete[] tmp;
            if (k != "")
                (*headerMap)[k] = v;
            state = 0;
        }
    }

    return realsize;
}

static string headerToStr(map<string, string> *map) {
    string s = "";
    for (auto ite = map->begin();
            ite != map->end();
            ++ite) {
        s += ite->first + ": " + ite->second + "\n";
    }
    return s;
}

/* --------------- */

BaseRequester::BaseRequester(
        string host,
        string basePath,
        RequesterReport *report,
        map<string, string> *(*middleware_func)(map<string, string>*, void*),
        int timeout) {
    this->report = report;
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

void BaseRequester::work() {
    if (this->err != NULL) {
        delete this->err;
    }

    if (this->report)
        this->report->startTime = time(0);

    /***----stage 1--------***/
    // this guy saved to report
    map<string, BaseDataObject*> *dataParam = this->dataGen();

    if (this->err != NULL) {
        if (this->report)
            this->report->err = this->err, this->report->endTime = time(0);
        if (dataParam != NULL) {
            for (map<string, BaseDataObject*>::iterator ite = dataParam->begin();
                    ite != dataParam->end();
                    ++ite) {
                delete ite->second;
            }
            delete dataParam;
        }
        return;
    }

    if (this->report) {
        for (map<string, BaseDataObject*>::iterator ite = dataParam->begin();
             ite != dataParam->end();
             ++ite)
            this->report->request[ite->first] = ite->second;
    }
    /***----stage 1 end----***/

    /***----stage 2--------***/
    // tmp guy
    map<string, string> *strParam = this->serialize(dataParam);

    if (this->err != NULL) {
        if (this->report)
            this->report->err = this->err, this->report->endTime = time(0);
        if (strParam != NULL) {
            delete strParam;
        }
        return;
    }
    /***----stage 2 end----***/

    /***----stage 3--------***/
    // tmp guy
    map<string, string> *middledStrParam = this->middleware(strParam);

    if (this->err != NULL) {
        if (this->report)
            this->report->err = this->err, this->report->endTime = time(0);
        if (middledStrParam != NULL) {
            delete middledStrParam;
        }
        return;
    }
    /***----stage 3 end----***/

    /***----stage 4--------***/
    // tmp guy
    pair<long long, pair<map<string, string>*, string>> *rawResponse = this->emit(middledStrParam);

    // recycle temp guy
    if (strParam != middledStrParam) {
        delete strParam;
        delete middledStrParam;
    } else
        delete strParam;

    if (this->err != NULL) {
        if (this->report)
            this->report->err = this->err, this->report->endTime = time(0);
        if (rawResponse != NULL) {
            delete rawResponse;
        }
        return;
    }
    /***----stage 4 end----***/

    /***----stage 5--------***/
    // tmp guy
    pair<long long, DocObjectElement*> *docResponse = this->responsePartition(rawResponse);

    // recycle temp guy
    delete rawResponse->second.first;

    if (this->err != NULL) {
        if (this->report)
            this->report->err = this->err, this->report->endTime = time(0);
        if (docResponse != NULL) {
            delete docResponse->second;
            delete docResponse;
        }
        return;
    }
    /***----stage 5 end----***/

    /***----stage 6--------***/
    pair<string, BaseDataObject*> *objResponse = this->responseParse(docResponse);

    // recycle tmp guy
    if (docResponse->second)
        delete docResponse->second;
    delete docResponse;

    if (this->err != NULL) {
        if (this->report)
            this->report->err = this->err, this->report->endTime = time(0);
        if (objResponse != NULL) {
            delete objResponse->second;
            delete objResponse;
        }
        return;
    }
    /***----stage 6 end----***/
    if (this->report) {
        this->report->responseType = objResponse->first;
        this->report->response = objResponse->second;
        this->report->endTime = time(0);
    }
    return;
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
    else {
        return this->middleware_func(strParam, this->userP);
    }
}

pair<long long, pair<map<string, string>*, string>> *BaseRequester::emit(
        map<string, string> *strParam) {

    CURL *curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_ALL);

    curl = curl_easy_init();
    if (curl) {
        long long responseCode = 0;
        string responseStr = "";
        map<string, string> *headers = new map<string, string>();

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
        if ((api->requestMethod == APIRequestMethod::POST) ||
                (api->requestMethod == APIRequestMethod::GET) ||
                (api->requestMethod == APIRequestMethod::DELETE)) {
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
            if (api->requestMethod == APIRequestMethod::POST)
                curl_easy_setopt(curl, CURLOPT_POST, 1L);
            else if (api->requestMethod == APIRequestMethod::PUT)
                curl_easy_setopt(curl, CURLOPT_PUT, 1L);
            else if (api->requestMethod == APIRequestMethod::DELETE) {
                curl_easy_setopt(curl, CURLOPT_POST, 1L);
                curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
            }
            else if (api->requestMethod == APIRequestMethod::HEAD) {
                curl_easy_setopt(curl, CURLOPT_POST, 1L);
                curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
            }
            else if (api->requestMethod == APIRequestMethod::OPTIONS) {
                curl_easy_setopt(curl, CURLOPT_POST, 1L);
                curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "OPTIONS");
            }
            else if (api->requestMethod == APIRequestMethod::PATCH) {
                curl_easy_setopt(curl, CURLOPT_POST, 1L);
                curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PATCH");
            }
            else if (api->requestMethod == APIRequestMethod::TRACE) {
                curl_easy_setopt(curl, CURLOPT_POST, 1L);
                curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "TRACE");
            }
        } else
            curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);

        curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, (unsigned)this->timeout);

        curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");

        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)(&responseStr));

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeMemoryCallback);

        curl_easy_setopt(curl, CURLOPT_HEADERDATA, (void*)(headers));

        curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, writeHeaderCallback);

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        res = curl_easy_perform(curl);

        if (this->report)
            this->report->url = url;

        /** Logger head **/
        Logger::addLog(new StrLog("URL: " + url));
        for (map<string, string>::iterator ite = strParam->begin();
                ite != strParam->end();
                ++ite) {
            Logger::addLog(new StrLog("Request Param \"" + ite->first + "\": " + ite->second));
        }
        /** Logger tail **/

        if ((res == CURLE_OK) && (responseStr.length() > 0)) {
            if (this->report) {
                double total;
                curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &total);
                this->report->requestMilliTime = (int)(total * 1000. + 0.5);
            }
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &responseCode);
            /** Logger head **/
            Logger::addLog(new StrLog("Code: " + to_string(responseCode)));
            Logger::addLog(new StrLog("Headers: " + headerToStr(headers)));
            Logger::addLog(new StrLog("Response: " + responseStr));
            Logger::addLog(new StrLog("------splitter------"));
            /** Logger tail **/
        } else {
            stringstream err_s;
            err_s.clear();
            err_s << "request failed: " << curl_easy_strerror(res);
            err_s.flush();
            this->err = new UnknownRequestError((int)res, err_s.str());
            return NULL;
        }

        curl_easy_cleanup(curl);
        curl_formfree(post);
        curl_slist_free_all(headerlist);
        curl_global_cleanup();

        return new pair<long long, pair<map<string, string>*, string>>
                (responseCode, pair<map<string, string>*, string>(headers, responseStr));
    } else {
        curl_global_cleanup();
        this->err = new UnknownRequestError(-1, "requester init error");
        return NULL;
    }
}

pair<long long, DocObjectElement*> *BaseRequester::responsePartition(
        pair<long long, pair<map<string, string>*, string>> *response) {
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

    ResponseObject *responseObj = NULL;
    if (api->responses.count(strResponseCode) > 0) {
        responseObj = api->responses[strResponseCode];
    } else {
        if (api->responses.count("default") > 0)
            responseObj = api->responses["default"];
        else {
            this->err = new IllegalResponseCodeError(api->name, response->first);
            return NULL;
        }
    }

    DocObjectElement *rootEle = new DocObjectElement(1, 1);
    DocElement *bodyEle = NULL;
    if (responseObj->schema == NULL)
        bodyEle = new DocScalarElement(1, 1, "");
    else {
        bodyEle = ExecTransformer::deserialize(response->second.second, responseObj->schema);
        if (bodyEle == NULL) {
            this->err = new IllegalResponseCodeError(api->name, response->first);
            return NULL;
        }
    }
    rootEle->add("body", bodyEle);

    DocObjectElement *headersEle = new DocObjectElement(1, 1);
    map<string, ParameterObject*> &headersMap = responseObj->headers;
    for (map<string, ParameterObject*>::iterator ite = headersMap.begin();
            ite != headersMap.end();
            ++ite) {
        if (response->second.first->count(ite->first) > 0) {
            DocElement *nowHeader = ExecTransformer::deserialize((*(response->second.first))[ite->first], ite->second->schema);
            if (nowHeader == NULL) {
                this->err = new IllegalResponseCodeError(api->name, response->first);
                return NULL;
            } else {
                headersEle->add(ite->first, nowHeader);
            }
        } else {
            if (ite->second->required) {
                this->err = new IllegalResponseCodeError(api->name, response->first);
                return NULL;
            }
        }
    }
    rootEle->add("headers", headersEle);

    return new pair<long long, DocObjectElement*>(response->first, rootEle);

    /*
    DataSchemaObject *schema = NULL;
    if (api->responses.count(strResponseCode) > 0) {
        schema = api->responses[strResponseCode]->schema;
        if (schema == NULL) {
            // means we don't care the response
            return new pair<long long, DocElement*>(response->first, new DocScalarElement(1, 1, ""));
        }
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
     */
}

/** TODO **/
pair<string, BaseDataObject*> *BaseRequester::responseParse(
        pair<long long, DocObjectElement*> *eleResponse) {
    if (eleResponse == NULL) {
        this->err = new IllegalResponseError(api->name);
        return NULL;
    }

    long long code = eleResponse->first;
    DocObjectElement *docEle = eleResponse->second;

    stringstream sstream;
    sstream.clear();
    sstream << code;
    sstream.flush();
    string strCode = sstream.str();
    sstream.clear();

    ResponseObject *responseObj = NULL;
    if (api->responses.count(strCode) > 0) {
        responseObj = api->responses[strCode];
    } else {
        if (api->responses.count("default") > 0)
            responseObj = api->responses["default"];
        else {
            this->err = new IllegalResponseCodeError(api->name, eleResponse->first);
            return NULL;
        }
    }

    DocElement *bodyEle = docEle->get("body");
    DocObjectElement *headersEle = (DocObjectElement*)(docEle->get("headers"));
    assert(bodyEle != NULL);
    assert(headersEle != NULL);

    BaseDataObject *bodyObj = NULL;
    if (responseObj->schema == NULL) {
        bodyObj = new StringDataObject("");
    } else {
        bodyObj = responseObj->schema->transform(bodyEle);
        if (bodyObj == NULL) {
            this->err = new IllegalResponseFormatError(api->name, code);
            return NULL;
        }
    }

    ObjectDataObject *headerObj = new ObjectDataObject();
    map<string, ParameterObject*> &headerMap = responseObj->headers;
    for (map<string, ParameterObject*>::iterator ite = headerMap.begin();
            ite != headerMap.end();
            ++ite) {
        DocElement *nowEle = headersEle->get(ite->first);
        if (nowEle != NULL) {
            BaseDataObject *nowDataObj = ite->second->schema->transform(nowEle);
            if (nowDataObj == NULL) {
                this->err = new IllegalResponseFormatError(api->name, code);
                return NULL;
            } else
                (*headerObj)[ite->first] = nowDataObj;
        }
    }

    ObjectDataObject *obj = new ObjectDataObject();
    (*obj)["body"] = bodyObj;
    (*obj)["headers"] = headerObj;


    /*
    DataSchemaObject *schema = NULL;
    if (api->responses.count(strCode) > 0) {
        schema = api->responses[strCode]->schema;
        if (schema == NULL) {
            // means we don't care the response
            return new pair<string, BaseDataObject*>(strCode, new StringDataObject(""));
        }
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
    */

    // save the schema definition
    if (this->report)
        this->report->schema = responseObj->schema;
    string responseType = strCode;

    if (api->responseExtensions.count(strCode) > 0) {
        vector<ResponseExtensionObject*> &candidateVec = api->responseExtensions[strCode];
        for (vector<ResponseExtensionObject*>::iterator ite = candidateVec.begin();
                ite != candidateVec.end();
                ++ite) {
            ResponseExtensionObject *nowExt = *ite;
            BaseDataObject *nowField = obj;
            vector<string> &fieldVec = nowExt->fieldVec;
            if ((fieldVec.size() > 0) && (fieldVec[0] != "headers"))
                nowField = (*obj)["body"];
            bool needDel = false;
            for (vector<string>::iterator iite = fieldVec.begin();
                    iite != fieldVec.end();
                    ++iite) {
                // add "size" support
                if (nowField->type == SEQUENCE) {
                    SequenceDataObject *seq = (SequenceDataObject*)nowField;
                    if ((*iite) == "size")
                        needDel = true, nowField = new IntegerDataObject(seq->length());
                    else {
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
                }
                else if (nowField->type == OBJECT) {
                    ObjectDataObject *objField = (ObjectDataObject*)nowField;
                    if (objField->hasKey(*iite)) {
                        nowField = (*objField)[*iite];
                    } else {
                        if ((*iite) == "size")
                            needDel = true, nowField = new IntegerDataObject(objField->size());
                        else {
                            nowField = NULL;
                            break;
                        }
                    }
                }
                else {
                    if (needDel) {
                        delete nowField;
                        needDel = false;
                    }
                    nowField = NULL;
                    break;
                }
            }
            if (nowField == NULL) continue;
            if (nowExt->schema->check(nowField)) {
                responseType = nowExt->name;
                if (needDel) delete nowField;
                break;
            }
            if (needDel) delete nowField;
        }
    }

    return new pair<string, BaseDataObject*>(responseType, obj);
}
