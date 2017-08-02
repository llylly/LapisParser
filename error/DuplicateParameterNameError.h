//
// Created by lly on 27/07/2017.
//

#ifndef VPARSER_DUPLICATEPARAMETERNAMEERROR_H
#define VPARSER_DUPLICATEPARAMETERNAMEERROR_H

#include <string>
#include "Error.h"

class DuplicateParameterNameError: public Error {
public:
    DuplicateParameterNameError(std::string _fileName, int _line, int _col, std::string _fieldName, std::string _name) {
        this->fileName = _fileName;
        this->line = _line;
        this->col = _col;
        this->errorType = DUPLICATE_PARAMETER_NAME;
        this->msg = "Duplicate parameter names \"" + _name + "\" found in field: " + _fieldName + ".";
    }
};


#endif //VPARSER_DUPLICATEPARAMETERNAMEERROR_H
