//
// Created by lly on 22/09/2017.
//

#ifndef VPARSER_DUPLICATESCENARIOELEMENT_H
#define VPARSER_DUPLICATESCENARIOELEMENT_H

#include "Error.h"

class DuplicateScenarioElementError: public Error {
public:
    DuplicateScenarioElementError(std::string _fileName, int _line, int _col, std::string _fieldName) {
        this->fileName = _fileName;
        this->line = _line;
        this->col = _col;
        this->errorType = DUPLICATE_SCENARIO_ELEMENT_NAME;
        this->msg = "Duplicate scenario element name found in field: " + _fieldName + ".";
    }
};

#endif //VPARSER_DUPLICATESCENARIOELEMENT_H
