//
// Created by lly on 09/10/2017.
//

#ifndef VPARSER_ILLEGALSETEFFECTERROR_H
#define VPARSER_ILLEGALSETEFFECTERROR_H

#include "RuntimeError.h"
#include <string>

using namespace std;

class IllegalSetEffectError: public RuntimeError {
public:
    IllegalSetEffectError(string moduleName) {
        this->type = ILLEGAL_SET_EFFECT;
        this->timestamp = time(0);
        this->msg = "The set effect of module \"" + moduleName + "\" cannot be done because of inconsistency "
                                                                         "between definition and response.";
    }
};

#endif //VPARSER_ILLEGALSETEFFECTERROR_H
