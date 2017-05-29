//
// Created by lly on 29/05/2017.
//

#ifndef VPARSER_ROOTFIELDDUPLICATEERROR_H
#define VPARSER_ROOTFIELDDUPLICATEERROR_H


#include "Error.h"

class RootFieldDuplicateError: public Error {
public:
    RootFieldDuplicateError(std::string _fileName, int _line, int _col, std::string _fieldName) {
        this->fileName = _fileName;
        this->line = _line;
        this->col = _col;
        this->errorType = ROOT_FIELD_DUPLICATE_ERROR;
        this->msg = "Duplicate field: " + _fieldName;
    }
};


#endif //VPARSER_ROOTFIELDDUPLICATEERROR_H
