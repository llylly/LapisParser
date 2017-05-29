//
// Created by lly on 30/05/2017.
//

/**
 * Python Interface
 * Module Name: LapisParser
 */

#include "python2.7/Python.h"
#include "../error/Error.h"
#include "../doc/DocElement.h"
#include "../doc/YAMLAdapter.h"
#include "../doc/XMLAdapter.h"
#include "../ref_expand/RefExpand.h"
#include "../schema/Controller.h"

enum CURRENT_STATE {
    START, PARSED, ERROR
};

enum FORMAT {
    YAML, XML
};

CURRENT_STATE state;

bool readFiles(int filec, char** filev, FORMAT format) {
    //----- phase I -----
    for (int i=0; i<filec; ++i) {
        Error::curFileName = (std::string)(filev[i]);
        DocElement *curDoc = NULL;
        if (format == YAML) {
            curDoc = YAMLAdapter::parseDoc(filev[i]);
        }
        if (format == XML) {
            curDoc = XMLAdapter::parseDoc(filev[i]);
        }
        DocElement::docs[string(filev[i])] = curDoc;
    }
    if (Error::hasError()) {
        state = ERROR;
        return false;
    }

    //----- phase II -----
    RefExpand *refExpand = new RefExpand();
    refExpand->work();
    if (Error::hasError()) {
        state = ERROR;
        return false;
    }

    //----- phase III -----
    Controller *controller = new Controller();
    controller->work();
    if (Error::hasError()) {
        state = ERROR;
        return false;
    }

    state = PARSED;
    return true;
}

std::vector<Error*> *getErrors() {
    return Error::getErrors();
}

/**
 * Translate stored doc to XML or YAML
 * But only translate the first doc in map now
 * @param path
 * @param format
 * @return true: succeed, false: failed
 */
bool translate(char *path, FORMAT format) {
    if (state != PARSED) {
        return false;
    } else {
        DocElement *root = DocElement::docs.begin()->second;
        if (format == YAML) {
            if (YAMLAdapter::saveFile(path, root)) {
                state = ERROR;
                return false;
            }
        }
        if (format == XML) {
            if (XMLAdapter::saveFile(path, root)) {
                state = ERROR;
                return false;
            }
        }
    }
    return true;
}

/**
 * Read YAML
 * @param self
 * @param args, contains filePath list
 * @return 0 - success, 1 - failed
 */
PyObject* wrap_readYAML(PyObject* self, PyObject* args) {
    PyObject *pathList;
    if (!PyArg_ParseTuple(args, "O", &pathList))
        return NULL;
    if (!(PyList_CheckExact(pathList)))
        return NULL;
    Py_ssize_t len = PyList_Size(pathList);

    char **fileList = new char*[(int)len];
    for (Py_ssize_t i = 0; i < len; ++i) {
        PyObject *item = PyList_GetItem(pathList, i);
        if (!(PyString_CheckExact(item)))
            return NULL;
        Py_ssize_t nowlen = PyString_Size(item);
        fileList[(int)i] = new char[nowlen + 1];
        nowlen++;
        if (PyString_AsStringAndSize(item, &(fileList[(int)i]), &nowlen))
            return NULL;
    }

    bool res = readFiles(len, fileList, YAML);

    PyObject *ret = Py_BuildValue("i", !res);
    return ret;
}

/**
 * Read XML
 * @param self
 * @param args, contains filePath list
 * @return 0 - success, 1 - failed
 */
PyObject* wrap_readXML(PyObject* self, PyObject* args) {
    PyObject *pathList;
    if (!PyArg_ParseTuple(args, "O", &pathList))
        return NULL;
    if (!(PyList_CheckExact(pathList)))
        return NULL;
    Py_ssize_t len = PyList_Size(pathList);

    char **fileList = new char*[(int)len];
    for (Py_ssize_t i = 0; i < len; ++i) {
        PyObject *item = PyList_GetItem(pathList, i);
        if (!(PyString_CheckExact(item)))
            return NULL;
        Py_ssize_t nowlen = PyString_Size(item);
        fileList[(int)i] = new char[nowlen + 1];
        nowlen++;
        if (PyString_AsStringAndSize(item, &(fileList[(int)i]), &nowlen))
            return NULL;
    }

    bool res = readFiles(len, fileList, YAML);

    PyObject *ret = Py_BuildValue("i", !res);
    return ret;
}

/**
 * Get parse errors
 * @param self
 * @param args, nothing needed
 * @return list, each item is a dict: {fileName: string, line: integer, col: integer, msg: string}
 */
PyObject* wrap_getErrors(PyObject* self, PyObject* args) {
    std::vector<Error*> *errorsVec = getErrors();
    PyObject *res = PyList_New(errorsVec->size());
    for (int i=0; i<errorsVec->size(); ++i) {
        Error *now = (*errorsVec)[i];
        PyObject *nowObj = PyDict_New();
        PyDict_SetItemString(nowObj, "fileName", Py_BuildValue("s", now->fileName.c_str()));
        PyDict_SetItemString(nowObj, "line", Py_BuildValue("i", now->line));
        PyDict_SetItemString(nowObj, "col", Py_BuildValue("i", now->col));
        PyDict_SetItemString(nowObj, "msg", Py_BuildValue("s", now->msg.c_str()));
        PyList_SetItem(res, (Py_ssize_t)i, nowObj);
    }
    return res;
}

/**
 * Translate internal doc to a YAML file
 * @param self
 * @param args, contains a filePath to be written to
 * @return 0 - success, 1 - failed
 */
PyObject* wrap_translate2YAML(PyObject* self, PyObject* args) {
    char *filePath;
    if (!PyArg_ParseTuple(args, "s", &filePath))
        return NULL;

    bool res = translate(filePath, YAML);

    PyObject *ret = Py_BuildValue("i", !res);
    return ret;
}

/**
 * Translate internal doc to a XML file
 * @param self
 * @param args, contains a filePath to be written to
 * @return 0 - success, 1 - failed
 */
PyObject* wrap_translate2XML(PyObject* self, PyObject* args) {
    char *filePath;
    if (!PyArg_ParseTuple(args, "s", &filePath))
        return NULL;

    bool res = translate(filePath, XML);

    PyObject *ret = Py_BuildValue("i", !res);
    return ret;
}

static PyMethodDef LapisParserMethods[] = {
        {"readYAML", wrap_readYAML, METH_VARARGS, "Read YAML format files."},
        {"readXML", wrap_readXML, METH_VARARGS, "Read XML format files."},
        {"getErrors", wrap_getErrors, METH_VARARGS, "Get all parse errors."},
        {"translate2YAML", wrap_translate2YAML, METH_VARARGS, "Translate parsed doc to YAML."},
        {"translate2XML", wrap_translate2XML, METH_VARARGS, "Translate parsed doc to XML."},
        {NULL, NULL}
};

PyMODINIT_FUNC initLapisParser() {
    state = START;
    Py_InitModule("LapisParser", LapisParserMethods);
}