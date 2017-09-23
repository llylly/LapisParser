//
// Created by lly on 06/08/2017.
//

#ifndef VPARSER_SCENARIOS_H
#define VPARSER_SCENARIOS_H

#include "../schema/Controller.h"
#include "Scenario.h"

/**
 * This class serves as the sub controller for scenarios
 * Meanwhile it's also a pool of scenarios
 */
class Scenarios {

public:
    Scenarios();
    ~Scenarios();
    bool work(Controller *controller);

    const set<string> &getTitleSet();
    Scenario *getScenarioByTitle(string title);

private:
    set<Scenario*> scenarioSet;
    map<DocObjectElement*, Scenario*> pool;
    set<string> titleSet;
    map<string, Scenario*> titleMap;

};


#endif //VPARSER_SCENARIOS_H
