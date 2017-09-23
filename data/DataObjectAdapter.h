//
// Created by lly on 16/07/2017.
//

#ifndef VPARSER_DATAOBJECTADAPTER_H
#define VPARSER_DATAOBJECTADAPTER_H

#include "../doc/DocElement.h"
#include "../doc/DocScalarElement.h"
#include "BaseDataObject.h"
#include "IntegerDataObject.h"
#include "NumberDataObject.h"
#include "BooleanDataObject.h"
#include "StringDataObject.h"
#include "FileDataObject.h"
#include "CustomizedDataObject.h"

#include <string>
#include <iostream>
#include <sstream>

using namespace std;

/**
 * Adapter for DataObject classes and DocElement classes
 * Currently only for DataObject -> DocElement
 * TODO: DocElement -> DataObject
 */
class DataObjectAdapter {
public:
    static DocElement *toDocElement(BaseDataObject *origin, int level = 0);

    /**
     * Default version to transform DocElement to DataObject
     * It does NOT need schema info, BUT merely view all Scalar as String
     * @param ele source
     * @return result
     */
    static BaseDataObject *defaultFromDocElement(DocElement *ele);

    /**
     * Deep copy the origin
     * @param origin source
     * @return dest
     */
    static BaseDataObject *deepCopy(const BaseDataObject *origin);
};


#endif //VPARSER_DATAOBJECTADAPTER_H
