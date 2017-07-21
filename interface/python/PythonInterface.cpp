//
// Created by lly on 30/05/2017.
//

/**
 * Python Interface
 * Module Name: LapisParser
 */

#include "PythonInterface.h"

CURRENT_STATE state;

Controller *controller;

bool readFiles(int filec, char** filev, FORMAT format) {
    //----- Clean all -----
    for (map<string, DocElement*>::iterator ite = DocElement::docs.begin(); ite != DocElement::docs.end(); ++ite)
        delete ite->second;
    DocElement::docs.clear();

    std::vector<Error*> *errs = Error::getErrors();
    for (std::vector<Error*>::iterator ite = errs->begin(); ite != errs->end(); ++ite)
        delete *ite;
    errs->clear();


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
    controller = new Controller();
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


PyObject* wrap_readYAML(PyObject* self, PyObject* args) {
    PyObject *pathList;
    if (!PyArg_ParseTuple(args, "O", &pathList))
        return Py_None;
    if (!(PyList_CheckExact(pathList)))
        return Py_None;
    Py_ssize_t len = PyList_Size(pathList);

    char **fileList = new char*[(int)len];
    for (Py_ssize_t i = 0; i < len; ++i) {
        PyObject *item = PyList_GetItem(pathList, i);
        if (!(PyString_CheckExact(item)))
            return Py_None;
        Py_ssize_t nowlen = PyString_Size(item);
        fileList[(int)i] = new char[nowlen + 1];
        nowlen++;
        if (PyString_AsStringAndSize(item, &(fileList[(int)i]), &nowlen))
            return Py_None;
    }

    bool res = readFiles(len, fileList, YAML);

    PyObject *ret = Py_BuildValue("i", !res);
    return ret;
}

PyObject* wrap_readXML(PyObject* self, PyObject* args) {
    PyObject *pathList;
    if (!PyArg_ParseTuple(args, "O", &pathList))
        return Py_None;
    if (!(PyList_CheckExact(pathList)))
        return Py_None;
    Py_ssize_t len = PyList_Size(pathList);

    char **fileList = new char*[(int)len];
    for (Py_ssize_t i = 0; i < len; ++i) {
        PyObject *item = PyList_GetItem(pathList, i);
        if (!(PyString_CheckExact(item)))
            return Py_None;
        Py_ssize_t nowlen = PyString_Size(item);
        fileList[(int)i] = new char[nowlen + 1];
        nowlen++;
        if (PyString_AsStringAndSize(item, &(fileList[(int)i]), &nowlen))
            return Py_None;
    }

    bool res = readFiles(len, fileList, XML);

    PyObject *ret = Py_BuildValue("i", !res);
    return ret;
}

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

PyObject* wrap_translate2YAML(PyObject* self, PyObject* args) {
    char *filePath;
    if (!PyArg_ParseTuple(args, "s", &filePath))
        return Py_None;

    bool res = translate(filePath, YAML);

    PyObject *ret = Py_BuildValue("i", !res);
    return ret;
}

PyObject* wrap_translate2XML(PyObject* self, PyObject* args) {
    char *filePath;
    if (!PyArg_ParseTuple(args, "s", &filePath))
        return Py_None;

    bool res = translate(filePath, XML);

    PyObject *ret = Py_BuildValue("i", !res);
    return ret;
}

PyObject *wrap_getInfo(PyObject *self, PyObject *args) {
    if (state != PARSED)
        return NULL;
    BaseDataObject *dataObject = controller->infoObject->toDataObject();
    PyObject *py = PythonObjectAdapter::fromDataObject(dataObject);
    delete dataObject;
    return py;
}

PyObject *wrap_getHost(PyObject *self, PyObject *args) {
    if (state != PARSED)
        return Py_None;
    return PyString_FromString(controller->host.c_str());
}

PyObject *wrap_getBasePath(PyObject *self, PyObject *args) {
    if (state != PARSED)
        return Py_None;
    return PyString_FromString(controller->basePath.c_str());
}

PyObject *wrap_getSchemes(PyObject *self, PyObject *args) {
    if (state != PARSED)
        return Py_None;
    if (controller->schemes == NULL)
        return Py_None;
    BaseDataObject *dataObject = controller->schemes->toDataObject();
    PyObject *py = PythonObjectAdapter::fromDataObject(dataObject);
    delete dataObject;
    return py;
}

PyObject *wrap_getConsumes(PyObject *self, PyObject *args) {
    if (state != PARSED)
        return Py_None;
    if (controller->consumes == NULL)
        return Py_None;
    BaseDataObject *dataObject = controller->consumes->toDataObject();
    PyObject *py = PythonObjectAdapter::fromDataObject(dataObject);
    delete dataObject;
    return py;
}

PyObject *wrap_getProduces(PyObject *self, PyObject *args) {
    if (state != PARSED)
        return Py_None;
    if (controller->produces == NULL)
        return Py_None;
    BaseDataObject *dataObject = controller->produces->toDataObject();
    PyObject *py = PythonObjectAdapter::fromDataObject(dataObject);
    delete dataObject;
    return py;
}

PyObject *wrap_getTags(PyObject *self, PyObject *args) {
    if (state != PARSED)
        return Py_None;
    if (controller->tags == NULL)
        return Py_None;
    BaseDataObject *dataObject = controller->tags->toDataObject();
    PyObject *py = PythonObjectAdapter::fromDataObject(dataObject);
    delete dataObject;
    return py;
}

PyObject *wrap_getExternalDocs(PyObject *self, PyObject *args) {
    if (state != PARSED)
        return Py_None;
    if (controller->externalDocs == NULL)
        return Py_None;
    BaseDataObject *dataObject = controller->externalDocs->toDataObject();
    PyObject *py = PythonObjectAdapter::fromDataObject(dataObject);
    delete dataObject;
    return py;
}

static PyMethodDef LapisParserMethods[] = {
        {"readYAML", wrap_readYAML, METH_VARARGS, "Read YAML format files."},
        {"readXML", wrap_readXML, METH_VARARGS, "Read XML format files."},
        {"getErrors", wrap_getErrors, METH_VARARGS, "Get all parse errors."},
        {"translate2YAML", wrap_translate2YAML, METH_VARARGS, "Translate parsed doc to YAML."},
        {"translate2XML", wrap_translate2XML, METH_VARARGS, "Translate parsed doc to XML."},
        {"getInfo", wrap_getInfo, METH_VARARGS, "Get the Info Object."},
        {"getHost", wrap_getHost, METH_VARARGS, "Get the host url"},
        {"getBasePath", wrap_getBasePath, METH_VARARGS, "Get the basePath"},
        {"getSchemes", wrap_getSchemes, METH_VARARGS, "Get the schemes list"},
        {"getConsumes", wrap_getConsumes, METH_VARARGS, "Get the consumes list"},
        {"getProduces", wrap_getProduces, METH_VARARGS, "Get the produces list"},
        {"getTags", wrap_getTags, METH_VARARGS, "Get the tags list"},
        {"getExternalDocs", wrap_getExternalDocs, METH_VARARGS, "Get the externalDocs Object"},
        {NULL, NULL}
};

PyMODINIT_FUNC initLapisParser() {
    state = START;
    Py_InitModule("LapisParser", LapisParserMethods);
}