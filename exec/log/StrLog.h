//
// Created by lly on 06/10/2017.
//

#ifndef VPARSER_STRLOG_H
#define VPARSER_STRLOG_H

#include "Logger.h"

class StrLog: public Logger {
public:
    StrLog(std::string str, int level=1) {
        this->timestamp = time(0);
        this->level = level;
        this->msg = str;
    }
};

#endif //VPARSER_STRLOG_H
