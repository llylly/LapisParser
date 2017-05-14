//
// Created by lly on 23/04/2017.
//

#ifndef VPARSER_XMLFORMATERROR_H
#define VPARSER_XMLFORMATERROR_H

#include "Error.h"

class XMLFormatError: public Error {
public:
    XMLFormatError(int _line, int _col) {
        this->fileName = curFileName;
        this->line = _line;
        this->col = _col;
        this->errorType = XML_FORMAT_ERROR;
        this->msg = "XML File Format Error.";
    }
};


#endif //VPARSER_XMLFORMATERROR_H
