//
// Created by lly on 28/07/2017.
//

#ifndef VPARSER_ILLEGALCONSUMESFORFORMDATA_H
#define VPARSER_ILLEGALCONSUMESFORFORMDATA_H


#include "Error.h"

class IllegalConsumesForFormData: public Error {
public:
    IllegalConsumesForFormData(std::string _fileName, int _line, int _col) {
        this->fileName = _fileName;
        this->line = _line;
        this->col = _col;
        this->errorType = ILLEGAL_CONSUMES_FOR_FORMDATA;
        this->msg = "This operation has formData parameter, but it does not consume form MIMEs.";
    }
};


#endif //VPARSER_ILLEGALCONSUMESFORFORMDATA_H
