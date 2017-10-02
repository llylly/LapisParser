//
// Created by lly on 23/09/2017.
//

#ifndef VPARSER_MODULEINPUTCONSTRAINTOBJECT_H
#define VPARSER_MODULEINPUTCONSTRAINTOBJECT_H


#include "../../schema/DataSchemaObject.h"
#include "../../schema/Controller.h"
#include "../../error/InvalidExpressionError.h"

enum ModuleInputConstraintType {
    INVALID_INPUTCONSTRAINT, RANDOM_INPUTCONSTRAINT, FROMSET_INPUTCONSTRAINT
};

enum ModuleInputConstraintSetPolicy {
    INVALID_SETPOLICY, MEMBER_SETPOLICY, SIZE_SETPOLICY
};

enum ModuleInputConstraintNullPolicy {
    INVALID_NULLPOLICY, BREAK_NULLPOLICY, RANDOM_NULLPOLICY
};

class ModuleInputConstraintObject: public BaseObject {
public:
    ModuleInputConstraintObject();
    ~ModuleInputConstraintObject();

    string getKeyName();
    virtual BaseDataObject* toDataObject() override;

    /** 'fieldVec' contains 'in' as its first element **/
    vector<string> fieldVec;
    ModuleInputConstraintType type;
    /** 'from' is just a pointer to data schema, not owns it **/
    DataSchemaObject *from;
    string setName;
    ModuleInputConstraintSetPolicy setPolicy;
    string expression;
    ModuleInputConstraintNullPolicy nullPolicy;
};

struct ModuleInputConstraintObjectFactory {
    static ModuleInputConstraintObject *create(string filePath, string key, DocElement *ele, Controller *controller);

    static bool expressionValidate(string exp);
};


#endif //VPARSER_MODULEINPUTCONSTRAINTOBJECT_H
