//
// Created by lly on 13/05/2017.
//

#ifndef VPARSER_YAMLEMITTERINITERROR_H
#define VPARSER_YAMLEMITTERINITERROR_H


#include "Error.h"

class YAMLEmitterInitError: public Error {
public:
    YAMLEmitterInitError(int _line, int _col) {
        this->fileName = curFileName;
        this->line = _line;
        this->col = _col;
        this->errorType = YAML_EMITTER_INIT_ERROR;
        this->msg = "YAML Emitter Initialize Error.";
    }
};


#endif //VPARSER_YAMLEMITTERINITERROR_H
