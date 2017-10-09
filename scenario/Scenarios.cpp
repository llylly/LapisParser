//
// Created by lly on 06/08/2017.
//

#include "Scenarios.h"
#include "../error/DuplicateScenarioNamesError.h"
#include <iostream>

using namespace std;

Scenarios::Scenarios() {
    this->scenarioSet.clear();
    this->pool.clear();
    this->titleSet.clear();
    this->titleMap.clear();
}

Scenarios::~Scenarios() {
    for (set<Scenario*>::iterator ite = this->scenarioSet.begin();
            ite != this->scenarioSet.end();
            ++ite) {
        delete *ite;
    }
    this->scenarioSet.clear();
    this->pool.clear();
    this->titleSet.clear();
    this->titleMap.clear();
}

bool Scenarios::work(Controller *controller) {
    for (map<string, DocElement*>::iterator ite=DocElement::docs.begin(); ite != DocElement::docs.end(); ++ite) {
        DocElement *priDocRoot = ite->second;
        if (priDocRoot->type == DOC_OBJECT) {
            DocObjectElement *docRoot = (DocObjectElement*)priDocRoot;
            DocElement *priScenariosField = docRoot->get("x-scenario");
            if (priScenariosField != NULL) {
                if (priScenariosField->type != DOC_SEQUENCE) {
                    Error::addError(
                            new FieldInvalidError(ite->first, priScenariosField->line, priScenariosField->col, "x-scenario",
                                                  priScenariosField->type, DOC_SEQUENCE)
                    );
                    return false;
                }
                DocSequenceElement *scenariosField = (DocSequenceElement*)priScenariosField;
                int len = scenariosField->getLength();
                for (int i=0; i<len; ++i) {
                    DocElement *nowScenarioEle = scenariosField->get(i);
                    if (nowScenarioEle->type != DOC_OBJECT) {
                        Error::addError(
                                new FieldInvalidError(ite->first, nowScenarioEle->line, nowScenarioEle->col, "x-scenario",
                                                      nowScenarioEle->type, DOC_OBJECT)
                        );
                        return false;
                    }
                    DocObjectElement *scenarioField = (DocObjectElement*)nowScenarioEle;
                    if (this->pool.count(scenarioField) > 0)
                        continue;
                    Scenario *now = ScenarioFactory::create(ite->first, scenarioField, controller);
                    if (now == NULL)
                        return false;
                    if (this->titleSet.count(now->info->title) > 0) {
                        Error::addError(
                                new DuplicateScenarioNamesError(ite->first, scenarioField->line, scenarioField->col, "x-scenario.info.title", now->info->title)
                        );
                        return false;
                    }
                    this->scenarioSet.insert(now);
                    this->pool[scenarioField] = now;
                    this->titleSet.insert(now->info->title);
                    this->titleMap[now->info->title] = now;
                }
            }
        }
    }
    return true;
}

const set<string> &Scenarios::getTitleSet() {
    return this->titleSet;
}

Scenario *Scenarios::getScenarioByTitle(string title) {
    if (this->titleMap.count(title) > 0)
        return this->titleMap[title];
    else
        return NULL;
}