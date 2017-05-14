//
// Created by lly on 14/05/2017.
//

#ifndef VPARSER_XMLEMITTERERROR_H
#define VPARSER_XMLEMITTERERROR_H

#include "Error.h"

class XMLEmitterError: public Error {
public:
    XMLEmitterError(int _line, int _col) {
        this->fileName = curFileName;
        this->line = _line;
        this->col = _col;
        this->errorType = XML_EMIT_ERROR;
        this->msg = "XML Emitter Error.";
    }
};


#endif //VPARSER_XMLEMITTERERROR_H
