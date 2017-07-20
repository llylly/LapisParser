//
// Created by lly on 17/07/2017.
//

#ifndef VPARSER_FIELDILLEGALERROR_H
#define VPARSER_FIELDILLEGALERROR_H

#include <string>
#include "Error.h"

class FieldIllegalError: public Error {
public:
    FieldIllegalError(std::string _fileName, int _line, int _col, std::string _fieldName) {
        this->fileName = _fileName;
        this->line = _line;
        this->col = _col;
        this->errorType = FIELD_ILLEGAL_ERROR;
        this->msg = "The value of the field is illegal: \"" + _fieldName + "\".";
    }
};


#endif //VPARSER_FIELDILLEGALERROR_H
