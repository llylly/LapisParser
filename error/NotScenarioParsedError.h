//
// Created by lly on 22/09/2017.
//

#ifndef VPARSER_NOTSCENARIOPARSEDERROR_H
#define VPARSER_NOTSCENARIOPARSEDERROR_H

#include "Error.h"

class NotScenarioParsedError: public Error {
public:
    NotScenarioParsedError() {
        this->fileName = "";
        this->line = 1;
        this->col = 1;
        this->errorType = NOT_SCENARIO_PARSED;
        this->msg = "The scenarios has not been parsed to internal representation. The request is invalid.";
    }
};

#endif //VPARSER_NOTSCENARIOPARSEDERROR_H
