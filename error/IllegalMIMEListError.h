//
// Created by lly on 29/07/2017.
//

#ifndef VPARSER_ILLEGALMIMELISTERROR_H
#define VPARSER_ILLEGALMIMELISTERROR_H


#include <string>
#include "Error.h"

class IllegalMIMEListError: public Error {
public:
    IllegalMIMEListError(std::string _fileName, int _line, int _col, std::string _fieldName) {
        this->fileName = _fileName;
        this->line = _line;
        this->col = _col;
        this->errorType = ILLEGAL_MIME_LIST;
        this->msg = "The MIME list in this field \"" + _fieldName + "\" contains illegal MIME type(s).";
    }
};


#endif //VPARSER_ILLEGALMIMELISTERROR_H
