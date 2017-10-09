//
// Created by lly on 07/10/2017.
//

#ifndef VPARSER_UNKNOWNREQUESTERROR_H
#define VPARSER_UNKNOWNREQUESTERROR_H

#include "RuntimeError.h"
#include <string>

using namespace std;

class UnknownRequestError: public RuntimeError {
public:
    UnknownRequestError(int errNo, string msg) {
        this->type = UNKNOWN_REQUEST_ERR;
        this->timestamp = time(0);
        stringstream s;
        s.clear();
        s << "ErrNo: " << errNo << " msg: " << msg << ".";
        s.flush();
        this->msg = s.str();
    }
};

#endif //VPARSER_UNKNOWNREQUESTERROR_H
