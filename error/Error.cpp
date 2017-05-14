//
// Created by lly on 23/04/2017.
//

#include "Error.h"

std::string Error::curFileName = "";

std::vector<Error*> Error::errorPool;

void Error::addError(Error *_err) {
    Error::errorPool.push_back(_err);
}

bool Error::hasError() {
    return (!Error::errorPool.empty());
}

void Error::printError(std::ostream &os) {
    for (std::vector<Error*>::iterator ite = Error::errorPool.begin();
            ite != Error::errorPool.end();
            ++ite) {
        Error *cur = *ite;
        os << "(File: " << cur->fileName << " Line: " << cur->line << " Col: " << cur->col << ") " << cur->msg << std::endl;
    }
}