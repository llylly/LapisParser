//
// Created by lly on 29/05/2017.
//

#ifndef VPARSER_FIELDMISSERROR_H
#define VPARSER_FIELDMISSERROR_H


#include "Error.h"

class FieldMissError: public Error {
public:
    FieldMissError(std::string _fileName, int _line, int _col, std::string _fieldName) {
        this->fileName = _fileName;
        this->line = _line;
        this->col = _col;
        this->errorType = FIELD_MISS_ERROR;
        this->msg = "Required field not found: \"" + _fieldName + "\".";
    }
};


#endif //VPARSER_FIELDMISSERROR_H
