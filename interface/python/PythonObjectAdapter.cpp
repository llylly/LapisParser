//
// Created by lly on 14/07/2017.
//

#include "PythonObjectAdapter.h"
#include "../../data/IntegerDataObject.h"
#include "../../data/NumberDataObject.h"
#include "../../data/BooleanDataObject.h"
#include "../../data/StringDataObject.h"
#include "../../data/FileDataObject.h"
#include "../../data/CustomizedDataObject.h"
#include "../../data/SequenceDataObject.h"
#include "../../data/ObjectDataObject.h"
#include "../../data/DataObjectAdapter.h"

PyObject *PythonObjectAdapter::fromDataObject(BaseDataObject *obj) {
    if (obj == NULL)
        return Py_None;
    if (obj->type == BASE) {
        return Py_None;
    }
    if (obj->type == INTEGER) {
        IntegerDataObject *o = (IntegerDataObject*)obj;
        return PyLong_FromLongLong(o->value);
    }
    if (obj->type == NUMBER) {
        NumberDataObject *o = (NumberDataObject*)obj;
        return PyFloat_FromDouble(o->value);
    }
    if (obj->type == BOOLEAN) {
        BooleanDataObject *o = (BooleanDataObject*)obj;
        return PyBool_FromLong(o->value);
    }
    if (obj->type == STRING) {
        StringDataObject *o = (StringDataObject*)obj;
        return PyString_FromString(o->str.c_str());
    }
    if (obj->type == BYTEFILE) {
        FileDataObject *o = (FileDataObject*)obj;
        PyObject *dict = PyDict_New();
        PyDict_SetItemString(dict, "type", PyString_FromString("file"));
        PyDict_SetItemString(dict, "data", PyByteArray_FromStringAndSize((char*)o->data, o->len));
        return dict;
    }
    if (obj->type == CUSTOMIZED) {
        CustomizedDataObject *o = (CustomizedDataObject*)obj;
        PyObject *dict = PyDict_New();
        PyDict_SetItemString(dict, "type", PyString_FromString("customized"));
        PyDict_SetItemString(dict, "data", PyByteArray_FromStringAndSize((char*)o->data, o->len));
        return dict;
    }
    if (obj->type == SEQUENCE) {
        SequenceDataObject *o = (SequenceDataObject*)obj;
        int len = o->length();
        PyObject *seq = PyList_New((Py_ssize_t)len);
        for (int i=0; i<len; ++i) {
            PyObject *now = PythonObjectAdapter::fromDataObject((*o)[i]);
            PyList_SetItem(seq, (Py_ssize_t)i, now);
        }
        return seq;
    }
    if (obj->type == OBJECT) {
        ObjectDataObject *o = (ObjectDataObject*)obj;
        PyObject *dict = PyDict_New();
        for (map<string, BaseDataObject*>::iterator ite = o->m.begin();
                ite != o->m.end(); ++ite) {
            PyObject *now = PythonObjectAdapter::fromDataObject(ite->second);
            PyDict_SetItemString(dict, ite->first.c_str(), now);
        }
        return dict;
    }
    return Py_None;
}

BaseDataObject *PythonObjectAdapter::toDataObject(PyObject *obj) {
    if (PyInt_Check(obj)) {
        // to IntegerDataObject
        return new IntegerDataObject(PyInt_AsLong(obj));
    }
    if (PyLong_Check(obj)) {
        // to IntegerDataObject
        return new IntegerDataObject(PyLong_AsLongLong(obj));
    }
    if (PyFloat_Check(obj)) {
        // to NumberDataObject
        return new NumberDataObject(PyFloat_AsDouble(obj));
    }
    if (PyBool_Check(obj)) {
        // to BooleanDataObject
        if (Py_True == obj)
            return new BooleanDataObject(true);
        else
            return new BooleanDataObject(false);
    }
    if (PyString_Check(obj)) {
        // to StringDataObject
        return new StringDataObject(PyString_AsString(obj));
    }
    if (PyList_Check(obj)) {
        // to SequenceDataObject
        SequenceDataObject *seq = new SequenceDataObject();
        int len = (int)PyList_Size(obj);
        for (int i=0; i<len; ++i) {
            seq->push(PythonObjectAdapter::toDataObject(PyList_GetItem(obj, (Py_ssize_t)i)));
        }
        return seq;
    }
    if (PyDict_Check(obj)) {
        // to DocScalarElement if represents Customized for File
        if (PythonObjectAdapter::isFile(obj)) {
            PyObject *data = PyDict_GetItemString(obj, "data");
            FileDataObject *fi = new FileDataObject((unsigned char*)PyByteArray_AsString(data), (int)PyByteArray_Size(data));
            return fi;
        }
        if (PythonObjectAdapter::isCustomized(obj)) {
            PyObject *data = PyDict_GetItemString(obj, "data");
            CustomizedDataObject *cus = new CustomizedDataObject((unsigned char*)PyByteArray_AsString(data), (int)PyByteArray_Size(data));
            return cus;
        }
        // to DocObjectElement if others
        ObjectDataObject *o = new ObjectDataObject();
        PyObject *keyList = PyDict_Keys(obj);
        int size = (int)PyList_Size(keyList);
        if (size > 0) {
            for (int i=0; i<size; ++i) {
                PyObject *nowKey = PyList_GetItem(keyList, (Py_ssize_t)i);
                if ((nowKey) && (PyString_Check(nowKey))) {
                    (*o)[string(PyString_AsString(nowKey))] = PythonObjectAdapter::toDataObject(PyDict_GetItem(obj, nowKey));
                }
            }
        }
        return o;
    }
    return NULL;
}

bool PythonObjectAdapter::isFile(PyObject *obj) {
    if (PyDict_Check(obj)) {
        int size = (int)PyDict_Size(obj);
        if (size == 2) {
            PyObject *type = PyDict_GetItemString(obj, "type");
            PyObject *data = PyDict_GetItemString(obj, "data");
            if ((type) && (PyString_Check(type)))
                if (strcmp(PyString_AsString(type), "file") == 0)
                    if ((data) && (PyByteArray_Check(data)))
                        return true;
        }
    }
    return false;
}

bool PythonObjectAdapter::isCustomized(PyObject *obj) {
    if (PyDict_Check(obj)) {
        int size = (int)PyDict_Size(obj);
        if (size == 2) {
            PyObject *type = PyDict_GetItemString(obj, "type");
            PyObject *data = PyDict_GetItemString(obj, "data");
            if ((type) && (PyString_Check(type)))
                if (strcmp(PyString_AsString(type), "customized"))
                    if ((data) && (PyByteArray_Check(data)))
                        return true;
        }
    }
    return false;
}