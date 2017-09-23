//
// Created by lly on 22/09/2017.
//

#ifndef VPARSER_SCENARIOINFOOBJECT_H
#define VPARSER_SCENARIOINFOOBJECT_H


#include "../schema/BaseObject.h"
#include "../doc/DocElement.h"

#include <string>

using namespace std;

class ScenarioInfoObject: public BaseObject {
public:
    string title;
    string summary;
    string description;
    string version;

    ScenarioInfoObject();

    BaseDataObject *toDataObject() override;
};

struct ScenarioInfoObjectFactory {
    static ScenarioInfoObject *create(string filePath, DocElement *ele);
};


#endif //VPARSER_SCENARIOINFOOBJECT_H
