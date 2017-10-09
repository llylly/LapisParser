//
// Created by lly on 08/10/2017.
//

#include "ScenarioController.h"

ScenarioController::ScenarioController(ConfigObject *config, Scenarios *scenarios, ScenarioReport *report, string host, string baseaPath,
                                       bool verbose, ostream &os): os(os) {
    assert(config != NULL);
    assert(scenarios != NULL);
    assert(report != NULL);

    this->verbose = verbose;

    this->scenario = scenarios->getScenarioByTitle(config->scenario);
    this->config = config;
    this->report = report;
    this->host = host;
    this->basePath = basePath;
}

ScenarioController::~ScenarioController() { }

void ScenarioController::run() {
    this->report->start(this->scenario->info->title);
    if (this->verbose) {
        os << "Scenario \"" << this->scenario->info->title << "\" begin" << endl;
        os.flush();
    }
    for (int nowCase = 0; nowCase < this->config->maxCase; ++nowCase) {
        TestCaseController *testCase = new TestCaseController(this->scenario, config, host, basePath);
        TestCaseReport *nowRep = testCase->run();
        if (nowRep != NULL)
            report->addTestCase(nowRep);
        if (this->verbose) {
            os << "Case #" << nowCase << endl;
            os.flush();
        }
    }
    this->report->end();
    if (this->verbose) {
        os << "Scenario end" << endl;
        os.flush();
    }
}
