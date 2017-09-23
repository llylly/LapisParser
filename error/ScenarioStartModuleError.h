//
// Created by lly on 23/09/2017.
//

#ifndef VPARSER_SCENARIOSTARTMODULEERROR_H
#define VPARSER_SCENARIOSTARTMODULEERROR_H

#include <string>
#include "Error.h"

class ScenarioStartModuleError: public Error {
public:
    ScenarioStartModuleError(std::string _fileName, int _line, int _col) {
        this->fileName = _fileName;
        this->line = _line;
        this->col = _col;
        this->errorType = SCENARIO_START_MODULE_ERROR;
        this->msg = "The scenario has no start module or more than one start modules.";
    }
};

#endif //VPARSER_SCENARIOSTARTMODULEERROR_H
