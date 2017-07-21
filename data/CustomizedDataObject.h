//
// Created by lly on 12/07/2017.
//

#ifndef VPARSER_CUSTOMIZEDDATAOBJECT_H
#define VPARSER_CUSTOMIZEDDATAOBJECT_H

#include <cstring>
#include "BaseDataObject.h"

struct CustomizedDataObject: public BaseDataObject {
    /**
     * It owns the data
     * We view customized data entity as a char sequence
     */
    unsigned char* data;
    int len;

    CustomizedDataObject(unsigned char *data, int len);
    CustomizedDataObject(const CustomizedDataObject &source);
    ~CustomizedDataObject() override;

    bool equals(BaseDataObject *b) override;
};


#endif //VPARSER_CUSTOMIZEDDATAOBJECT_H
