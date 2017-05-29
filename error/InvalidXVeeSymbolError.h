//
// Created by lly on 29/05/2017.
//

#ifndef VPARSER_INVALIDXVEESYMBOLERROR_H
#define VPARSER_INVALIDXVEESYMBOLERROR_H


#include "Error.h"

class InvalidXVeeSymbolError: public Error {
public:
    InvalidXVeeSymbolError(std::string _fileName, int _line, int _col) {
        this->fileName = _fileName;
        this->line = _line;
        this->col = _col;
        this->errorType = INVALID_X_VEE_SYMBOL_ERROR;
        this->msg = "Illegal \"x-vee\" field(not equals to \"2.0 AS\").";
    }
};


#endif //VPARSER_INVALIDXVEESYMBOLERROR_H
