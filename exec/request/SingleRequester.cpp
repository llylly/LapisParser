//
// Created by lly on 06/10/2017.
//

#include "SingleRequester.h"

SingleRequester::SingleRequester(string host,
                                 string basePath,
                                 RequesterReport *report,
                                 map<string, string> *(*middleware_func)(map<string, string>*, void*),
                                 int timeout):
        BaseRequester(host, basePath, report, middleware_func, timeout) { }

map<string, BaseDataObject*> *SingleRequester::dataGen() {
    map<string, BaseDataObject*> *ans = new map<string, BaseDataObject*>();
    for (map<string, ParameterObject*>::iterator ite = api->parameters.begin();
            ite != api->parameters.end();
            ++ite) {
        const string &paramName = ite->first;
        ParameterObject *nowParam = ite->second;
        double ran_v = DataSchemaObject::randomReal();
        if (ran_v <= nowParam->nullProbability)
            continue;
        BaseDataObject *nowData = nowParam->schema->generate();
        (*ans)[paramName] = nowData;
    }
    return ans;
}

bool SingleRequester::init(APIObject *api) {
    if (api == NULL)
        return false;
    this->api = api;
    return true;
}