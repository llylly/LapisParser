//
// Created by lly on 06/10/2017.
//

#ifndef VPARSER_ILLEGALRESPONSEERROR_H
#define VPARSER_ILLEGALRESPONSEERROR_H

#include "RuntimeError.h"
#include <string>

using namespace std;

class IllegalResponseError: public RuntimeError {
public:
    IllegalResponseError(string apiName) {
        this->type = ILLEGAL_RESPONSE;
        this->timestamp = time(0);
        this->msg = "This API returned illegal response: " + apiName + ".";
    }
};

#endif //VPARSER_ILLEGALRESPONSEERROR_H
