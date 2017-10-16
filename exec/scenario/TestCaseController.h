//
// Created by lly on 08/10/2017.
//

#ifndef VPARSER_TESTCASECONTROLLER_H
#define VPARSER_TESTCASECONTROLLER_H

#include <cassert>
#include "../../config/ConfigObject.h"
#include "../report/TestCaseReport.h"
#include "../err/NowayTerminate.h"
#include "../err/CheckpointNotPassTerminate.h"
#include "../err/IllegalSetEffectError.h"
#include "../err/EndModuleTerminate.h"
#include "../request/ModuleRequester.h"
#include "../middleware/AliMiddleware.h"

class TestCaseController {
public:
    TestCaseController(Scenario *scenario, ConfigObject *config, string host, string basePath, int verbose = 0);
    TestCaseReport *run();

private:
    Scenario *scenario;
    ConfigObject *config;
    string host;
    string basePath;
    int verbose;

    string nowModule;
    map<string, vector<DocElement*>> nowSets;
    string nowState;
    string nowResponseType;
    RequesterReport *nowRequest;
    vector<string> moduleSeq;
    int nowRepeatTimes;

    TestCaseStepRecord *packetNow();
    bool checkConstraintsWith(string nowModule);

    static void setsCpy(map<string, vector<DocElement*>> &from, map<string, vector<DocElement*>> &to);
    static void vecDocEleCpy(vector<DocElement*> &from, vector<DocElement*> &to);
};


#endif //VPARSER_TESTCASECONTROLLER_H
