//
// Created by lly on 01/10/2017.
//

#ifndef VPARSER_INVALIDAPIREFERENCE_H
#define VPARSER_INVALIDAPIREFERENCE_H

#include "Error.h"
#include <string>

class InvalidAPIReferenceError: public Error {
public:
    InvalidAPIReferenceError(std::string _fileName, int _line, int _col, std::string _fieldName) {
        this->fileName = _fileName;
        this->line = _line;
        this->col = _col;
        this->errorType = INVALID_API_REFERENCE;
        this->msg = "API Reference here is invalid: " + _fieldName + ".";
    }
};

#endif //VPARSER_INVALIDAPIREFERENCE_H
