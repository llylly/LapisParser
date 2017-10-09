//
// Created by lly on 06/10/2017.
//

#ifndef VPARSER_BASEREQUESTER_H
#define VPARSER_BASEREQUESTER_H

#include <string>
#include <map>
#include "../../data/BaseDataObject.h"
#include "../../schema/ParameterObject.h"
#include "../../schema/APIObject.h"
#include "../err/RuntimeError.h"
#include "../err/UnknownRequestError.h"
#include "../err/IllegalResponseError.h"
#include "../err/IllegalResponseCodeError.h"
#include "../err/IllegalResponseFormatError.h"
#include "../transformer/ExecTransformer.h"
#include "../report/RequesterReport.h"
#include "../../lib/libcurl/curl/curl.h"

using namespace std;

class BaseRequester {
protected:
    virtual map<string, BaseDataObject*> *dataGen() = 0;

    map<string, string> *serialize(map<string, BaseDataObject*> *dataParam);

    map<string, string> *middleware(map<string, string> *strParam);

    pair<long long, string> *emit(map<string, string> *strParam);

    pair<long long, DocElement*> *responsePartition(pair<long long, string> *response);

    pair<string, BaseDataObject*> *responseParse(pair<long long, DocElement*> *eleResponse);

    APIObject *api;

public:
    BaseRequester(
            string host,
            string basePath,
            RequesterReport *report,
            map<string, string> *(*middleware_func)(map<string, string>*, void*) = NULL,
            int timeout = 2000);

    void setUserP(void *p);

    void work();

    RequesterReport *report;

    RuntimeError *err;

    string host;
    string basePath;
    map<string, string> *(*middleware_func)(map<string, string>*, void*);
    int timeout;
    void *userP;
};


#endif //VPARSER_BASEREQUESTER_H
