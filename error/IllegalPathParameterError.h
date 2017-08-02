//
// Created by lly on 28/07/2017.
//

#ifndef VPARSER_ILLEGALPATHPARAMETERERROR_H
#define VPARSER_ILLEGALPATHPARAMETERERROR_H


#include "Error.h"

class IllegalPathParameterError: public Error {
public:
    IllegalPathParameterError(std::string _fileName, int _line, int _col, std::string _paramName) {
        this->fileName = _fileName;
        this->line = _line;
        this->col = _col;
        this->errorType = ILLEGAL_PATH_PARAMETER_ERROR;
        this->msg = "Illegal path parameter name \"" + _paramName + "\". Could not match the name to the operation path.";
    }
};


#endif //VPARSER_ILLEGALPATHPARAMETERERROR_H
