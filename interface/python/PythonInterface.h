//
// Created by lly on 13/07/2017.
//

#ifndef VPARSER_PYTHONINTERFACE_H
#define VPARSER_PYTHONINTERFACE_H

#endif //VPARSER_PYTHONINTERFACE_H

extern "C" {
#include "python2.7/Python.h"
}

#include "../../error/Error.h"
#include "../../doc/DocElement.h"
#include "../../doc/YAMLAdapter.h"
#include "../../doc/XMLAdapter.h"
#include "../../ref_expand/RefExpand.h"
#include "../../schema/Controller.h"
#include "PythonObjectAdapter.h"

enum CURRENT_STATE {
    START, PARSED, ERROR
};

enum FORMAT {
    YAML, XML
};

/*******************
 * Public Functions
 ******************/

/**
 * Read scripts. YAML or XML is detected by file suffix
 * @param self
 * @param args
 * @return 0 - success, 1 - failed
 */
PyObject *wrap_read(PyObject *self, PyObject *args);

/**
 * Read YAML
 * @param self
 * @param args, contains filePath list
 * @return 0 - success, 1 - failed
 */
PyObject *wrap_readYAML(PyObject *self, PyObject *args);

/**
 * Read XML
 * @param self
 * @param args, contains filePath list
 * @return 0 - success, 1 - failed
 */
PyObject *wrap_readXML(PyObject *self, PyObject *args);

/**
 * Get parse errors
 * @param self
 * @param args, nothing needed
 * @return list, each item is a dict: {fileName: string, line: integer, col: integer, msg: string}
 */
PyObject *wrap_getErrors(PyObject *self, PyObject *args);

/**
 * Translate internal doc to a YAML file
 * @param self
 * @param args, contains a filePath to be written to
 * @return 0 - success, 1 - failed
 */
PyObject *wrap_translate2YAML(PyObject *self, PyObject *args);

/**
 * Translate internal doc to a XML file
 * @param self
 * @param args, contains a filePath to be written to
 * @return 0 - success, 1 - failed
 */
PyObject *wrap_translate2XML(PyObject *self, PyObject *args);

/**
 * Get the Info Object of root element
 * @param self
 * @param args, no argument needed
 * @return a python dict containing the Info Object
 */
PyObject *wrap_getInfo(PyObject *self, PyObject *args);

/**
 * Get the host URL string
 * @param self
 * @param args, no argument needed
 * @return a python string
 */
PyObject *wrap_getHost(PyObject *self, PyObject *args);

/**
 * Get the bashPath string
 * @param self
 * @param args, no argument needed
 * @return a python string if exists, empty string if not exists
 */
PyObject *wrap_getBasePath(PyObject *self, PyObject *args);

/**
 * Get the schemes list
 * @param self
 * @param args, no argument needed
 * @return a python list of string if exists, None if not exists
 */
PyObject *wrap_getSchemes(PyObject *self, PyObject *args);

/**
 * Get the consumes list
 * @param self
 * @param args, no argument needed
 * @return a python list of string if exists, None if not exists
 */
PyObject *wrap_getConsumes(PyObject *self, PyObject *args);

/**
 * Get the produces list
 * @param self
 * @param args, no argument needed
 * @return a python list of string if exists, None if not exists
 */
PyObject *wrap_getProduces(PyObject *self, PyObject *args);

/**
 * Get the tags list
 * @param self
 * @param args, no argument needed
 * @return a python list of dict if exists, None if not exists
 */
PyObject *wrap_getTags(PyObject *self, PyObject *args);

/**
 * Get the externalDocs dict
 * @param self
 * @param args, no argument needed
 * @return a python dict if exists, None if not exists
 */
PyObject *wrap_getExternalDocs(PyObject *self, PyObject *args);

/**
 * Get the names for data schemas
 * @param self
 * @param args, no argument needed
 * @return a python list
 */
PyObject *wrap_getDataSchemaNames(PyObject *self, PyObject *args);

/**
 * Get the data schema structure by name
 * @param self
 * @param args, a string from schema name list
 * @return a python dict representing the data schema
 */
PyObject *wrap_getDataSchemaByName(PyObject *self, PyObject *args);

/**
 * Genrerate a random value from data schema definition
 * @param self
 * @param args, a string from schema name list
 * @return the generated data object
 */
PyObject *wrap_randFromDataSchema(PyObject *self, PyObject *args);

/**
 * Check whether the data object conforms to the data schema
 * @param self
 * @param args, first one is the data object to check, second one is the name of the data schema
 * @return true or false: conform or not conform
 */
PyObject *wrap_checkDataBySchema(PyObject *self, PyObject *args);

/**
 * Get the names for parameters
 * @param self
 * @param args, no argument needed
 * @return a python list
 */
PyObject *wrap_getParameterNames(PyObject *self, PyObject *args);

/**
 * Get the parameter structure by name
 * @param self
 * @param args, a string from parameter name list
 * @return a python dict representing the data schema
 */
PyObject *wrap_getParameterByName(PyObject *self, PyObject *args);

/**
 * Generate a random value from schema definition of parameter
 * @param self
 * @param args, a string from parameter name list
 * @return the generated data object
 */
PyObject *wrap_randFromParameter(PyObject *self, PyObject *args);

/**
// * Check whether the data object conforms to the data schema of parameter
 * @param self
 * @param args, first one is the data object to check, second one is the name of the parameter
 * @return true or false: conform or not conform
 */
PyObject *wrap_checkDataByParam(PyObject *self, PyObject *args);

/**
 * Get the names for responses
 * @param self
 * @param args, no argument needed
 * @return a python list
 */
PyObject *wrap_getResponseNames(PyObject *self, PyObject *args);

/**
 * Get the response structure by name
 * The return value could be Response Object or Response Extension Object
 * If it has 'name' field, then Extension Object. Otherwise Response Object.
 * @param self
 * @param args, a string from parameter name list
 * @return a python dict representing the data schema
 */
PyObject *wrap_getResponseByName(PyObject *self, PyObject *args);

/**
 * Check whether the data object conforms to the data schema of response
 * @param self
 * @param args, first one is the data object to check, second one is the name of the response
 * @return true or false: conform or not conform
 */
PyObject *wrap_checkDataByResponse(PyObject *self, PyObject *args);


/*******************
 * Private Functions
 ******************/

/**
 *
 * @param filec: # of files
 * @param filev: char* array of file pathname
 * @param format: file format: yaml or xml
 * @return Whether successfully read
 */
bool readFiles(int filec, char **filev, FORMAT* format);

/**
 * Get parse errors
 * @return vector of errors
 */
std::vector<Error *> *getErrors();

/**
 * Translate stored doc to XML or YAML
 * But only translate the first doc in map now
 * @param path
 * @param format
 * @return true: succeed, false: failed
 */
bool translate(char *path, FORMAT format);

/*******************
 * Global Variables
 ******************/
extern Controller *controller;