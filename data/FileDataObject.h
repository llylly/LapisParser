//
// Created by lly on 12/07/2017.
//

#ifndef VPARSER_FILEDATAOBJECT_H
#define VPARSER_FILEDATAOBJECT_H


#include "BaseDataObject.h"

struct FileDataObject: public BaseDataObject {
    /**
     * CAUTION: the class only wrap the passed data block, NOT copy it
     */
    unsigned char* data;
    int len;

    FileDataObject(unsigned char* data, int len);
    FileDataObject(const FileDataObject &source);

    bool equals(BaseDataObject *b) override;
};


#endif //VPARSER_FILEDATAOBJECT_H
