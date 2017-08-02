//
// Created by lly on 28/07/2017.
//

#ifndef VPARSER_TOOMANYBODYPARAMERROR_H
#define VPARSER_TOOMANYBODYPARAMERROR_H

#include <string>
#include "Error.h"

class TooManyBodyParamError: public Error {
public:
    TooManyBodyParamError(std::string _fileName, int _line, int _col) {
        this->fileName = _fileName;
        this->line = _line;
        this->col = _col;
        this->errorType = TOO_MANY_BADY_PARAM;
        this->msg = "This operation has multiple Body parameters. Only one Body parameter is permitted.";
    }
};


#endif //VPARSER_TOOMANYBODYPARAMERROR_H
