//
// Created by lly on 06/10/2017.
//

#ifndef VPARSER_SINGLEREQUESTER_H
#define VPARSER_SINGLEREQUESTER_H


#include "../../data/BaseDataObject.h"
#include "BaseRequester.h"

class SingleRequester: public BaseRequester {
public:
    SingleRequester(
            string host,
            string basePath,
            map<string, string> *(*middleware_func)(map<string, string>*, void*) = NULL, int timeout = 2000);

    map<string, BaseDataObject*> *dataGen() override;

    bool init(APIObject *api);
};


#endif //VPARSER_SINGLEREQUESTER_H
