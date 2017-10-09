//
// Created by lly on 08/10/2017.
//

#ifndef VPARSER_APINOTFOUNDERROR_H
#define VPARSER_APINOTFOUNDERROR_H

#include "RuntimeError.h"
#include <string>

using namespace std;

class APINotFoundError: public RuntimeError {
public:
    APINotFoundError() {
        this->type = API_NOT_FOUND;
        this->timestamp = time(0);
        this->msg = "Required API not found in definition.";
    }
};


#endif //VPARSER_APINOTFOUNDERROR_H
