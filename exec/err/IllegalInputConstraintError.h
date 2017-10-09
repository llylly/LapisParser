//
// Created by lly on 07/10/2017.
//

#ifndef VPARSER_ILLEGALINPUTCONSTRAINTERROR_H
#define VPARSER_ILLEGALINPUTCONSTRAINTERROR_H

#include "RuntimeError.h"
#include <string>

using namespace std;

class IllegalInputConstraintError: public RuntimeError {
public:
    IllegalInputConstraintError(string inputConstraint) {
        this->type = ILLEGAL_INPUTCONSTRAINT;
        this->timestamp = time(0);
        this->msg = "The input constraint cannot be satisfied: " + inputConstraint;
    }
};

#endif //VPARSER_ILLEGALINPUTCONSTRAINTERROR_H
