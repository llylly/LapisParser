//
// Created by lly on 12/07/2017.
//

#ifndef VPARSER_STRINGDATAOBJECT_H
#define VPARSER_STRINGDATAOBJECT_H

#include <string>
#include "BaseDataObject.h"

using namespace std;

struct StringDataObject: public BaseDataObject {
    string str;

    StringDataObject(string s);

    StringDataObject(const StringDataObject &source);

    bool equals(BaseDataObject *b) override;
};


#endif //VPARSER_STRINGDATAOBJECT_H
