//
// Created by lly on 08/10/2017.
//

#ifndef VPARSER_TESTCASEREPORT_H
#define VPARSER_TESTCASEREPORT_H

#include "BaseReport.h"
#include "../../doc/DocElement.h"
#include "RequesterReport.h"
#include "../../data/SequenceDataObject.h"
#include "../err/LengthExceedTerminate.h"

struct TestCaseStepRecord: public BaseReport {
    // all things are self-owned, except "err" of request object
    string moduleName;
    map<string, vector<DocElement*>> sets;
    string stateName;
    /** only for normal module **/
    RequesterReport *request;

    TestCaseStepRecord();
    ~TestCaseStepRecord();
    BaseDataObject *toDataObject() override;
    ostream &printTo(ostream &os) override;
};

class TestCaseReport: public BaseReport {
public:
    virtual BaseDataObject *toDataObject() override;
    virtual ostream &printTo(ostream &os) override;

    TestCaseReport();
    ~TestCaseReport();

    void start();
    void end();
    void addStep(TestCaseStepRecord *step);
    void setTerminate(RuntimeError *err);
    RuntimeError *getTerminate();

private:
    long long startTime;
    long long endTime;
    long long lastTime;
    vector<TestCaseStepRecord*> steps;
    // may just points to a request error, or specifies a terminate reason.
    // anyway, they are been deleted here
    RuntimeError *terminate;
};


#endif //VPARSER_TESTCASEREPORT_H
