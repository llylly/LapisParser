//
// Created by lly on 12/07/2017.
//

#ifndef VPARSER_FILEDATAOBJECT_H
#define VPARSER_FILEDATAOBJECT_H

#include <cstring>
#include "BaseDataObject.h"

struct FileDataObject: public BaseDataObject {
    /**
     * It owns the data
     */
    unsigned char* data;
    int len;

    FileDataObject(unsigned char* data, int len);
    FileDataObject(const FileDataObject &source);
    ~FileDataObject() override;

    bool equals(BaseDataObject *b) override;
};


#endif //VPARSER_FILEDATAOBJECT_H
