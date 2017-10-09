//
// Created by lly on 23/09/2017.
//

#ifndef VPARSER_MODULESETEFFECTOBJECT_H
#define VPARSER_MODULESETEFFECTOBJECT_H


#include "../../schema/BaseObject.h"
#include "../../doc/DocElement.h"
#include "../../schema/APIObject.h"
#include "../../schema/data_schema/ObjectSchema.h"
#include "../../schema/data_schema/ArraySchema.h"
#include <string>
#include <vector>

using namespace std;

enum ModuleSetEffectType {
    INVALID_SETEFFECT, ADD_SETEFFECT, DELETE_SETEFFECT
};

class ModuleSetEffectObject: public BaseObject {
public:
    ModuleSetEffectObject();
    ~ModuleSetEffectObject();

    virtual BaseDataObject *toDataObject() override;

    /** TODO **/
    /** Check whether the element should be deleted according to the condition, input data and output data **/
    static bool needDelete(string condition, map<string, BaseDataObject*> &in, BaseDataObject *out, DocElement *now);

    /** TODO **/
    /** Check the correctness of condition expression **/
    static bool conditionCheck(string condition, APIObject *api);

    /** Check the validity of dot separated in.{member} expression **/
    static bool inFieldCheck(const vector<string> &inObj, APIObject *api);

    /** Check the correctness of dot separated out.{member} expression **/
    static bool outFieldCheck(const vector<string> &outObj);

    ModuleSetEffectType type;
    vector<string> object;
    /** setName is 'set' field **/
    string setName;
    string condition;
    vector<string> excluding;
    vector<string> including;
};

struct ModuleSetEffectObjectFactory {
    static ModuleSetEffectObject *create(string filePath, DocElement *ele, APIObject *api);
};

#endif //VPARSER_MODULESETEFFECTOBJECT_H
