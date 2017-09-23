//
// Created by lly on 22/09/2017.
//

#ifndef VPARSER_DUPLICATESCENARIONAMES_H
#define VPARSER_DUPLICATESCENARIONAMES_H


#include "Error.h"
#include <string>

class DuplicateScenarioNamesError: public Error {
public:
    DuplicateScenarioNamesError(std::string _fileName, int _line, int _col, std::string _fieldName, std::string _name) {
        this->fileName = _fileName;
        this->line = _line;
        this->col = _col;
        this->errorType = DUPLICATE_SCENARIO_NAME;
        this->msg = "Duplicate scenario names \"" + _name + "\" found in field: " + _fieldName + ".";
    }
};


#endif //VPARSER_DUPLICATESCENARIONAMES_H
