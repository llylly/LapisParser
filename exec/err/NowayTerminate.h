//
// Created by lly on 08/10/2017.
//

#ifndef VPARSER_NOWAYTERMINATE_H
#define VPARSER_NOWAYTERMINATE_H

#include "RuntimeError.h"
#include <string>

using namespace std;

class NowayTerminate: public RuntimeError {
public:
    NowayTerminate(string moduleName) {
        this->type = NO_WAY_TERMINATE;
        this->timestamp = time(0);
        this->msg = "There's no available out edge from this module \"" + moduleName + "\". Terminated."
                + " This can also be caused by too hard API constraints.";
    }
};

#endif //VPARSER_NOWAYTERMINATE_H
