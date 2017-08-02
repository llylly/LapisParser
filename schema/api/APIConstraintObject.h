//
// Created by lly on 26/07/2017.
//

#ifndef VPARSER_APICONSTRAINTOBJECT_H
#define VPARSER_APICONSTRAINTOBJECT_H


#include "../BaseObject.h"
#include "../../error/IllegalIntervalError.h"
#include "../../doc/DocElement.h"
#include "../../doc/DocObjectElement.h"

enum APIConstraintStage {
    INVALID_CONS, BAKED_CONS, VALID_CONS
};

enum APIConstraintType {
    BEFORE, AFTER
};

enum APIRequestMethod {
    GET, POST
};

class APIPool;

class APIObject;

class APIConstraintObject: public BaseObject {
public:

    /**
     * Constructor
     */
    APIConstraintObject();

    /**
     * Create it from DocElement
     * INVALID -> BAKED
     * @param filePath: parse file path
     * @param ele: the doc node
     * @return whether successful or not
     */
    bool create(string filePath, DocElement *ele);

    /**
     * Find the guestName & guestMethod by APIObject* from the pool
     * BAKED -> VALID
     * @param pool: the pool
     * @return whether found or not
     */
    bool findGuest(APIPool *pool);

    /**
     * Output interface
     * @return
     */
    BaseDataObject* toDataObject() override;


    APIConstraintStage stage;

    DocObjectElement *guestDoc;
    APIObject *guest;
    string guestName;
    APIRequestMethod guestMethod;

    APIConstraintType type;
    int minInterval;
    /** -1 means infinity **/
    int maxInterval;
    int minTimes;
    /** -1 means infinity **/
    int maxTimes;

};


#endif //VPARSER_APICONSTRAINTOBJECT_H
