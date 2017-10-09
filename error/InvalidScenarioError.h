//
// Created by lly on 05/10/2017.
//

#ifndef VPARSER_INVALIDSCENARIOERROR_H
#define VPARSER_INVALIDSCENARIOERROR_H

#include "Error.h"

using namespace std;

class InvalidScenarioError: public Error {
public:
    InvalidScenarioError(std::string _fileName, int _line, int _col, std::string _fieldName, std::string _scenarioName) {
        this->fileName = _fileName;
        this->line = _line;
        this->col = _col;
        this->errorType = INVALID_SCENARIO;
        this->msg = "The module '" + _scenarioName + "' does not exist in scenario definition (Field: " + _fieldName + ").";
    }
};

#endif //VPARSER_INVALIDSCENARIOERROR_H
