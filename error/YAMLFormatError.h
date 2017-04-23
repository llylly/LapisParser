//
// Created by lly on 23/04/2017.
//

#ifndef VPARSER_YAMLFORMATERROR_H
#define VPARSER_YAMLFORMATERROR_H

#include "Error.h"

class YAMLFormatError: public Error {
public:
    YAMLFormatError(int _line, int _col) {
        this->line = _line;
        this->col = _col;
        this->errorType = YAML_FORMAT_ERROR;
        this->msg = "YAML File Format Error.";
    }
};


#endif //VPARSER_YAMLFORMATERROR_H
