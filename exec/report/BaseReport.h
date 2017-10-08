//
// Created by lly on 08/10/2017.
//

#ifndef VPARSER_BASEREPORT_H
#define VPARSER_BASEREPORT_H

#include <iostream>
#include "../../data/BaseDataObject.h"
#include "../log/Logger.h"
#include "../log/StrLog.h"

using namespace std;

class BaseReport {
public:
    virtual BaseDataObject *toDataObject() = 0;
    virtual ostream &printTo(ostream &os) = 0;
};


#endif //VPARSER_BASEREPORT_H
