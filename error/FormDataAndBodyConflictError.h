//
// Created by lly on 28/07/2017.
//

#ifndef VPARSER_FORMDATAANDBODYCONFLICTERROR_H
#define VPARSER_FORMDATAANDBODYCONFLICTERROR_H


#include "Error.h"

class FormDataAndBodyConflictError: public Error {
public:
    FormDataAndBodyConflictError(std::string _fileName, int _line, int _col) {
        this->fileName = _fileName;
        this->line = _line;
        this->col = _col;
        this->errorType = FORMDATA_AND_BODY_CONFLICT;
        this->msg = "This operation has both FormData parameter and Body parameter, which is not permitted.";
    }
};


#endif //VPARSER_FORMDATAANDBODYCONFLICTERROR_H
