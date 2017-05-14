//
// Created by lly on 14/05/2017.
//

#ifndef VPARSER_YAMLEMITTERERROR_H
#define VPARSER_YAMLEMITTERERROR_H


#include "Error.h"

class YAMLEmitterError: public Error {
public:
    YAMLEmitterError(int _line, int _col) {
        this->fileName = curFileName;
        this->line = _line;
        this->col = _col;
        this->errorType = YAML_EMIT_ERROR;
        this->msg = "YAML Emitter Error.";
    }
};


#endif //VPARSER_YAMLEMITTERERROR_H
