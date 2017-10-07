//
// Created by lly on 07/10/2017.
//

#ifndef VPARSER_SETEMPTYTERMINATE_H
#define VPARSER_SETEMPTYTERMINATE_H

#include "RuntimeError.h"
#include <string>

using namespace std;

class SetEmptyTerminate: public RuntimeError {
public:
    SetEmptyTerminate(string inputConstraint, string setName) {
        this->type = SET_EMPTY_TERMINATE;
        this->timestamp = time(0);
        this->msg = "The input constraint \"" + inputConstraint + "\" cannot be satisfied "
                                                                          "because the set \"" + setName + "\" is empty now.";
    }
};

#endif //VPARSER_SETEMPTYTERMINATE_H
