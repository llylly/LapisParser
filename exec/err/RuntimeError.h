//
// Created by lly on 06/10/2017.
//

#ifndef VPARSER_RUNTIMEERROR_H
#define VPARSER_RUNTIMEERROR_H

#include <string>
#include <vector>
#include <iostream>
#include "../../data/BaseDataObject.h"
#include "../../data/ObjectDataObject.h"
#include "../../data/IntegerDataObject.h"
#include "../../data/StringDataObject.h"


enum RUNTIME_ERROR_TYPE {
    RUNTIME_BASE, INVALID_URLPATTERN, REQUEST_TIMEOUT, ILLEGAL_RESPONSE, ILLEGAL_INPUTCONSTRAINT,
    UNKNOWN_REQUEST_ERR, ILLEGAL_RESPONSE_CODE, ILLEGAL_RESPONSE_FORMAT,
    API_NOT_FOUND, API_NOT_PARSED, REQUESTER_INIT_ERR,
    SET_EMPTY_TERMINATE
};

class RuntimeError {

public:
    RUNTIME_ERROR_TYPE type;
    long long timestamp;
    std::string msg;

    RuntimeError();

    BaseDataObject *toDataObject();

    static void addError(RuntimeError *_err);
    static bool hasError();
    static void printError(std::ostream &os);

    static std::vector<RuntimeError*> *getErrors();


private:
    static std::vector<RuntimeError*> errorPool;
};


#endif //VPARSER_RUNTIMEERROR_H
