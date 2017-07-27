//
// Created by lly on 26/07/2017.
//

#ifndef VPARSER_APICONSTRAINTOBJECT_H
#define VPARSER_APICONSTRAINTOBJECT_H


#include "../BaseObject.h"
#include "../APIPool.h"

enum APIConstraintType {
    BEFORE, AFTER
};

class APIConstraintObject: public BaseObject {
public:

    /**
     * Constructor
     * @param pool: the pool to parse guest APIObject
     */
    APIConstraintObject(APIPool *pool);

    /**
     * Output interface
     * @return
     */
    BaseDataObject* toDataObject() override;


    APIPool *pool;
    APIObject *guest;
    string guestName;
    APIRequestMethod guestMethod;

    int minInterval;
    /** -1 means infinity **/
    int maxInterval;
    int minTimes;
    /** -1 means infinity **/
    int maxTimes;

};


#endif //VPARSER_APICONSTRAINTOBJECT_H
