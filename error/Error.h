//
// Created by lly on 23/04/2017.
//

#ifndef VPARSER_ERROR_H
#define VPARSER_ERROR_H

#include <string>
#include <vector>
#include <iostream>

enum ERROR_TYPE {
    NO_TYPE, YAML_FORMAT_ERROR, YAML_PARSER_INIT_ERROR, XML_FORMAT_ERROR, REF_NOT_FOUND_ERROR, REF_RECURSIVE_ERROR,
    YAML_EMITTER_INIT_ERROR, YAML_EMIT_ERROR, XML_EMIT_ERROR,
    INVALID_ROOT_NODE_ERROR, INVALID_SWAGGER_SYMBOL_ERROR, INVALID_X_VEE_SYMBOL_ERROR, ROOT_FIELD_DUPLICATE_ERROR, FIELD_MISS_ERROR, FIELD_INVALID_ERROR, INVALID_SCHEME_ERROR
};

class Error {

public:
    ERROR_TYPE errorType;
    std::string fileName;
    int line, col;
    std::string msg;

    Error(): line(-1), col(-1) {
        errorType = NO_TYPE;
        msg = "Abstract Error";
    }

    static std::string curFileName;

    static void addError(Error *_err);
    static bool hasError();
    static void printError(std::ostream &os);

    static std::vector<Error*> *getErrors();

private:
    static std::vector<Error*> errorPool;
};


#endif //VPARSER_ERROR_H
