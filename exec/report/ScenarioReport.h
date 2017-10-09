//
// Created by lly on 08/10/2017.
//

#ifndef VPARSER_SCENARIOREPORT_H
#define VPARSER_SCENARIOREPORT_H

#include "BaseReport.h"
#include "TestCaseReport.h"
#include "../../data/SequenceDataObject.h"

class ScenarioReport: public BaseReport {
public:
    ScenarioReport();
    ~ScenarioReport();
    virtual BaseDataObject *toDataObject() override;
    virtual ostream &printTo(ostream &os) override;

    void start(string scenarioName);
    void addTestCase(TestCaseReport *report);
    void end();

private:
    long long startTime;
    long long endTime;
    long long lastTime;
    string scenarioName;
    /** owned them **/
    vector<TestCaseReport*> testCases;
};


#endif //VPARSER_SCENARIOREPORT_H
