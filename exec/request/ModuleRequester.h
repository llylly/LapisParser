//
// Created by lly on 06/10/2017.
//

#ifndef VPARSER_MODULEREQUESTER_H
#define VPARSER_MODULEREQUESTER_H

#include <map>
#include "BaseRequester.h"
#include "../../scenario/ScenarioModuleObject.h"
#include "../err/IllegalInputConstraintError.h"
#include "../err/SetEmptyTerminate.h"

class ModuleRequester: public BaseRequester {
public:
    ModuleRequester(
            string host,
            string basePath,
            RequesterReport *report,
            map<string, string> *(*middleware_func)(map<string, string>*, void*) = NULL,
            int timeout = 2000);

    map<string, BaseDataObject*> *dataGen() override;

    bool init(ScenarioModuleObject *module,
              map<string, vector<DocElement*>> *sets);

private:
    ScenarioModuleObject *module;
    map<string, vector<DocElement *>> *sets;
};


#endif //VPARSER_MODULEREQUESTER_H
