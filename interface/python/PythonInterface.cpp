//
// Created by lly on 30/05/2017.
//

/**
 * Python Interface
 * Module Name: LapisParser
 */

#include "PythonInterface.h"

CURRENT_STATE state;

Controller *controller = NULL;

bool readFiles(int filec, char **filev, FORMAT *format) {
    //----- Clean all -----
    for (map<string, DocElement *>::iterator ite = DocElement::docs.begin(); ite != DocElement::docs.end(); ++ite)
        delete ite->second;
    DocElement::docs.clear();

    std::vector<Error *> *errs = Error::getErrors();
    for (std::vector<Error *>::iterator ite = errs->begin(); ite != errs->end(); ++ite)
        delete *ite;
    errs->clear();


    //----- phase I -----
    for (int i = 0; i < filec; ++i) {
        Error::curFileName = (std::string) (filev[i]);
        DocElement *curDoc = NULL;
        if (format[i] == YAML) {
            curDoc = YAMLAdapter::parseDoc(filev[i]);
        }
        if (format[i] == XML) {
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
    if (controller) delete controller;
    controller = new Controller();
    controller->work();
    if (Error::hasError()) {
        state = ERROR;
        return false;
    }

    state = PARSED;
    return true;
}

std::vector<Error *> *getErrors() {
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

PyObject *wrap_read(PyObject *self, PyObject *args) {
    PyObject *pathList;
    if (!PyArg_ParseTuple(args, "O", &pathList))
        return Py_None;
    if (!(PyList_CheckExact(pathList)))
        return Py_None;
    Py_ssize_t len = PyList_Size(pathList);

    char **fileList = new char *[(int) len];
    for (Py_ssize_t i = 0; i < len; ++i) {
        PyObject *item = PyList_GetItem(pathList, i);
        if (!(PyString_CheckExact(item)))
            return Py_None;
        Py_ssize_t nowlen = PyString_Size(item);
        fileList[(int) i] = new char[nowlen + 1];
        nowlen++;
        if (PyString_AsStringAndSize(item, &(fileList[(int) i]), &nowlen))
            return Py_None;
    }

    FORMAT *farr = new FORMAT[len];
    for (int i = 0; i < len; ++i) {
        char *now = fileList[i];
        char *endNow = now + strlen(now) - 3;
        // if filePath is ended with "xml", parse as XML file. Otherwise parse as YAML file.
        if (strcmp(endNow, "xml") == 0) {
            farr[i] = XML;
        } else {
            farr[i] = YAML;
        }
    }
    bool res = readFiles((int) len, fileList, farr);
    delete[] farr;

    PyObject *ret = Py_BuildValue("i", !res);
    return ret;
}


PyObject *wrap_readYAML(PyObject *self, PyObject *args) {
    PyObject *pathList;
    if (!PyArg_ParseTuple(args, "O", &pathList))
        return Py_None;
    if (!(PyList_CheckExact(pathList)))
        return Py_None;
    Py_ssize_t len = PyList_Size(pathList);

    char **fileList = new char *[(int) len];
    for (Py_ssize_t i = 0; i < len; ++i) {
        PyObject *item = PyList_GetItem(pathList, i);
        if (!(PyString_CheckExact(item)))
            return Py_None;
        Py_ssize_t nowlen = PyString_Size(item);
        fileList[(int) i] = new char[nowlen + 1];
        nowlen++;
        if (PyString_AsStringAndSize(item, &(fileList[(int) i]), &nowlen))
            return Py_None;
    }

    FORMAT *farr = new FORMAT[len];
    for (int i = 0; i < len; ++i)
        farr[i] = YAML;
    bool res = readFiles((int) len, fileList, farr);
    delete[] farr;

    PyObject *ret = Py_BuildValue("i", !res);
    return ret;
}

PyObject *wrap_readXML(PyObject *self, PyObject *args) {
    PyObject *pathList;
    if (!PyArg_ParseTuple(args, "O", &pathList))
        return Py_None;
    if (!(PyList_CheckExact(pathList)))
        return Py_None;
    Py_ssize_t len = PyList_Size(pathList);

    char **fileList = new char *[(int) len];
    for (Py_ssize_t i = 0; i < len; ++i) {
        PyObject *item = PyList_GetItem(pathList, i);
        if (!(PyString_CheckExact(item)))
            return Py_None;
        Py_ssize_t nowlen = PyString_Size(item);
        fileList[(int) i] = new char[nowlen + 1];
        nowlen++;
        if (PyString_AsStringAndSize(item, &(fileList[(int) i]), &nowlen))
            return Py_None;
    }

    FORMAT *farr = new FORMAT[len];
    for (int i = 0; i < len; ++i)
        farr[i] = XML;
    bool res = readFiles((int) len, fileList, farr);
    delete[] farr;

    PyObject *ret = Py_BuildValue("i", !res);
    return ret;
}

PyObject *wrap_getErrors(PyObject *self, PyObject *args) {
    std::vector<Error *> *errorsVec = getErrors();
    PyObject *res = PyList_New(errorsVec->size());
    for (int i = 0; i < errorsVec->size(); ++i) {
        Error *now = (*errorsVec)[i];
        PyObject *nowObj = PyDict_New();
        PyDict_SetItemString(nowObj, "fileName", Py_BuildValue("s", now->fileName.c_str()));
        PyDict_SetItemString(nowObj, "line", Py_BuildValue("i", now->line));
        PyDict_SetItemString(nowObj, "col", Py_BuildValue("i", now->col));
        PyDict_SetItemString(nowObj, "msg", Py_BuildValue("s", now->msg.c_str()));
        PyList_SetItem(res, (Py_ssize_t) i, nowObj);
    }
    return res;
}

PyObject *wrap_translate2YAML(PyObject *self, PyObject *args) {
    char *filePath;
    if (!PyArg_ParseTuple(args, "s", &filePath))
        return Py_None;

    bool res = translate(filePath, YAML);

    PyObject *ret = Py_BuildValue("i", !res);
    return ret;
}

PyObject *wrap_translate2XML(PyObject *self, PyObject *args) {
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

PyObject *wrap_getDataSchemaNames(PyObject *self, PyObject *args) {
    if (state != PARSED)
        return Py_None;
    const map<string, DocElement *> &nameMap = controller->definitions->getNameMap();
    PyObject *l = PyList_New(nameMap.size());
    int i = 0;
    for (map<string, DocElement *>::const_iterator ite = nameMap.cbegin();
         ite != nameMap.cend();
         ++ite) {
        PyList_SetItem(l, (Py_ssize_t) i, Py_BuildValue("s", ite->first.c_str()));
        ++i;
    }
    return l;
}

PyObject *wrap_getDataSchemaByName(PyObject *self, PyObject *args) {
    if (state != PARSED)
        return Py_None;
    char *nameObj;
    if (!PyArg_ParseTuple(args, "s", &nameObj))
        return Py_None;
    string name = string(nameObj);
    DataSchemaObject *o = controller->definitions->getSchemaByName(name);
    if (o == NULL)
        return Py_None;
    BaseDataObject *ele = o->toDataObject();
    if (ele == NULL)
        return Py_None;
    return PythonObjectAdapter::fromDataObject(ele);
}

PyObject *wrap_randFromDataSchema(PyObject *self, PyObject *args) {
    if (state != PARSED)
        return Py_None;
    char *nameObj;
    if (!PyArg_ParseTuple(args, "s", &nameObj))
        return Py_None;
    string name = string(nameObj);
    DataSchemaObject *o = controller->definitions->getSchemaByName(name);
    if (o == NULL)
        return Py_None;
    BaseDataObject *ele = o->generate();
    if (ele == NULL)
        return Py_None;
    PyObject *ans = PythonObjectAdapter::fromDataObject(ele);
    if (ele) delete ele;
    return ans;
}

PyObject *wrap_checkDataBySchema(PyObject *self, PyObject *args) {
    if (state != PARSED)
        return Py_None;
    PyObject *dataObj;
    char *nameObj;
    if (!PyArg_ParseTuple(args, "Os", &dataObj, &nameObj))
        return Py_None;
    string name = string(nameObj);
    DataSchemaObject *o = controller->definitions->getSchemaByName(name);
    if (o == NULL)
        return Py_None;
    BaseDataObject *obj = PythonObjectAdapter::toDataObject(dataObj);
    if (obj == NULL)
        return Py_None;
    bool ans = o->check(obj);
    delete obj;
    if (ans)
        return Py_True;
    else
        return Py_False;
}

// ------

PyObject *wrap_getParameterNames(PyObject *self, PyObject *args) {
    if (state != PARSED)
        return Py_None;
    const map<string, DocElement *> &nameMap = controller->parameters->getNameMap();
    PyObject *l = PyList_New(nameMap.size());
    int i = 0;
    for (map<string, DocElement *>::const_iterator ite = nameMap.cbegin();
         ite != nameMap.cend();
         ++ite) {
        PyList_SetItem(l, (Py_ssize_t) i, Py_BuildValue("s", ite->first.c_str()));
        ++i;
    }
    return l;
}

PyObject *wrap_getParameterByName(PyObject *self, PyObject *args) {
    if (state != PARSED)
        return Py_None;
    char *nameObj;
    if (!PyArg_ParseTuple(args, "s", &nameObj))
        return Py_None;
    string name = string(nameObj);
    ParameterObject *o = controller->parameters->getParameterByName(name);
    if (o == NULL)
        return Py_None;
    BaseDataObject *ele = o->toDataObject();
    if (ele == NULL)
        return Py_None;
    return PythonObjectAdapter::fromDataObject(ele);
}

PyObject *wrap_randFromParameter(PyObject *self, PyObject *args) {
    if (state != PARSED)
        return Py_None;
    char *nameObj;
    if (!PyArg_ParseTuple(args, "s", &nameObj))
        return Py_None;
    string name = string(nameObj);
    ParameterObject *o = controller->parameters->getParameterByName(name);
    if (o == NULL)
        return Py_None;
    BaseDataObject *ele = o->schema->generate();
    if (ele == NULL)
        return Py_None;
    PyObject *ans = PythonObjectAdapter::fromDataObject(ele);
    if (ele) delete ele;
    return ans;
}

PyObject *wrap_checkDataByParam(PyObject *self, PyObject *args) {
    if (state != PARSED)
        return Py_None;
    PyObject *dataObj;
    char *nameObj;
    if (!PyArg_ParseTuple(args, "Os", &dataObj, &nameObj))
        return Py_None;
    string name = string(nameObj);
    ParameterObject *o = controller->parameters->getParameterByName(name);
    if (o == NULL)
        return Py_None;
    BaseDataObject *obj = PythonObjectAdapter::toDataObject(dataObj);
    if (obj == NULL)
        return Py_None;
    bool ans = o->schema->check(obj);
    delete obj;
    if (ans)
        return Py_True;
    else
        return Py_False;
}

PyObject *wrap_getResponseNames(PyObject *self, PyObject *args) {
    if (state != PARSED)
        return Py_None;
    const map<string, DocElement *> &nameMap = controller->responses->getNameMap();
    PyObject *l = PyList_New(nameMap.size());
    int i = 0;
    for (map<string, DocElement *>::const_iterator ite = nameMap.cbegin();
         ite != nameMap.cend();
         ++ite) {
        PyList_SetItem(l, (Py_ssize_t) i, Py_BuildValue("s", ite->first.c_str()));
        ++i;
    }
    return l;
}

PyObject *wrap_getResponseByName(PyObject *self, PyObject *args) {
    if (state != PARSED)
        return Py_None;
    char *nameObj;
    if (!PyArg_ParseTuple(args, "s", &nameObj))
        return Py_None;
    string name = string(nameObj);
    AbstractResponseObject *o = controller->responses->getResponseByName(name);
    if (o == NULL)
        return Py_None;
    BaseDataObject *ele = o->toDataObject();
    if (ele == NULL)
        return Py_None;
    return PythonObjectAdapter::fromDataObject(ele);
}

PyObject *wrap_checkDataByResponse(PyObject *self, PyObject *args) {
    if (state != PARSED)
        return Py_None;
    PyObject *dataObj;
    char *nameObj;
    if (!PyArg_ParseTuple(args, "Os", &dataObj, &nameObj))
        return Py_None;
    string name = string(nameObj);
    AbstractResponseObject *o = controller->responses->getResponseByName(name);
    if (o == NULL)
        return Py_None;
    BaseDataObject *obj = PythonObjectAdapter::toDataObject(dataObj);
    DocElement *ele = DataObjectAdapter::toDocElement(obj);
    if (obj == NULL)
        return Py_None;
    bool ans = o->checkResponse(ele);
    delete ele;
    if (ans)
        return Py_True;
    else
        return Py_False;
}

static PyMethodDef LapisParserMethods[] = {
        {"read",                wrap_read,                METH_VARARGS, "Read YAML and XML format files."},
        {"readYAML",            wrap_readYAML,            METH_VARARGS, "Read YAML format files."},
        {"readXML",             wrap_readXML,             METH_VARARGS, "Read XML format files."},
        {"getErrors",           wrap_getErrors,           METH_VARARGS, "Get all parse errors."},
        {"translate2YAML",      wrap_translate2YAML,      METH_VARARGS, "Translate parsed doc to YAML."},
        {"translate2XML",       wrap_translate2XML,       METH_VARARGS, "Translate parsed doc to XML."},

        {"getInfo",             wrap_getInfo,             METH_VARARGS, "Get the Info Object."},
        {"getHost",             wrap_getHost,             METH_VARARGS, "Get the host url"},
        {"getBasePath",         wrap_getBasePath,         METH_VARARGS, "Get the basePath"},
        {"getSchemes",          wrap_getSchemes,          METH_VARARGS, "Get the schemes list"},
        {"getConsumes",         wrap_getConsumes,         METH_VARARGS, "Get the consumes list"},
        {"getProduces",         wrap_getProduces,         METH_VARARGS, "Get the produces list"},
        {"getTags",             wrap_getTags,             METH_VARARGS, "Get the tags list"},
        {"getExternalDocs",     wrap_getExternalDocs,     METH_VARARGS, "Get the externalDocs Object"},

        {"getDataSchemaNames",  wrap_getDataSchemaNames,  METH_VARARGS, "Get the names for data schemas"},
        {"getDataSchemaByName", wrap_getDataSchemaByName, METH_VARARGS, "Get the data schema structure by name"},
        {"randFromDataSchema",  wrap_randFromDataSchema,  METH_VARARGS, "Generate a random value from data schema definition"},
        {"checkDataBySchema",   wrap_checkDataBySchema,   METH_VARARGS, "Check whether the data object conforms to the data schema"},

        {"getParameterNames",   wrap_getParameterNames,   METH_VARARGS, "Get the names for parameters"},
        {"getParameterByName",  wrap_getParameterByName,  METH_VARARGS, "Get the parameter structure by name"},
        {"randFromParameter",   wrap_randFromParameter,   METH_VARARGS, "Generate a random value from schema definition of parameter"},
        {"checkDataByParam",    wrap_checkDataByParam,    METH_VARARGS, "Check whether the data object conforms to the data schema of parameter"},

        {"getResponseNames",    wrap_getResponseNames,    METH_VARARGS, "Get the names for responses"},
        {"getResponseByName",   wrap_getResponseByName,   METH_VARARGS, "Get the response structure by name"},
        {"checkDataByResponse", wrap_checkDataByResponse, METH_VARARGS, "Check whether the data object conforms to the data schema of response"},

        {NULL, NULL,}
};

PyMODINIT_FUNC initLapisParser() {
    state = START;
    Py_InitModule("LapisParser", LapisParserMethods);
}