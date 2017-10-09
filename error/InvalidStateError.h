//
// Created by lly on 03/10/2017.
//

#ifndef VPARSER_INVALIDSTATEERROR_H
#define VPARSER_INVALIDSTATEERROR_H

#include "Error.h"

using namespace std;

class InvalidStateError: public Error {
public:
    InvalidStateError(std::string _fileName, int _line, int _col, std::string _fieldName, std::string _stateName) {
        this->fileName = _fileName;
        this->line = _line;
        this->col = _col;
        this->errorType = INVALID_STATE;
        this->msg = "The state '" + _stateName + "' does not exist in this scenario (Field: " + _fieldName + ").";
    }
};

#endif //VPARSER_INVALIDSTATEERROR_H
