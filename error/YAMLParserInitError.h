//
// Created by lly on 07/05/2017.
//

#ifndef VPARSER_YAMLPARSERINITERROR_H
#define VPARSER_YAMLPARSERINITERROR_H

#include "Error.h"

class YAMLParserInitError: public Error {
public:
    YAMLParserInitError(int _line, int _col) {
        this->fileName = curFileName;
        this->line = _line;
        this->col = _col;
        this->errorType = YAML_PARSER_INIT_ERROR;
        this->msg = "YAML Parser Initialize Error.";
    }
};


#endif //VPARSER_YAMLPARSERINITERROR_H
