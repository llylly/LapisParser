//
// Created by lly on 08/10/2017.
//

#ifndef VPARSER_REQUESTERINITERROR_H
#define VPARSER_REQUESTERINITERROR_H

#include "RuntimeError.h"
#include <string>

using namespace std;

class RequesterInitError: public RuntimeError {
public:
    RequesterInitError() {
        this->type = REQUESTER_INIT_ERR;
        this->timestamp = time(0);
        this->msg = "Requester initialization error.";
    }
};

#endif //VPARSER_REQUESTERINITERROR_H
