//
// Created by lly on 08/10/2017.
//

#ifndef VPARSER_LENGTHEXCEEDTERMINATE_H
#define VPARSER_LENGTHEXCEEDTERMINATE_H

#include "RuntimeError.h"
#include <string>

using namespace std;

class LengthExceedTerminate: public RuntimeError {
public:
    LengthExceedTerminate() {
        this->type = LENGTH_EXCEED_TERMINATE;
        this->timestamp = time(0);
        this->msg = "Exceed the length limitations of test case.";
    }
};

#endif //VPARSER_LENGTHEXCEEDTERMINATE_H
