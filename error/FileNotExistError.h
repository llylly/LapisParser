//
// Created by lly on 31/07/2017.
//


#ifndef VPARSER_FILENOTEXISTERROR_H
#define VPARSER_FILENOTEXISTERROR_H

#include <string>
#include "Error.h"

class FileNotExistError: public Error {
public:
    FileNotExistError(std::string _fileName) {
        this->fileName = _fileName;
        this->line = 1;
        this->col = 1;
        this->errorType = FILE_NOT_EXIST;
        this->msg = "The file \"" + _fileName + "\" does not exist.";
    }
};


#endif //VPARSER_FILENOTEXISTERROR_H