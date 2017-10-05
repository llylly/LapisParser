//
// Created by lly on 03/10/2017.
//

#ifndef VPARSER_CONFIGOBJECT_H
#define VPARSER_CONFIGOBJECT_H


#include "../schema/BaseObject.h"
#include "../scenario/Scenarios.h"
#include "../error/DuplicateTestConfigError.h"
#include "../error/InvalidScenarioError.h"
#include "../error/InvalidModuleError.h"
#include <string>

using namespace std;

enum ConfigTraversePolicy {
    INVALID_TRAVERSE_POLICY, RANDOM_TRAVERSE_POLICY,
    BASIC_EXPLORE_TRAVERSE_POLICY, ADVANCE_EXPLORE_TRAVERSE_POLICY
};

class ConfigObject: public BaseObject {
public:
    ConfigObject();
    ~ConfigObject();
    BaseDataObject *toDataObject() override;
    bool work(Controller *controller, Scenarios *scenarios);

    string scenario;
    map<string, vector<DocElement*>> setInitial;
    ConfigTraversePolicy policy;
    int maxCase;
    vector<string> vitalModules;
    int maxLength;
    int timeout;

    bool isAli;
    string aliKeySecret;
};


#endif //VPARSER_CONFIGOBJECT_H
