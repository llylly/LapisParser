//
// Created by lly on 13/07/2017.
//

#ifndef VPARSER_PYTHONINTERFACE_H
#define VPARSER_PYTHONINTERFACE_H

extern "C" {
#include "python2.7/Python.h"
}
#include "PythonObjectAdapter.h"
#include "../Interface.h"


PyObject *wrap_getErrors
        (PyObject *self, PyObject *args);
PyObject *wrap_cleanErrors
        (PyObject *self, PyObject *args);
PyObject *wrap_addDocFromFile
        (PyObject *self, PyObject *args);
PyObject *wrap_addYAMLDocFromFile
        (PyObject *self, PyObject *args);
PyObject *wrap_addXMLDocFromFile
        (PyObject *self, PyObject *args);
PyObject *wrap_addDoc
        (PyObject *self, PyObject *args);
PyObject *wrap_getDoc
        (PyObject *self, PyObject *args);
PyObject *wrap_removeDoc
        (PyObject *self, PyObject *args);
PyObject *wrap_getDocList
        (PyObject *self, PyObject *args);
PyObject *wrap_saveAsYAML
        (PyObject *self, PyObject *args);
PyObject *wrap_saveAsXML
        (PyObject *self, PyObject *args);
PyObject *wrap_reservedFieldsInit
        (PyObject *self, PyObject *args);
PyObject *wrap_setInfo
        (PyObject *self, PyObject *args);
PyObject *wrap_setHost
        (PyObject *self, PyObject *args);
PyObject *wrap_setBasePath
        (PyObject *self, PyObject *args);
PyObject *wrap_removeBasePath
        (PyObject *self, PyObject *args);
PyObject *wrap_setSchemes
        (PyObject *self, PyObject *args);
PyObject *wrap_removeSchemes
        (PyObject *self, PyObject *args);
PyObject *wrap_setConsumes
        (PyObject *self, PyObject *args);
PyObject *wrap_removeConsumes
        (PyObject *self, PyObject *args);
PyObject *wrap_setProduces
        (PyObject *self, PyObject *args);
PyObject *wrap_removeProduces
        (PyObject *self, PyObject *args);
PyObject *wrap_setTags
        (PyObject *self, PyObject *args);
PyObject *wrap_removeTags
        (PyObject *self, PyObject *args);
PyObject *wrap_setExternalDocs
        (PyObject *self, PyObject *args);
PyObject *wrap_removeExternalDocs
        (PyObject *self, PyObject *args);
PyObject *wrap_addAPISharedParameter
        (PyObject *self, PyObject *args);
PyObject *wrap_removeAPISharedParameters
        (PyObject *self, PyObject *args);
PyObject *wrap_addAPI
        (PyObject *self, PyObject *args);
PyObject *wrap_removeAPI
        (PyObject *self, PyObject *args);
PyObject *wrap_addDataSchema
        (PyObject *self, PyObject *args);
PyObject *wrap_removeDataSchema
        (PyObject *self, PyObject *args);
PyObject *wrap_addParameter
        (PyObject *self, PyObject *args);
PyObject *wrap_removeParameter
        (PyObject *self, PyObject *args);
PyObject *wrap_addResponse
        (PyObject *self, PyObject *args);
PyObject *wrap_removeResponse
        (PyObject *self, PyObject *args);
PyObject *wrap_parseAPI
        (PyObject *self, PyObject *args);
PyObject *wrap_getInfo
        (PyObject *self, PyObject *args);
PyObject *wrap_getHost
        (PyObject *self, PyObject *args);
PyObject *wrap_getBasePath
        (PyObject *self, PyObject *args);
PyObject *wrap_getSchemes
        (PyObject *self, PyObject *args);
PyObject *wrap_getConsumes
        (PyObject *self, PyObject *args);
PyObject *wrap_getProduces
        (PyObject *self, PyObject *args);
PyObject *wrap_getAPINames
        (PyObject *self, PyObject *args);
PyObject *wrap_getAPI
        (PyObject *self, PyObject *args);
PyObject *wrap_getDataSchemaNames
        (PyObject *self, PyObject *args);
PyObject *wrap_getDataSchema
        (PyObject *self, PyObject *args);
PyObject *wrap_generateRandomDataFromDataSchema
        (PyObject *self, PyObject *args);
PyObject *wrap_verifyDataByDataSchema
        (PyObject *self, PyObject *args);
PyObject *wrap_generateRandomDataFromAPISchema
        (PyObject *self, PyObject *args);
PyObject *wrap_verifyDataByAPISchema
        (PyObject *self, PyObject *args);
PyObject *wrap_getParameterNames
        (PyObject *self, PyObject *args);
PyObject *wrap_getParameter
        (PyObject *self, PyObject *args);
PyObject *wrap_getResponseNames
        (PyObject *self, PyObject *args);
PyObject *wrap_getResponse
        (PyObject *self, PyObject *args);
PyObject *wrap_getTags
        (PyObject *self, PyObject *args);
PyObject *wrap_getExternalDocs
        (PyObject *self, PyObject *args);
PyObject *wrap_parseScenario
        (PyObject *self, PyObject *args);
PyObject *wrap_getScenarioNames
        (PyObject *self, PyObject *args);
PyObject *wrap_getScenario
        (PyObject *self, PyObject *args);
PyObject *wrap_parseConfig
        (PyObject *self, PyObject *args);
PyObject *wrap_getConfig
        (PyObject *self, PyObject *args);
PyObject *wrap_runSingleAPI
        (PyObject *self, PyObject *args);
PyObject *wrap_runSingleAPIforAli
        (PyObject *self, PyObject *args);
PyObject *wrap_runScenario
        (PyObject *self, PyObject *args);
PyObject *wrap_getRuntimeErrors
        (PyObject *self, PyObject *args);
PyObject *wrap_cleanRuntimeErrors
        (PyObject *self, PyObject *args);
PyObject *wrap_getRuntimeLogs
        (PyObject *self, PyObject *args);
PyObject *wrap_cleanRuntimeLogs
        (PyObject *self, PyObject *args);

#endif //VPARSER_PYTHONINTERFACE_H