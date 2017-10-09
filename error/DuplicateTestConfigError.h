//
// Created by lly on 03/10/2017.
//

#ifndef VPARSER_DUPLICATETESTCONFIGERROR_H
#define VPARSER_DUPLICATETESTCONFIGERROR_H

#include "Error.h"

class DuplicateTestConfigError: public Error {
public:
    DuplicateTestConfigError() {
        this->fileName = "";
        this->line = 1;
        this->col = 1;
        this->errorType = DUPLICATE_TEST_CONFIG;
        this->msg = "Duplicate test config object found.";
    }
};

#endif //VPARSER_DUPLICATETESTCONFIGERROR_H
