//
// Created by lly on 29/05/2017.
//

#ifndef VPARSER_INVALIDSWAGGERSYMBOLERROR_H
#define VPARSER_INVALIDSWAGGERSYMBOLERROR_H

#include "Error.h"

class InvalidSwaggerSymbolError: public Error {
public:

    InvalidSwaggerSymbolError(std::string _fileName, int _line, int _col) {
        this->fileName = _fileName;
        this->line = _line;
        this->col = _col;
        this->errorType = INVALID_SWAGGER_SYMBOL_ERROR;
        this->msg = "Illegal \"swagger\" field(not equals to \"2.0\").";
    }
};


#endif //VPARSER_INVALIDSWAGGERSYMBOLERROR_H
