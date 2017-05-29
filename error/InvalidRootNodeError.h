//
// Created by lly on 29/05/2017.
//

#ifndef VPARSER_INVALIDROOTNODEERROR_H
#define VPARSER_INVALIDROOTNODEERROR_H


#include "Error.h"

class InvalidRootNodeError: public Error {
public:
    InvalidRootNodeError(std::string _fileName, int _line, int _col) {
        this->fileName = _fileName;
        this->line = _line;
        this->col = _col;
        this->errorType = INVALID_ROOT_NODE_ERROR;
        this->msg = "Invalid root node. The root node should be a map.";
    }

};


#endif //VPARSER_INVALIDROOTNODEERROR_H
