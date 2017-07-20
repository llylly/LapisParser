//
// Created by lly on 12/07/2017.
//

#ifndef VPARSER_BASEDATASCHEMAOBJECT_H
#define VPARSER_BASEDATASCHEMAOBJECT_H

#include <cstdio>

enum DATA_OBJECT_TYPE {BASE, NUMBER, INTEGER, BOOLEAN, STRING, BYTEFILE, SEQUENCE, OBJECT, CUSTOMIZED};

class DataSchemaObject;

struct BaseDataObject {
    /**
     * Base class for data object
     */

    bool isTop;
    DataSchemaObject *schema;
        // only top data object can have valid schema member
    DATA_OBJECT_TYPE type;

    /**
     * Return true for equivalent real value
     * @param b
     * @return equal or not
     */
    virtual bool equals(BaseDataObject *b);

    virtual ~BaseDataObject();

protected:
    BaseDataObject();
    BaseDataObject(const BaseDataObject &source);
};


#endif //VPARSER_BASEDATASCHEMAOBJECT_H
