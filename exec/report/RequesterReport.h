//
// Created by lly on 08/10/2017.
//

#ifndef VPARSER_REQUESTERREPORT_H
#define VPARSER_REQUESTERREPORT_H

#include <string>
#include <map>
#include "BaseReport.h"
#include "../../data/BaseDataObject.h"
#include "../../data/ObjectDataObject.h"
#include "../../data/DataObjectAdapter.h"
#include "../err/RuntimeError.h"

using namespace std;

class RequesterReport: public BaseReport {
public:
    RequesterReport();
    virtual ~RequesterReport();

    virtual BaseDataObject *toDataObject() override;
    virtual ostream &printTo(ostream &os) override;

    map<string, BaseDataObject*> request;
    string url;
    string responseType;
    BaseDataObject *response;
    long long startTime;
    long long endTime;
    long long requestMilliTime;
    // below is processed but not owned
    DataSchemaObject *schema; // a hidden field, only used for scenario check things
    RuntimeError *err;
};


#endif //VPARSER_REQUESTERREPORT_H
