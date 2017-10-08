//
// Created by lly on 06/10/2017.
//

#include "Logger.h"

std::vector<Logger*> Logger::logs;

Logger::Logger() {
    this->timestamp = time(0);
    this->level = 0;
    this->msg = "";
}

BaseDataObject *Logger::toDataObject() {
    ObjectDataObject *obj = new ObjectDataObject();
    (*obj)["timestamp"] = new IntegerDataObject(this->timestamp);
    (*obj)["level"] = new IntegerDataObject(this->level);
    (*obj)["msg"] = new StringDataObject(this->msg);
    return obj;
}

void Logger::addLog(Logger *_log) {
    Logger::logs.push_back(_log);
}

void Logger::cleanLog() {
    Logger::logs.clear();
}

void Logger::printLog(std::ostream &os, int level) {
    for (std::vector<Logger*>::iterator ite = Logger::logs.begin();
            ite != Logger::logs.end();
            ++ite) {
        Logger *now = *ite;
        if (now->level >= level) {
            os << "[time: " << (now->timestamp) << " level: " << (now->level) << "] "
               << now->msg << std::endl;
        }
    }
}

std::vector<Logger*> *Logger::getLogs() {
    return &Logger::logs;
}