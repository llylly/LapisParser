//
// Created by lly on 22/09/2017.
//

#ifndef VPARSER_SCENARIOINITIALSTATEERROR_H
#define VPARSER_SCENARIOINITIALSTATEERROR_H

#include "Error.h"

class ScenarioInitialStateError: public Error {
public:
    ScenarioInitialStateError(std::string _fileName, int _line, int _col) {
        this->fileName = _fileName;
        this->line = _line;
        this->col = _col;
        this->errorType = SCENARIO_INITIAL_STATE_ERROR;
        this->msg = "The scenario has no initial state or more than one initial states.";
    }
};

#endif //VPARSER_SCENARIOINITIALSTATEERROR_H
