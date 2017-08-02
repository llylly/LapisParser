//
// Created by lly on 01/08/2017.
//

#ifndef VPARSER_NOTPARSEDERROR_H
#define VPARSER_NOTPARSEDERROR_H

#include "Error.h"

class NotParsedError: public Error {
public:
    NotParsedError() {
        this->fileName = "";
        this->line = 1;
        this->col = 1;
        this->errorType = NOT_PARSED;
        this->msg = "The Doc tree has not parsed to internal representation. The request is invalid.";
    }
};

#endif //VPARSER_NOTPARSEDERROR_H
