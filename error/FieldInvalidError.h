//
// Created by lly on 29/05/2017.
//

#ifndef VPARSER_FIELDINVALIDERROR_H
#define VPARSER_FIELDINVALIDERROR_H


#include "Error.h"
#include "../doc/DocElement.h"

class FieldInvalidError: public Error {
public:
    FieldInvalidError(std::string _fileName, int _line, int _col, std::string _fieldName, DOC_ELEMENT_TYPE real, DOC_ELEMENT_TYPE expected) {
        std::string realStr, expectedStr;

        switch (real) {
            case DOC_OBJECT:
                realStr = "Object"; break;
            case DOC_SEQUENCE:
                realStr = "Sequence"; break;
            case DOC_SCALAR:
                realStr = "Scalar"; break;
        }

        switch (expected) {
            case DOC_OBJECT:
                expectedStr = "Object"; break;
            case DOC_SEQUENCE:
                expectedStr = "Sequence"; break;
            case DOC_SCALAR:
                expectedStr = "Scalar"; break;
        }

        this->fileName = _fileName;
        this->line = _line;
        this->col = _col;
        this->errorType = FIELD_MISS_ERROR;
        this->msg = "Field is invalid due to wrong format: \"" + _fieldName + "\". Format " + realStr + " found. " +
                "Format " + expectedStr + " expected.";
    }
};


#endif //VPARSER_FIELDINVALIDERROR_H
