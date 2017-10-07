//
// Created by lly on 06/10/2017.
//

#ifndef VPARSER_REQUESTTIMEOUTERROR_H
#define VPARSER_REQUESTTIMEOUTERROR_H

#include "RuntimeError.h"
#include <string>

using namespace std;

class RequestTimeoutError: public RuntimeError {
public:
    RequestTimeoutError(string apiName) {
        this->type = REQUEST_TIMEOUT;
        this->timestamp = time(0);
        this->msg = "Request timeout: " + apiName + ".";
    }
};

#endif //VPARSER_REQUESTTIMEOUTERROR_H
