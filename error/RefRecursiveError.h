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
        this->msg = "Recursion too match when parsing references. Maybe there are some infinite recursive references.";
    }

    RefRecursiveError(std::string _fileName, int _line, int _col) {
            this->fileName = _fileName;
            this->line = _line;
            this->col = _col;
            this->errorType = REF_RECURSIVE_ERROR;
            this->msg = "Recursion too match when parsing references. Maybe there are some infinite recursive references.";
    }
};


#endif //VPARSER_REFRECURSIVEERROR_H
