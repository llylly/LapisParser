//
// Created by lly on 18/07/2017.
//

#ifndef VPARSER_ILLEGALENUMOBJECT_H
#define VPARSER_ILLEGALENUMOBJECT_H

#include <string>
#include "Error.h"

using namespace std;

class IllegalEnumObjectError: public Error {

public:
    IllegalEnumObjectError(std::string _fileName, int _line, int _col, std::string _fieldName) {
        this->fileName = _fileName;
        this->line = _line;
        this->col = _col;
        this->errorType = ILLEGAL_ENUM_OBJECT;
        this->msg = "The enum element is incompatible with the defined schema. Field name: " + _fieldName + ".";
    }
};


#endif //VPARSER_ILLEGALENUMOBJECT_H
