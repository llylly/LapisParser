//
// Created by lly on 08/10/2017.
//

#include "TestCaseReport.h"

TestCaseStepRecord::TestCaseStepRecord() {
    this->moduleName = "";
    this->sets.clear();
    this->stateName = "";
    this->request = NULL;
}

TestCaseStepRecord::~TestCaseStepRecord() {
    if (this->request != NULL)
        delete this->request;
    for (map<string, vector<DocElement*>>::iterator ite = this->sets.begin();
         ite != this->sets.end();
         ++ite) {
        vector<DocElement *> &setVec = ite->second;
        for (vector<DocElement*>::iterator iite = setVec.begin();
                iite != setVec.end();
                ++iite)
            delete *iite;
    }
}

BaseDataObject *TestCaseStepRecord::toDataObject() {
    ObjectDataObject *obj = new ObjectDataObject();
    (*obj)["moduleName"] = new StringDataObject(this->moduleName);

    ObjectDataObject *setsObj = new ObjectDataObject();
    for (map<string, vector<DocElement*>>::iterator ite = this->sets.begin();
            ite != this->sets.end();
            ++ite) {
        vector<DocElement*> &setVec = ite->second;
        SequenceDataObject *setSeq = new SequenceDataObject();
        for (vector<DocElement*>::iterator iite = setVec.begin();
                iite != setVec.end();
                ++iite) {
            setSeq->push(DataObjectAdapter::defaultFromDocElement(*iite));
        }
        (*setsObj)[ite->first] = setSeq;
    }
    (*obj)["sets"] = setsObj;

    (*obj)["stateName"] = new StringDataObject(this->stateName);

    if (request != NULL) {
        ObjectDataObject *requestObj = new ObjectDataObject();
        for (map<string, BaseDataObject*>::iterator ite = request->request.begin();
             ite != request->request.end();
             ++ite)
            if (ite->second) {
                (*requestObj)[ite->first] = DataObjectAdapter::deepCopy(ite->second);
            }
        (*obj)["request"] = requestObj;

        if (request->url != "")
            (*obj)["url"] = new StringDataObject(request->url);

        if (request->responseType != "")
            (*obj)["responseType"] = new StringDataObject(request->responseType);

        if (request->response != NULL) {
            (*obj)["response"] = DataObjectAdapter::deepCopy(request->response);
        }

        if (request->requestMilliTime != 0LL)
            (*obj)["requestMilliTime"] = new IntegerDataObject(request->requestMilliTime);

        if (request->err != NULL) {
            (*obj)["err"] = request->err->toDataObject();
        }
    }
    return obj;
}

ostream &TestCaseStepRecord::printTo(ostream &os) {
    os << "---Case Step Report Begin---" << endl;
    os << "Module Name: " << this->moduleName << endl;
    os << "State Name: " << this->stateName << endl;
    os << "Sets:" << endl;
    for (map<string, vector<DocElement*>>::iterator ite = this->sets.begin();
            ite != this->sets.end();
            ++ite) {
        os << "{" << ite->first << ":" << endl;
        vector<DocElement*> &vec = ite->second;
        os << "[";
        for (int i = 0; i < vec.size(); ++i) {
            if (i>0) os << ",";
            vec[i]->printTo(os);
        }
        os  << "]}" << endl;
    }
    if (this->request)
        this->request->printTo(os);
    os << "---Case Step Report End---" << endl;
    return os;
}

/** -------------------------- **/

TestCaseReport::TestCaseReport() {
    this->startTime = 0LL;
    this->endTime = 0LL;
    this->lastTime = 0LL;
    this->steps.clear();
    this->terminate = NULL;
}

TestCaseReport::~TestCaseReport() {
    for (vector<TestCaseStepRecord*>::iterator ite = this->steps.begin();
            ite != this->steps.end();
            ++ite)
        delete *ite;
    if (this->terminate != NULL)
        delete this->terminate;
}

BaseDataObject *TestCaseReport::toDataObject() {
    ObjectDataObject *obj = new ObjectDataObject();
    (*obj)["startTime"] = new IntegerDataObject(this->startTime);
    (*obj)["endTime"] = new IntegerDataObject(this->endTime);
    (*obj)["lastTime"] = new IntegerDataObject(this->lastTime);
    if (this->steps.size() > 0) {
        SequenceDataObject* stepSeq = new SequenceDataObject();
        for (vector<TestCaseStepRecord*>::iterator ite = this->steps.begin();
                ite != this->steps.end();
                ++ite) {
            stepSeq->push((*ite)->toDataObject());
        }
        (*obj)["steps"] = stepSeq;
    }
    if (this->terminate != NULL)
        (*obj)["terminate"] = this->terminate->toDataObject();
    return obj;
}

ostream &TestCaseReport::printTo(ostream &os) {
    os << "---Test Case Report Begin---" << endl;
    os << "Steps #:    " << this->steps.size() << endl;
    os << "Start Time: " << this->startTime << endl;
    os << "End Time:   " << this->endTime << endl;
    os << "Last Time:  " << this->lastTime << endl;
    os << "Steps: " << endl;
    for (int i = 0; i < this->steps.size(); ++i) {
        os << "Step " << i << ":" << endl;
        this->steps[i]->printTo(os);
        os << "*****" << endl;
    }
    os << "---Test Case Report End---" << endl;
    return os;
}

void TestCaseReport::start() {
    this->startTime = time(0);
}

void TestCaseReport::end() {
    this->endTime = time(0);
    this->lastTime = this->endTime - this->startTime;
}

void TestCaseReport::addStep(TestCaseStepRecord *step) {
    if (step == NULL) return;
    this->steps.push_back(step);
    if ((step->request) && (step->request->err)) {
        if (this->terminate) delete this->terminate;
        this->terminate = step->request->err;
    }
}

void TestCaseReport::setTerminate(RuntimeError *err) {
    if (err == NULL) return;
    if (this->terminate) delete this->terminate;
    this->terminate = err;
}

RuntimeError *TestCaseReport::getTerminate() {
    return this->terminate;
}
