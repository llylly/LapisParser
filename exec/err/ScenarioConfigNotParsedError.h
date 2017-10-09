//
// Created by lly on 08/10/2017.
//

#ifndef VPARSER_SCENARIOCONFIGNOTPARSEDERROR_H
#define VPARSER_SCENARIOCONFIGNOTPARSEDERROR_H

#include "RuntimeError.h"
#include <string>

using namespace std;

class ScenarioConfigNotParsedError: public RuntimeError {
public:
    ScenarioConfigNotParsedError() {
        this->type = SCENARIO_CONFIG_NOT_PARSED;
        this->timestamp = time(0);
        this->msg = "The execution config, scenario definitions or API definitions hasn't been parsed.";
    }
};

#endif //VPARSER_SCENARIOCONFIGNOTPARSEDERROR_H
