//
// Created by lly on 28/05/2017.
//

#ifndef VPARSER_BASEOBJECT_H
#define VPARSER_BASEOBJECT_H


struct BaseDataObject;

class BaseObject {
public:
    virtual BaseDataObject* toDataObject() = 0;
};


#endif //VPARSER_BASEOBJECT_H
