//
// Created by lly on 29/05/2017.
//

#ifndef VPARSER_INVALIDSCHEMEERROR_H
#define VPARSER_INVALIDSCHEMEERROR_H

#include "Error.h"

class InvalidSchemeError: public Error {
public:
    InvalidSchemeError(std::string _fileName, int _line, int _col) {
        this->fileName = _fileName;
        this->line = _line;
        this->col = _col;
        this->errorType = INVALID_SCHEME_ERROR;
        this->msg = "Unsupported or unrecognizable scheme name.";
    }
};


#endif //VPARSER_INVALIDSCHEMEERROR_H
