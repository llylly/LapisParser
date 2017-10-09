//
// Created by lly on 04/10/2017.
//

#ifndef VPARSER_NOTCONFIGPARSEDERROR_H
#define VPARSER_NOTCONFIGPARSEDERROR_H

#include "Error.h"

class NotConfigParsedError: public Error {
public:
    NotConfigParsedError() {
        this->fileName = "";
        this->line = 1;
        this->col = 1;
        this->errorType = NOT_CONFIG_PARSED;
        this->msg = "The config has not been parsed to internal representation. The request is invalid.";
    }
};

#endif //VPARSER_NOTCONFIGPARSEDERROR_H
