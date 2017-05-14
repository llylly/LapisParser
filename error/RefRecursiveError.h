//
// Created by lly on 08/05/2017.
//

#ifndef VPARSER_REFRECURSIVEERROR_H
#define VPARSER_REFRECURSIVEERROR_H

#include "Error.h"

class RefRecursiveError: public Error {
public:
    RefRecursiveError(int _line, int _col) {
        this->fileName = curFileName;
        this->line = _line;
        this->col = _col;
        this->errorType = REF_RECURSIVE_ERROR;
        this->msg = "Recursive Reference.";
    }
};


#endif //VPARSER_REFRECURSIVEERROR_H
