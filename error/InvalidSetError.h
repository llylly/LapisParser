//
// Created by lly on 03/10/2017.
//

#ifndef VPARSER_INVALIDSETERROR_H
#define VPARSER_INVALIDSETERROR_H

#include "Error.h"

using namespace std;

class InvalidSetError: public Error {
public:
    InvalidSetError(std::string _fileName, int _line, int _col, std::string _fieldName, std::string _setName) {
        this->fileName = _fileName;
        this->line = _line;
        this->col = _col;
        this->errorType = INVALID_SET;
        this->msg = "The set '" + _setName + "' does not exist in this scenario (Field: " + _fieldName + ").";
    }
};

#endif //VPARSER_INVALIDSETERROR_H
