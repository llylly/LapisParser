//
// Created by lly on 08/10/2017.
//

#ifndef VPARSER_SCENARIOCONTROLLER_H
#define VPARSER_SCENARIOCONTROLLER_H

#include <cassert>
#include "TestCaseController.h"
#include "../../config/ConfigObject.h"
#include "../report/ScenarioReport.h"

class ScenarioController {
public:
    ScenarioController(ConfigObject *config, Scenarios *scenarios, ScenarioReport *report, string host, string basePath,
                       bool verbose=false, ostream &os=cout);
    ~ScenarioController();

    void run();

    bool verbose;
    ostream &os;

    ConfigObject *config;
    Scenario *scenario;
    ScenarioReport *report;
    string host;
    string basePath;
};


#endif //VPARSER_SCENARIOCONTROLLER_H
