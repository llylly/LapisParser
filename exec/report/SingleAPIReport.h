//
// Created by lly on 08/10/2017.
//

#ifndef VPARSER_SINGLEAPIREPORT_H
#define VPARSER_SINGLEAPIREPORT_H


#include "RequesterReport.h"

class SingleAPIReport: public RequesterReport {
public:
    SingleAPIReport();
    ~SingleAPIReport();

    BaseDataObject *toDataObject() override;
    ostream &printTo(ostream &os) override;

    bool useAliMiddleware;
};


#endif //VPARSER_SINGLEAPIREPORT_H
