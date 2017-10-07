//
// Created by lly on 07/10/2017.
//

#ifndef VPARSER_ILLEGALRESPONSECODEERROR_H
#define VPARSER_ILLEGALRESPONSECODEERROR_H

#include "RuntimeError.h"
#include <string>
#include <sstream>

using namespace std;

class IllegalResponseCodeError: public RuntimeError {
public:
    IllegalResponseCodeError(string apiName, long long code) {
        this->type = ILLEGAL_RESPONSE_CODE;
        this->timestamp = time(0);
        stringstream str;
        str.clear();
        str << "This API returned illegal response code: ";
        str << apiName;
        str << ". " << "The response code " << code << ".";
        str.flush();
        this->msg = str.str();
    }
};

#endif //VPARSER_ILLEGALRESPONSECODEERROR_H
