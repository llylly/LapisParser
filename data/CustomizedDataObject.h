//
// Created by lly on 12/07/2017.
//

#ifndef VPARSER_CUSTOMIZEDDATAOBJECT_H
#define VPARSER_CUSTOMIZEDDATAOBJECT_H


#include "BaseDataObject.h"

struct CustomizedDataObject: public BaseDataObject {
    /**
     * CAUTION: the class only wrap the passed data block, NOT copy it
     * We view customized data entity as a char sequence
     */
    unsigned char* data;
    int len;

    CustomizedDataObject(unsigned char *data, int len);
    CustomizedDataObject(const CustomizedDataObject &source);

    bool equals(BaseDataObject *b) override;
};


#endif //VPARSER_CUSTOMIZEDDATAOBJECT_H
