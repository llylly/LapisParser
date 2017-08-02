//
// Created by lly on 28/07/2017.
//

#ifndef VPARSER_ILLEGALFILEPARAMETERERROR_H
#define VPARSER_ILLEGALFILEPARAMETERERROR_H


#include <string>
#include "Error.h"

class IllegalFileParameterError: public Error {
public:
    IllegalFileParameterError(std::string _fileName, int _line, int _col, std::string _paramName) {
        this->fileName = _fileName;
        this->line = _line;
        this->col = _col;
        this->errorType = ILLEGAL_FILE_PARAMETER_ERROR;
        this->msg = "Illegal file parameter \"" + _paramName + "\": 'in' field should be formData.";
    }
};


#endif //VPARSER_ILLEGALFILEPARAMETERERROR_H
