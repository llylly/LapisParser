//
// Created by lly on 08/10/2017.
//

#ifndef VPARSER_CHECKPOINTNOTPASSTERMINATE_H
#define VPARSER_CHECKPOINTNOTPASSTERMINATE_H

#include "RuntimeError.h"
#include <string>

using namespace std;

class CheckpointNotPassTerminate: public RuntimeError {
public:
    CheckpointNotPassTerminate(string moduleName) {
        this->type = CHECKPOINT_NOT_PASS_TERMINATE;
        this->timestamp = time(0);
        this->msg = "The response does not satisfy checkpoints in module " + moduleName;
    }
};

#endif //VPARSER_CHECKPOINTNOTPASSTERMINATE_H
