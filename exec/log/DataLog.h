//
// Created by lly on 06/10/2017.
//

#ifndef VPARSER_DATALOG_H
#define VPARSER_DATALOG_H

#include "Logger.h"
#include "../../doc/DocElement.h"
#include "../../data/BaseDataObject.h"
#include "../../data/DataObjectAdapter.h"
#include <sstream>

class DataLog: public Logger {
public:
    DataLog(DocElement *obj, std::string str = "", int level = 2) {
        this->timestamp = time(0);
        this->level = level;
        stringstream strs;
        obj->printTo(strs);
        strs.flush();
        this->msg = str + " " + strs.str();
    }

    DataLog(BaseDataObject *obj, std::string str = "", int level = 2) {
        this->timestamp = time(0);
        this->level = level;
        stringstream strs;
        DataObjectAdapter::toDocElement(obj)->printTo(strs);
        strs.flush();
        this->msg = str + " " + strs.str();
    }
};

#endif //VPARSER_DATALOG_H
