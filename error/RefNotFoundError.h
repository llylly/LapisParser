//
// Created by lly on 08/05/2017.
//

#ifndef VPARSER_REFNOTFOUNDERROR_H
#define VPARSER_REFNOTFOUNDERROR_H

#include "Error.h"

class RefNotFoundError: public Error {
public:
    RefNotFoundError(int _line, int _col) {
        this->fileName = Error::curFileName;
        this->line = _line;
        this->col = _col;
        this->errorType = REF_NOT_FOUND_ERROR;
        this->msg = "Illegal path of reference object.";
    }

    RefNotFoundError(std::string _fileName, int _line, int _col) {
        this->fileName = _fileName;
        this->line = _line;
        this->col = _col;
        this->errorType = REF_NOT_FOUND_ERROR;
        this->msg = "Illegal path of reference object.";
    }

};


#endif //VPARSER_REFNOTFOUNDERROR_H
