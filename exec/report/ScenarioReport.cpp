//
// Created by lly on 08/10/2017.
//

#include "ScenarioReport.h"

ScenarioReport::ScenarioReport() {
    this->startTime = 0LL;
    this->endTime = 0LL;
    this->lastTime = 0LL;
    this->testCases.clear();
}

ScenarioReport::~ScenarioReport() {
    for (vector<TestCaseReport*>::iterator ite = this->testCases.begin();
            ite != this->testCases.end();
            ++ite) {
        delete *ite;
    }
    this->testCases.clear();
}

BaseDataObject *ScenarioReport::toDataObject() {
    ObjectDataObject *ans = new ObjectDataObject();
    (*ans)["scenarioName"] = new StringDataObject(this->scenarioName);
    (*ans)["startTime"] = new IntegerDataObject(this->startTime);
    (*ans)["endTime"] = new IntegerDataObject(this->endTime);
    (*ans)["lastTime"] = new IntegerDataObject(this->lastTime);
    (*ans)["totTestCase"] = new IntegerDataObject(this->testCases.size());
    SequenceDataObject *seq = new SequenceDataObject();
    for (vector<TestCaseReport*>::iterator ite = this->testCases.begin();
            ite != this->testCases.end();
            ++ite) {
        seq->push((*ite)->toDataObject());
    }
    (*ans)["testCases"] = seq;
    return ans;
}

ostream &ScenarioReport::printTo(ostream &os) {
    os << "*-----------------*" << endl;
    os << "|Scenario Report  |" << endl;
    os << "*-----------------*" << endl;
    os << "Scenario Name: " << this->scenarioName << endl;
    os << "Start Time:    " << this->startTime << endl;
    os << "End Time:      " << this->endTime << endl;
    os << "Last Time:     " << this->lastTime << endl;
    os << "Test Case #:   " << this->testCases.size() << endl;
    os << "*-----------------*" << endl;
    for (int i = 0; i < this->testCases.size(); ++i) {
        os << "Case #" << i << ":" << endl;
        os << "*********" << endl;
        this->testCases[i]->printTo(os);
        os << endl;
    }
    os.flush();
    return os;
}

void ScenarioReport::start(string scenarioName) {
    this->scenarioName = scenarioName;
    this->startTime = time(0);
}

void ScenarioReport::addTestCase(TestCaseReport *report) {
    this->testCases.push_back(report);
}

void ScenarioReport::end() {
    this->endTime = time(0);
    this->lastTime = this->endTime - this->startTime;
}
