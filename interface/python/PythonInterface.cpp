//
// Created by lly on 30/05/2017.
//

/**
 * Python Interface
 * Module Name: LapisParser
 */

#include "PythonInterface.h"

#define PARSE_sIsOb(args, func) {               \
    PyObject **arg = parseHelper(args, 1, 2);   \
    if (arg == NULL) return Py_None;            \
                                                \
    string arg0 = "", arg1 = "";                \
    if (PyString_Check(arg[0]))                 \
        arg0 = PyString_AsString(arg[0]);       \
    else {                                      \
        delete[] arg;                           \
        return Py_None;                         \
    }                                           \
    if (arg[1] && PyString_Check(arg[1]))       \
        arg1 = PyString_AsString(arg[1]);       \
    delete[] arg;                               \
                                                \
    bool ret;                                   \
    if (arg1 == "")                             \
        ret = func(arg0);                       \
    else                                        \
        ret = func(arg0, arg1);                 \
    return ret ? Py_True : Py_False;            \
}

#define PARSE_oIsOb(args, func) {               \
    PyObject **arg = parseHelper(args, 1, 2);   \
    if (arg == NULL) return Py_None;            \
                                                \
    BaseDataObject *arg0 = PythonObjectAdapter::toDataObject(arg[0]); \
    string arg1 = "";                           \
    if (arg[1] && PyString_Check(arg[1]))       \
        arg1 = PyString_AsString(arg[1]);       \
    delete[] arg;                               \
                                                \
    bool ret;                                   \
    if (arg1 == "")                             \
        ret = func(arg0);                       \
    else                                        \
        ret = func(arg0, arg1);                 \
    delete arg0;                                \
    return ret ? Py_True : Py_False;            \
}

#define PARSE_osOb(args, func) {                \
    PyObject **arg = parseHelper(args, 2, 2);   \
    if (arg == NULL) return Py_None;            \
                                                \
    BaseDataObject *arg0 = PythonObjectAdapter::toDataObject(arg[0]); \
    string arg1 = "";                           \
    if (PyString_Check(arg[1]))                 \
        arg1 = PyString_AsString(arg[1]);       \
    else {                                      \
        delete[] arg;                           \
        delete arg0;                            \
        return Py_None;                         \
    }                                           \
    delete[] arg;                               \
                                                \
    bool ret;                                   \
    ret = func(arg0, arg1);                     \
    delete arg0;                                \
    return ret ? Py_True : Py_False;            \
}


#define PARSE_Oo(func) {                        \
    BaseDataObject *ret = func();               \
    PyObject *rett = PythonObjectAdapter::fromDataObject(ret); \
    if (ret) delete ret;                        \
    return rett;                                \
}

#define PARSE_IsOb(args, func) {                \
    PyObject **arg = parseHelper(args, 0, 1);   \
    if (arg == NULL) return Py_None;            \
                                                \
    string arg0 = "";                           \
    if (PyString_Check(arg[0]))                 \
        arg0 = PyString_AsString(arg[0]);       \
    delete[] arg;                               \
                                                \
    bool ret;                                   \
    if (arg0 == "")                             \
        ret = func();                           \
    else                                        \
        ret = func(arg0);                       \
    return ret ? Py_True : Py_False;            \
}

#define PARSE_osIsOb(args, func) {              \
    PyObject **arg = parseHelper(args, 2, 3);   \
    if (arg == NULL) return Py_None;            \
                                                \
    BaseDataObject *arg0 = PythonObjectAdapter::toDataObject(arg[0]); \
    string arg1 = "", arg2 = "";                \
    if (PyString_Check(arg[1]))                 \
        arg1 = PyString_AsString(arg[1]);       \
    else {                                      \
        delete[] arg;                           \
        delete arg0;                            \
        return Py_None;                         \
    }                                           \
    if (PyString_Check(arg[2]))                 \
        arg2 = PyString_AsString(arg[2]);       \
    delete[] arg;                               \
                                                \
    bool ret;                                   \
    if (arg2 == "")                             \
        ret = func(arg0, arg1);                 \
    else                                        \
        ret = func(arg0, arg1, arg2);           \
    delete arg0;                                \
    return ret ? Py_True : Py_False;            \
}

#define PARSE_ossIsOb(args, func) {             \
    PyObject **arg = parseHelper(args, 3, 4);   \
    if (arg == NULL) return Py_None;            \
                                                \
    BaseDataObject *arg0 = PythonObjectAdapter::toDataObject(arg[0]); \
    string arg1 = "", arg2 = "", arg3 = "";     \
    if (PyString_Check(arg[1]))                 \
        arg1 = PyString_AsString(arg[1]);       \
    else {                                      \
        delete[] arg;                           \
        delete arg0;                            \
        return Py_None;                         \
    }                                           \
    if (PyString_Check(arg[2]))                 \
        arg2 = PyString_AsString(arg[2]);       \
    else {                                      \
        delete[] arg;                           \
        delete arg0;                            \
        return Py_None;                         \
    }                                           \
    if (PyString_Check(arg[3]))                 \
        arg3 = PyString_AsString(arg[3]);       \
    delete[] arg;                               \
                                                \
    bool ret;                                   \
    if (arg3 == "")                             \
        ret = func(arg0, arg1, arg2);           \
    else                                        \
        ret = func(arg0, arg1, arg2, arg3);     \
    delete arg0;                                \
    return ret ? Py_True : Py_False;            \
}

#define PARSE_ssIsOb(args, func) {              \
    PyObject **arg = parseHelper(args, 2, 3);   \
    if (arg == NULL) return Py_None;            \
                                                \
    string arg0 = "", arg1 = "", arg2 = "";     \
    if (PyString_Check(arg[0]))                 \
        arg0 = PyString_AsString(arg[0]);       \
    else {                                      \
        delete[] arg;                           \
        return Py_None;                         \
    }                                           \
    if (PyString_Check(arg[1]))                 \
        arg1 = PyString_AsString(arg[1]);       \
    else {                                      \
        delete[] arg;                           \
        return Py_None;                         \
    }                                           \
    if (PyString_Check(arg[2]))                 \
        arg2 = PyString_AsString(arg[2]);       \
    delete[] arg;                               \
                                                \
    bool ret;                                   \
    if (arg2 == "")                             \
        ret = func(arg0, arg1);                 \
    else                                        \
        ret = func(arg0, arg1, arg2);           \
    return ret ? Py_True : Py_False;            \
}

#define PARSE_ssOo(args, func) {                \
    PyObject **arg = parseHelper(args, 2, 2);   \
    if (arg == NULL) return Py_None;            \
                                                \
    string arg0 = "", arg1 = "";                \
    if (PyString_Check(arg[0]))                 \
        arg0 = PyString_AsString(arg[0]);       \
    else {                                      \
        delete[] arg;                           \
        return Py_None;                         \
    }                                           \
    if (PyString_Check(arg[1]))                 \
        arg1 = PyString_AsString(arg[1]);       \
    else {                                      \
        delete[] arg;                           \
        return Py_None;                         \
    }                                           \
    delete[] arg;                               \
                                                \
    BaseDataObject *ret = func(arg0, arg1);     \
    PyObject *rett = PythonObjectAdapter::fromDataObject(ret); \
    if (ret) delete ret;                        \
    return rett;                                \
}

#define PARSE_sOo(args, func) {                 \
    PyObject **arg = parseHelper(args, 1, 1);   \
    if (arg == NULL) return Py_None;            \
                                                \
    string arg0 = "";                           \
    if (PyString_Check(arg[0]))                 \
        arg0 = PyString_AsString(arg[0]);       \
    else {                                      \
        delete[] arg;                           \
        return Py_None;                         \
    }                                           \
                                                \
    BaseDataObject *ret = func(arg0);           \
    PyObject *rett = PythonObjectAdapter::fromDataObject(ret); \
    if (ret) delete ret;                        \
    return rett;                                \
}

/* ---------- */

PyObject **parseHelper(PyObject *&args, int min, int max) {
    if ((max > 10) || (min > max) || (min < 0) || (max <= 0))
        return NULL;
    PyObject **arr = new PyObject *[max];
    memset(arr, 0, sizeof(PyObject *) * max);
    bool ret = false;
    switch (max) {
        case 1:
            ret = (bool) PyArg_UnpackTuple(args, "lapis_arg", min, max,
                                           arr);
            break;
        case 2:
            ret = (bool) PyArg_UnpackTuple(args, "lapis_arg", min, max,
                                           arr, arr + 1);
            break;
        case 3:
            ret = (bool) PyArg_UnpackTuple(args, "lapis_arg", min, max,
                                           arr, arr + 1, arr + 2);
            break;
        case 4:
            ret = (bool) PyArg_UnpackTuple(args, "lapis_arg", min, max,
                                           arr, arr + 1, arr + 2, arr + 3);
            break;
        case 5:
            ret = (bool) PyArg_UnpackTuple(args, "lapis_arg", min, max,
                                           arr, arr + 1, arr + 2, arr + 3, arr + 4);
            break;
        case 6:
            ret = (bool) PyArg_UnpackTuple(args, "lapis_arg", min, max,
                                           arr, arr + 1, arr + 2, arr + 3, arr + 4,
                                           arr + 5);
            break;
        case 7:
            ret = (bool) PyArg_UnpackTuple(args, "lapis_arg", min, max,
                                           arr, arr + 1, arr + 2, arr + 3, arr + 4,
                                           arr + 5, arr + 6);
            break;
        case 8:
            ret = (bool) PyArg_UnpackTuple(args, "lapis_arg", min, max,
                                           arr, arr + 1, arr + 2, arr + 3, arr + 4,
                                           arr + 5, arr + 6, arr + 7);
            break;
        case 9:
            ret = (bool) PyArg_UnpackTuple(args, "lapis_arg", min, max,
                                           arr, arr + 1, arr + 2, arr + 3, arr + 4,
                                           arr + 5, arr + 6, arr + 7, arr + 8);
            break;
        case 10:
            ret = (bool) PyArg_UnpackTuple(args, "lapis_arg", min, max,
                                           arr, arr + 1, arr + 2, arr + 3, arr + 4,
                                           arr + 5, arr + 6, arr + 7, arr + 8, arr + 9);
            break;
        default:
            ret = false;
    }
    if (!ret) {
        delete[] arr;
        return NULL;
    }
    return arr;
}

/* ---------- */

PyObject *wrap_getErrors
        (PyObject *self, PyObject *args) {
    /** Oo **/
    PARSE_Oo(getErrors)
}

PyObject *wrap_cleanErrors
        (PyObject *self, PyObject *args) {
    /** O **/
    cleanErrors();
    return Py_None;
}

PyObject *wrap_addDocFromFile
        (PyObject *self, PyObject *args) {
    /** sIsOb **/
    PARSE_sIsOb(args, addDocFromFile)
}

PyObject *wrap_addYAMLDocFromFile
        (PyObject *self, PyObject *args) {
    /** sIsOb **/
    PARSE_sIsOb(args, addYAMLDocFromFile)
}

PyObject *wrap_addXMLDocFromFile
        (PyObject *self, PyObject *args) {
    /** sIsOb **/
    PARSE_sIsOb(args, addXMLDocFromFile)
}

PyObject *wrap_removeDoc
        (PyObject *self, PyObject *args) {
    /** sOb **/
    PyObject **arg = parseHelper(args, 1, 1);
    if (arg == NULL) return Py_None;

    string arg0 = "";
    if (PyString_Check(arg[0]))
        arg0 = PyString_AsString(arg[0]);
    else {
        delete[] arg;
        return Py_None;
    }
    delete[] arg;

    bool ret;
    ret = removeDoc(arg0);
    return ret ? Py_True : Py_False;
}

PyObject *wrap_getDocList
        (PyObject *self, PyObject *args) {
    /** Oo **/
    PARSE_Oo(getDocList)
}

PyObject *wrap_saveAsYAML
        (PyObject *self, PyObject *args) {
    /** sIsOb **/
    PARSE_sIsOb(args, saveAsYAML);
}

PyObject *wrap_saveAsXML
        (PyObject *self, PyObject *args) {
    /** sIsOb **/
    PARSE_sIsOb(args, saveAsXML);
}

PyObject *wrap_addDoc
        (PyObject *self, PyObject *args) {
    /** oIsOb **/
    PARSE_oIsOb(args, addDoc)
}

PyObject *wrap_reservedFieldsInit
        (PyObject *self, PyObject *args) {
    /** IsOb **/
    PARSE_IsOb(args, reservedFieldsInit)
}

PyObject *wrap_setInfo
        (PyObject *self, PyObject *args) {
    /** oIsOb **/
    PARSE_oIsOb(args, setInfo)
}

PyObject *wrap_setHost
        (PyObject *self, PyObject *args) {
    /** sIsOb **/
    PARSE_sIsOb(args, setHost);
}

PyObject *wrap_setBasePath
        (PyObject *self, PyObject *args) {
    /** sIsOb **/
    PARSE_sIsOb(args, setBasePath);
}

PyObject *wrap_removeBasePath
        (PyObject *self, PyObject *args) {
    /** IsOb **/
    PARSE_IsOb(args, removeBasePath)
}

PyObject *wrap_setSchemes
        (PyObject *self, PyObject *args) {
    /** oIsOb **/
    PARSE_oIsOb(args, setSchemes)
}

PyObject *wrap_removeSchemes
        (PyObject *self, PyObject *args) {
    /** IsOb **/
    PARSE_IsOb(args, removeSchemes)
}

PyObject *wrap_setConsumes
        (PyObject *self, PyObject *args) {
    /** oIsOb **/
    PARSE_oIsOb(args, setConsumes)
}

PyObject *wrap_removeConsumes
        (PyObject *self, PyObject *args) {
    /** IsOb **/
    PARSE_IsOb(args, removeConsumes)
}

PyObject *wrap_setProduces
        (PyObject *self, PyObject *args) {
    /** oIsOb **/
    PARSE_oIsOb(args, setProduces)
}

PyObject *wrap_removeProduces
        (PyObject *self, PyObject *args) {
    /** IsOb **/
    PARSE_IsOb(args, removeProduces)
}

PyObject *wrap_setTags
        (PyObject *self, PyObject *args) {
    /** oIsOb **/
    PARSE_oIsOb(args, setTags)
}

PyObject *wrap_removeTags
        (PyObject *self, PyObject *args) {
    /** IsOb **/
    PARSE_IsOb(args, removeTags)
}

PyObject *wrap_setExternalDocs
        (PyObject *self, PyObject *args) {
    /** oIsOb **/
    PARSE_oIsOb(args, setExternalDocs)
}

PyObject *wrap_removeExternalDocs
        (PyObject *self, PyObject *args) {
    /** IsOb **/
    PARSE_IsOb(args, removeExternalDocs)
}

PyObject *wrap_addAPISharedParameter
        (PyObject *self, PyObject *args) {
    /** osIsOb **/
    PARSE_osIsOb(args, addAPISharedParameter)
}

PyObject *wrap_removeAPISharedParameters
        (PyObject *self, PyObject *args) {
    /** sIsOb **/
    PARSE_sIsOb(args, removeAPISharedParameters)
}

PyObject *wrap_addAPI
        (PyObject *self, PyObject *args) {
    /** ossIsOb **/
    PARSE_ossIsOb(args, addAPI)
}

PyObject *wrap_removeAPI
        (PyObject *self, PyObject *args) {
    /** ssIsOb **/
    PARSE_ssIsOb(args, removeAPI)
}

PyObject *wrap_addDataSchema
        (PyObject *self, PyObject *args) {
    /** osIsOb **/
    PARSE_osIsOb(args, addDataSchema)
}

PyObject *wrap_removeDataSchema
        (PyObject *self, PyObject *args) {
    /** sIsOb **/
    PARSE_sIsOb(args, removeDataSchema)
}

PyObject *wrap_addParameter
        (PyObject *self, PyObject *args) {
    /** osIsOb **/
    PARSE_osIsOb(args, addParameter)
}

PyObject *wrap_removeParameter
        (PyObject *self, PyObject *args) {
    /** sIsOb **/
    PARSE_sIsOb(args, removeParameter)
}

PyObject *wrap_addResponse
        (PyObject *self, PyObject *args) {
    /** osIsOb **/
    PARSE_osIsOb(args, addResponse)
}

PyObject *wrap_removeResponse
        (PyObject *self, PyObject *args) {
    /** sIsOb **/
    PARSE_sIsOb(args, removeResponse)
}

PyObject *wrap_parseAPI
        (PyObject *self, PyObject *args) {
    /** Ob **/
    bool ret = parseAPI();
    return ret ? Py_True : Py_False;
}

PyObject *wrap_getInfo
        (PyObject *self, PyObject *args) {
    /** Oo **/
    PARSE_Oo(getInfo)
}

PyObject *wrap_getHost
        (PyObject *self, PyObject *args) {
    /** Oo **/
    PARSE_Oo(getHost)
}

PyObject *wrap_getBasePath
        (PyObject *self, PyObject *args) {
    /** Oo **/
    PARSE_Oo(getBasePath)
}

PyObject *wrap_getSchemes
        (PyObject *self, PyObject *args) {
    /** Oo **/
    PARSE_Oo(getSchemes)
}

PyObject *wrap_getConsumes
        (PyObject *self, PyObject *args) {
    /** Oo **/
    PARSE_Oo(getConsumes)
}

PyObject *wrap_getProduces
        (PyObject *self, PyObject *args) {
    /** Oo **/
    PARSE_Oo(getProduces)
}

PyObject *wrap_getAPINames
        (PyObject *self, PyObject *args) {
    /** Oo **/
    PARSE_Oo(getAPINames)
}

PyObject *wrap_getAPI
        (PyObject *self, PyObject *args) {
    /** ssOo **/
    PARSE_ssOo(args, getAPI)
}

PyObject *wrap_getDataSchemaNames
        (PyObject *self, PyObject *args) {
    /** Oo **/
    PARSE_Oo(getDataSchemaNames)
}

PyObject *wrap_getDataSchema
        (PyObject *self, PyObject *args) {
    /** sOo **/
    PARSE_sOo(args, getDataSchema)
}

PyObject *wrap_generateRandomDataFromDataSchema
        (PyObject *self, PyObject *args) {
    /** sOo **/
    PARSE_sOo(args, generateRandomDataFromDataSchema)
}

PyObject *wrap_verifyDataByDataSchema
        (PyObject *self, PyObject *args) {
    /** osOb **/
    PARSE_osOb(args, verifyDataByDataSchema)
}

PyObject *wrap_getParameterNames
        (PyObject *self, PyObject *args) {
    /** Oo **/
    PARSE_Oo(getParameterNames)
}

PyObject *wrap_getParameter
        (PyObject *self, PyObject *args) {
    /** sOo **/
    PARSE_sOo(args, getParameter)
}

PyObject *wrap_getResponseNames
        (PyObject *self, PyObject *args) {
    /** Oo **/
    PARSE_Oo(getResponseNames)
}

PyObject *wrap_getResponse
        (PyObject *self, PyObject *args) {
    /** sOo **/
    PARSE_sOo(args, getResponse)
}

PyObject *wrap_getTags
        (PyObject *self, PyObject *args) {
    /** Oo **/
    PARSE_Oo(getTags)
}

PyObject *wrap_getExternalDocs
        (PyObject *self, PyObject *args) {
    /** Oo **/
    PARSE_Oo(getExternalDocs)
}

/* ---------- */

static PyMethodDef LapisParserMethods[] = {
        {"getErrors",                        wrap_getErrors,                        METH_VARARGS, "getErrors"},
        {"cleanErrors",                      wrap_cleanErrors,                      METH_VARARGS, "cleanErrors"},
        {"addDocFromFile",                   wrap_addDocFromFile,                   METH_VARARGS, "addDocFromFile"},
        {"addYAMLDocFromFile",               wrap_addYAMLDocFromFile,               METH_VARARGS, "addYAMLDocFromFile"},
        {"addXMLDocFromFile",                wrap_addXMLDocFromFile,                METH_VARARGS, "addXMLDocFromFile"},
        {"removeDoc",                        wrap_removeDoc,                        METH_VARARGS, "removeDoc"},
        {"getDocList",                       wrap_getDocList,                       METH_VARARGS, "getDocList"},
        {"saveAsYAML",                       wrap_saveAsYAML,                       METH_VARARGS, "saveAsYAML"},
        {"saveAsXML",                        wrap_saveAsXML,                        METH_VARARGS, "saveAsXML"},
        {"addDoc",                           wrap_addDoc,                           METH_VARARGS, "addDoc"},
        {"reservedFieldsInit",               wrap_reservedFieldsInit,               METH_VARARGS, "reservedFieldsInit"},
        {"setInfo",                          wrap_setInfo,                          METH_VARARGS, "setInfo"},
        {"setHost",                          wrap_setHost,                          METH_VARARGS, "setHost"},
        {"setBasePath",                      wrap_setBasePath,                      METH_VARARGS, "setBasePath"},
        {"removeBasePath",                   wrap_removeBasePath,                   METH_VARARGS, "removeBasePath"},
        {"setSchemes",                       wrap_setSchemes,                       METH_VARARGS, "setSchemes"},
        {"removeSchemes",                    wrap_removeSchemes,                    METH_VARARGS, "removeSchemes"},
        {"setConsumes",                      wrap_setConsumes,                      METH_VARARGS, "setConsumes"},
        {"removeConsumes",                   wrap_removeConsumes,                   METH_VARARGS, "removeConsumes"},
        {"setProduces",                      wrap_setProduces,                      METH_VARARGS, "setProduces"},
        {"removeProduces",                   wrap_removeProduces,                   METH_VARARGS, "removeProduces"},
        {"setTags",                          wrap_setTags,                          METH_VARARGS, "setTags"},
        {"removeTags",                       wrap_removeTags,                       METH_VARARGS, "removeTags"},
        {"setExternalDocs",                  wrap_setExternalDocs,                  METH_VARARGS, "setExternalDocs"},
        {"removeExternalDocs",               wrap_removeExternalDocs,               METH_VARARGS, "removeExternalDocs"},
        {"addAPISharedParameter",            wrap_addAPISharedParameter,            METH_VARARGS, "addAPISharedParameter"},
        {"removeAPISharedParameters",        wrap_removeAPISharedParameters,        METH_VARARGS, "removeAPISharedParameters"},
        {"addAPI",                           wrap_addAPI,                           METH_VARARGS, "addAPI"},
        {"removeAPI",                        wrap_removeAPI,                        METH_VARARGS, "removeAPI"},
        {"addDataSchema",                    wrap_addDataSchema,                    METH_VARARGS, "addDataSchema"},
        {"removeDataSchema",                 wrap_removeDataSchema,                 METH_VARARGS, "removeDataSchema"},
        {"addParameter",                     wrap_addParameter,                     METH_VARARGS, "addParameter"},
        {"removeParameter",                  wrap_removeParameter,                  METH_VARARGS, "removeParameter"},
        {"addResponse",                      wrap_addResponse,                      METH_VARARGS, "addResponse"},
        {"removeResponse",                   wrap_removeResponse,                   METH_VARARGS, "removeResponse"},
        {"parseAPI",                         wrap_parseAPI,                         METH_VARARGS, "parseAPI"},
        {"getInfo",                          wrap_getInfo,                          METH_VARARGS, "getInfo"},
        {"getHost",                          wrap_getHost,                          METH_VARARGS, "getHost"},
        {"getBasePath",                      wrap_getBasePath,                      METH_VARARGS, "getBasePath"},
        {"getSchemes",                       wrap_getSchemes,                       METH_VARARGS, "getSchemes"},
        {"getConsumes",                      wrap_getConsumes,                      METH_VARARGS, "getConsumes"},
        {"getProduces",                      wrap_getProduces,                      METH_VARARGS, "getProduces"},
        {"getAPINames",                      wrap_getAPINames,                      METH_VARARGS, "getAPINames"},
        {"getAPI",                           wrap_getAPI,                           METH_VARARGS, "getAPI"},
        {"getDataSchemaNames",               wrap_getDataSchemaNames,               METH_VARARGS, "getDataSchemaNames"},
        {"getDataSchema",                    wrap_getDataSchema,                    METH_VARARGS, "getDataSchema"},
        {"generateRandomDataFromDataSchema", wrap_generateRandomDataFromDataSchema, METH_VARARGS, "generateRandomDataFromDataSchema"},
        {"verifyDataByDataSchema",           wrap_verifyDataByDataSchema,           METH_VARARGS, "verifyDataByDataSchema"},
        {"getParameterNames",                wrap_getParameterNames,                METH_VARARGS, "getParameterNames"},
        {"getParameter",                     wrap_getParameter,                     METH_VARARGS, "getParameter"},
        {"getResponseNames",                 wrap_getResponseNames,                 METH_VARARGS, "getResponseNames"},
        {"getResponse",                      wrap_getResponse,                      METH_VARARGS, "getResponse"},
        {"getTags",                          wrap_getTags,                          METH_VARARGS, "getTags"},
        {"getExternalDocs",                  wrap_getExternalDocs,                  METH_VARARGS, "getExternalDocs"},
        {NULL, NULL, 0, NULL}
};


PyMODINIT_FUNC initLapisParser() {
    init();
    Py_InitModule("LapisParser", LapisParserMethods);
}
