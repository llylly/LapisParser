//
// Created by lly on 14/07/2017.
//

#ifndef VPARSER_PYTHONOBJECTADAPTER_H
#define VPARSER_PYTHONOBJECTADAPTER_H

#include "../../data/BaseDataObject.h"
#include "../../doc/DocElement.h"

extern "C" {
#include "python2.7/Python.h"
}

/**
 * Adapter for DataObject classes and PyObject class
 */
class PythonObjectAdapter {
public:
    /**
     * Translate DataObject to PyObject
     * CAUTION: for BYTEFILE or CUSTOMIZED data object, it will be a Dict: {type: "file"/"customized", data: *ByteArray*}
     * @param obj
     * @return
     */
    static PyObject *fromDataObject(BaseDataObject* obj);

    /**
     * Translate PyObject to DocElement
     * CAUTION: it will view {type: "file"/"customized", data: *ByteArray*} as a DocScalarElement
     * @param obj
     * @return
     */
    static BaseDataObject *toDocElement(PyObject *obj);

private:
    static bool isFile(PyObject *obj);

    static bool isCustomized(PyObject *obj);
};


#endif //VPARSER_PYTHONOBJECTADAPTER_H
