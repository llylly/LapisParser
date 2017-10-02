//
// Created by lly on 02/10/2017.
//

#ifndef VPARSER_INVALIDEXPRESSIONERROR_H
#define VPARSER_INVALIDEXPRESSIONERROR_H


#include "Error.h"
#include <string>

using namespace std;

class InvalidExpressionError: public Error {
public:
    InvalidExpressionError(std::string _fileName, int _line, int _col, std::string _fieldName) {
        this->fileName = _fileName;
        this->line = _line;
        this->col = _col;
        this->errorType = INVALID_EXPRESSION;
        this->msg = "The expression in this field is invalid: " + _fieldName + '.';
    }
};

#endif //VPARSER_INVALIDEXPRESSIONERROR_H
