//
// Created by lly on 03/10/2017.
//

#ifndef VPARSER_INVALIDMODULEERROR_H
#define VPARSER_INVALIDMODULEERROR_H

#include "Error.h"

using namespace std;

class InvalidModuleError: public Error {
public:
    InvalidModuleError(std::string _fileName, int _line, int _col, std::string _fieldName, std::string _moduleName) {
        this->fileName = _fileName;
        this->line = _line;
        this->col = _col;
        this->errorType = INVALID_MODULE;
        this->msg = "The module '" + _moduleName + "' does not exist in this scenario (Field: " + _fieldName + ").";
    }
};

#endif //VPARSER_INVALIDMODULEERROR_H
