//
// Created by lly on 08/10/2017.
//

#ifndef VPARSER_ILLEGALRESPONSEFORMATERROR_H
#define VPARSER_ILLEGALRESPONSEFORMATERROR_H

#include "RuntimeError.h"
#include <string>
#include <sstream>

using namespace std;

class IllegalResponseFormatError: public RuntimeError {
public:
    IllegalResponseFormatError(string apiName, long long responseCode) {
        this->type = ILLEGAL_RESPONSE_FORMAT;
        this->timestamp = time(0);
        stringstream str;
        str.clear();
        str << "The response doesn't conform to the schema. ";
        str << "API name: " << apiName << ". ";
        str << "Response code: " << responseCode << ".";
        str.flush();
        this->msg = str.str();
    }
};

#endif //VPARSER_ILLEGALRESPONSEFORMATERROR_H
