//
// Created by lly on 31/07/2017.
//

#ifndef VPARSER_DOCNODENOTEXISTERROR_H
#define VPARSER_DOCNODENOTEXISTERROR_H

#include "Error.h"

class DocNodeNotExistError: public Error {
public:
    DocNodeNotExistError(string fieldName) {
        this->fileName = "/";
        this->line = 1;
        this->col = 1;
        this->errorType = DOC_NODE_NOT_EXIST;
        this->msg = "The \"" + fieldName + "\" does not exist in current doc element tree.";
    }
};

#endif //VPARSER_DOCNODENOTEXISTERROR_H
