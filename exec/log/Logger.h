//
// Created by lly on 06/10/2017.
//

#ifndef VPARSER_LOGGER_H
#define VPARSER_LOGGER_H

#include <string>
#include <vector>
#include <iostream>

class Logger {

public:
    long long timestamp;
    int level;
    std::string msg;

    Logger();

    static void addLog(Logger *_log);
    static void cleanLog();
    static void printLog(std::ostream &os, int level=0);
    static std::vector<Logger*> *getLogs();


private:
    static std::vector<Logger*> logs;
};


#endif //VPARSER_LOGGER_H
