//
// Created by lly on 01/10/2017.
//

#ifndef VPARSER_INVALIDRESPONSETYPE_H
#define VPARSER_INVALIDRESPONSETYPE_H

#include "Error.h"
#include <string>

using namespace std;

class InvalidResponseTypeError: public Error {
public:
    InvalidResponseTypeError(std::string _fileName, int _line, int _col, std::string _fieldName, std::string _typeName) {
        this->fileName = _fileName;
        this->line = _line;
        this->col = _col;
        this->errorType = INVALID_RESPONSE_TYPE;
        this->msg = "The response type: '" + _typeName + "' does not exist in field: " + _fieldName;
    }
};

#endif //VPARSER_INVALIDRESPONSETYPE_H
