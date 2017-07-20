//
// Created by lly on 19/07/2017.
//

#ifndef VPARSER_ILLEGALREGEX_H
#define VPARSER_ILLEGALREGEX_H

#include "Error.h"

class IllegalRegexError: public Error {
public:
    IllegalRegexError(std::string _fileName, int _line, int _col, std::string _fieldName) {
        this->fileName = _fileName;
        this->line = _line;
        this->col = _col;
        this->errorType = ILLEGAL_REGEX;
        this->msg = "The regex expression in this field is illegal: " + _fieldName + ".";
    }
};


#endif //VPARSER_ILLEGALREGEX_H
