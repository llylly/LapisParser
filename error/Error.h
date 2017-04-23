//
// Created by lly on 23/04/2017.
//

#ifndef VPARSER_ERROR_H
#define VPARSER_ERROR_H

#include <string>
#include <vector>
#include <iostream>

enum ERROR_TYPE {
    NO_TYPE, YAML_FORMAT_ERROR, XML_FORMAT_ERROR
};

class Error {

public:
    ERROR_TYPE errorType;
    int line, col;
    std::string msg;

    Error(): line(-1), col(-1) {
        errorType = NO_TYPE;
        msg = "Abstract Error";
    }

    static void addError(Error *_err);
    static bool hasError();
    static void printError(std::ostream &os);

private:
    static std::vector<Error*> errorPool;
};


#endif //VPARSER_ERROR_H
