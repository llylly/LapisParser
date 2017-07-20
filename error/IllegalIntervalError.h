//
// Created by lly on 19/07/2017.
//

#ifndef VPARSER_ILLEGALINTERVAL_H
#define VPARSER_ILLEGALINTERVAL_H

#include <string>
#include <sstream>
#include "Error.h"

class IllegalIntervalError: public Error {
public:
    IllegalIntervalError(std::string _fileName, int _line, int _col, std::string _fieldName, double l, double r) {
        this->fileName = _fileName;
        this->line = _line;
        this->col = _col;
        this->errorType = ILLEGAL_INTERVAL;
        std::stringstream ss;
        ss << "The interval in this field is illegal: " + _fieldName + ".";
        ss << " ";
        ss << "Left bound: " << l << "; right bound: " << r << ".";
        ss.flush();
        this->msg = ss.str();
    }
};


#endif //VPARSER_ILLEGALINTERVAL_H
