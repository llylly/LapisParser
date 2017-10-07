//
// Created by lly on 06/10/2017.
//

#ifndef VPARSER_INVALIDURLPATTERNERROR_H
#define VPARSER_INVALIDURLPATTERNERROR_H

#include "RuntimeError.h"
#include <string>

using namespace std;

class InvalidURLPatternError: public RuntimeError {
public:
    InvalidURLPatternError(string urlPattern) {
        this->type = INVALID_URLPATTERN;
        this->timestamp = time(0);
        this->msg = "Illegal URL pattern: " + urlPattern + ".";
    }
};


#endif //VPARSER_INVALIDURLPATTERNERROR_H
