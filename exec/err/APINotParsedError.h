//
// Created by lly on 08/10/2017.
//

#ifndef VPARSER_APINOTPARSEDERROR_H
#define VPARSER_APINOTPARSEDERROR_H

#include "RuntimeError.h"
#include <string>

using namespace std;

class APINotParsedError: public RuntimeError {
public:
    APINotParsedError() {
        this->type = API_NOT_PARSED;
        this->timestamp = time(0);
        this->msg = "The API definitions hasn't been parsed.";
    }
};

#endif //VPARSER_APINOTPARSEDERROR_H
