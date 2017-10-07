//
// Created by lly on 29/07/2017.
//

/**
 * @file Interface.h
 * @brief Public interface for Lapis Parser.
 *
 * Include the header file with the code:
 * @code
 * #include "Interface.h"
 * @endcode
 */

#ifndef VPARSER_INTERFACE_H
#define VPARSER_INTERFACE_H

#include <cstdio>
#include <unistd.h>
#include "../doc/DocElement.h"
#include "../doc/XMLAdapter.h"
#include "../doc/YAMLAdapter.h"
#include "../ref_expand/RefExpand.h"
#include "../schema/Controller.h"
#include "../error/FileNotExistError.h"
#include "../error/DocNodeNotExistError.h"
#include "../error/NotParsedError.h"
#include "../error/NotConfigParsedError.h"
#include "../scenario/Scenarios.h"
#include "../config/ConfigObject.h"
#include "../exec/request/SingleRequester.h"

enum InterfaceState {
    DOC_TREE, API_PARSED, SCENARIO_PARSED, CONFIG_PARSED
};

extern Controller *controller;
extern Scenarios *scenarios;
extern ConfigObject *config;
extern InterfaceState state;

/** @cond --- Init --- @endcond **/
    /**
     * Interface Initialization.
     *
     * Should be executed whatever.
     *
     * For CPP interface, you should visibly call it before using any other interfaces.
     *
     * For Python interface, it's not supported because it has been automatically called when importing the package.
     */
    void init();

/** @cond --- Error Related --- @endcond **/

    /**
     * Get errors array in Data Object format.
     *
     * @return the errors array
     */
    SequenceDataObject *getErrors();

    /**
     * Clean the inner errors array.
     *
     * Empty error array will be obtained after it's called
     */
    void cleanErrors();

/** @cond --- DocNode Editor --- @endcond **/

    /**
     * Add a XML/YAML doc to the doc forest.
     *
     * Format is automatically deducted from format suffix
     *
     * The operation will discard the parsed results of the doc forest. Should call parseAPI() again for parsing
     * @see parseAPI()
     * @param filePath: the file path
     * @param docName: the name in doc forest, if empty, use filePath
     * @return bool: successfully parsed or not
     */
    bool addDocFromFile(string filePath, string docName = "");

    /**
     * Add a YAML doc to the doc forest.
     *
     * The operation will discard the parsed results of the doc forest. Should call parseAPI() again for parsing
     * @see parseAPI()
     * @param filePath: the file path
     * @param docName: the name in doc forest, if empty, use filePath
     * @return bool: successfully parsed or not
     */
    bool addYAMLDocFromFile(string filePath, string docName = "");

    /**
     * Add a XML doc to the doc forest.
     *
     * The operation will discard the parsed results of the doc forest. Should call parseAPI() again for parsing
     * @see parseAPI()
     * @param filePath: the file path
     * @param docName: the name in doc forest, if empty, use filePath
     * @return bool: successfully parsed or not
     */
    bool addXMLDocFromFile(string filePath, string docName = "");

    /**
     * Add a doc tree to the doc forest.
     *
     * The doc tree is directly parsed from DataObject
     *
     * The operation will discard the parsed results of the doc forest. Should call parseAPI() again for parsing
     * @see parseAPI()
     * @param obj: the DataObject to be parsed and added
     * @param docName: the name in doc forest
     * @return bool: successfully added or not
     */
    bool addDoc(BaseDataObject *obj, string docName = "default");

    /**
     * Get the doc tree in the doc forest.
     *
     * The doc tree is transformed to DataObject
     * @param docName: the doc tree name
     * @return bool: successfully found and transformed or not
     */
    BaseDataObject *getDoc(string docName = "default");

    /**
     * Delete a doc in the doc forest.
     *
     * The operation will discard the parsed results of the doc forest. Should call parseAPI() again for parsing
     * @see parseAPI()
     * @param docName: the name in doc forest
     * @return bool: successfully removed or not
     */
    bool removeDoc(string docName);

    /**
     * Get the array list of all doc names in doc forest.
     * @return the doc name array
     */
    SequenceDataObject *getDocList();

    /**
     * Save the specified doc in doc forest to YAML file.
     * @param destFileName: the output file path
     * @param docName: the doc name in doc forest
     * @return bool: successfully saved or not
     */
    bool saveAsYAML(string destFileName, string docName = "default");

    /**
    * Save the specified doc in doc forest to XML file.
    * @param destFileName: the output file path
    * @param docName: the doc name in doc forest
    * @return bool: successfully saved or not
    */
    bool saveAsXML(string destFileName, string docName = "default");

/** @cond --- API Subitem Editor --- @endcond **/

    /**
     * Init 'swagger: 2.0' & 'x-vee: 2.0 AS' to the specified doc.
     *
     * The operation will discard the parsed results of the doc forest. Should call parseAPI() again for parsing
     * @see parseAPI()
     * @param docName: name of specified doc; default: 'default'
     * @return success or fail
     */
    bool reservedFieldsInit(string docName = "default");

    /**
     * Set Info Object in specified doc.
     *
     * Only set, should call parseAPI() to parse
     *
     * The operation will discard the parsed results of the doc forest. Should call parseAPI() again for parsing
     * @see parseAPI()
     * @param obj: the Info Object
     * @param docName: name of specified doc; default: 'default'
     * @return success set or fail
     */
    bool setInfo(BaseDataObject *obj, string docName = "default");

    /**
     * Set host string in specified doc.
     *
     * Only set, should call parseAPI() to parse
     *
     * The operation will discard the parsed results of the doc forest. Should call parseAPI() again for parsing
     * @see parseAPI()
     * @param host: the host string
     * @param docName: name of specified doc; default: 'default'
     * @return success set or fail
     */
    bool setHost(string host, string docName = "default");

    /**
     * Set base path string in specified doc.
     *
     * Only set, should call parseAPI() to parse
     *
     * The operation will discard the parsed results of the doc forest. Should call parseAPI() again for parsing
     * @see parseAPI()
     * @param basePath: the base path string
     * @param docName: name of specified doc; default: 'default'
     * @return success set or fail
     */
    bool setBasePath(string basePath, string docName = "default");

    /**
     * Remove base path field in specified doc.
     *
     * The operation will discard the parsed results of the doc forest. Should call parseAPI() again for parsing
     * @see parseAPI()
     * @param docName: name of specified doc; default: 'default'
     * @return success deleted or fail
     */
    bool removeBasePath(string docName = "default");

    /**
     * Set schemes object in specified doc.
     *
     * Only set, should call parseAPI() to parse
     *
     * The operation will discard the parsed results of the doc forest. Should call parseAPI() again for parsing
     * @see parseAPI()
     * @param basePath: the schemes object
     * @param docName: name of specified doc; default: 'default'
     * @return success set or fail
     */
    bool setSchemes(BaseDataObject *obj, string docName = "default");

    /**
     * Remove schemes object in specified doc.
     *
     * The operation will discard the parsed results of the doc forest. Should call parseAPI() again for parsing
     * @see parseAPI()
     * @param docName: name of specified doc; default: 'default'
     * @return success deleted or fail
     */
    bool removeSchemes(string docName = "default");

    /**
     * Set consumes object in specified doc.
     *
     * Only set, should call parseAPI() to parse
     *
     * The operation will discard the parsed results of the doc forest. Should call parseAPI() again for parsing
     * @see parseAPI()
     * @param basePath: the consumes object
     * @param docName: name of specified doc; default: 'default'
     * @return success set or fail
     */
    bool setConsumes(BaseDataObject *obj, string docName = "default");

    /**
     * Remove consumes object in specified doc.
     *
     * The operation will discard the parsed results of the doc forest. Should call parseAPI() again for parsing
     * @see parseAPI()
     * @param docName: name of specified doc; default: 'default'
     * @return success deleted or fail
     */
    bool removeConsumes(string docName = "default");

    /**
     * Set produces object in specified doc.
     *
     * Only set, should call parseAPI() to parse
     *
     * The operation will discard the parsed results of the doc forest. Should call parseAPI() again for parsing
     * @see parseAPI()
     * @param basePath: the produces object
     * @param docName: name of specified doc; default: 'default'
     * @return success set or fail
     */
    bool setProduces(BaseDataObject *obj, string docName = "default");

    /**
     * Remove produces object in specified doc.
     *
     * The operation will discard the parsed results of the doc forest. Should call parseAPI() again for parsing
     * @see parseAPI()
     * @param docName: name of specified doc; default: 'default'
     * @return success deleted or fail
     */
    bool removeProduces(string docName = "default");

    /**
     * Set tags object in specified doc.
     *
     * Only set, should call parseAPI() to parse
     *
     * The operation will discard the parsed results of the doc forest. Should call parseAPI() again for parsing
     * @see parseAPI()
     * @param basePath: the tags object
     * @param docName: name of specified doc; default: 'default'
     * @return success set or fail
     */
    bool setTags(BaseDataObject *obj, string docName = "default");

    /**
     * Remove tags object in specified doc.
     *
     * The operation will discard the parsed results of the doc forest. Should call parseAPI() again for parsing
     * @see parseAPI()
     * @param docName: name of specified doc ; default: 'default'
     * @return success deleted or fail
     */
    bool removeTags(string docName = "default");

    /**
     * Set external docs object in specified doc.
     *
     * Only set, should call parseAPI() to parse
     *
     * The operation will discard the parsed results of the doc forest. Should call parseAPI() again for parsing
     * @see parseAPI()
     * @param basePath: the external docs object
     * @param docName: name of specified doc; default: 'default'
     * @return success set or fail
     */
    bool setExternalDocs(BaseDataObject *obj, string docName = "default");

    /**
     * Remove external docs object in specified doc.
     *
     * The operation will discard the parsed results of the doc forest. Should call parseAPI() again for parsing
     * @see parseAPI()
     * @param docName: name of specified doc; default: 'default'
     * @return success deleted or fail
     */
    bool removeExternalDocs(string docName = "default");

    /**
     * Add a parameter which is to be shared by APIs of same path.
     *
     * The operation will discard the parsed results of the doc forest. Should call parseAPI() again for parsing
     * @see parseAPI()
     * @param obj: the parameter object
     * @param pathName: the path of APIs to share this parameter
     * @param docName: name of specified doc; default: 'default'
     * @return success add or fail
     */
    bool addAPISharedParameter(BaseDataObject *obj, string pathName, string docName = "default");

    /**
     * Remove ALL SHARED parameters of the specified API path. (only shared ones).
     *
     * The operation will discard the parsed results of the doc forest. Should call parseAPI() again for parsing
     * @see parseAPI()
     * @param pathName: the path of APIs to remove
     * @param docName: name of specified doc; default: 'default'
     * @return success remove or fail
     */
    bool removeAPISharedParameters(string pathName, string docName = "default");

    /**
     * Add an API in specified doc.
     *
     * The operation will discard the parsed results of the doc forest. Should call parseAPI() again for parsing
     * @see parseAPI()
     * @param obj: the API object
     * @param pathName: the path of the API
     * @param method: the request method of the API. Should be 'get' or 'post'.
     * @param docName: name of specified doc; default: 'default'
     * @return success add or fail
     */
    bool addAPI(BaseDataObject *obj, string pathName, string method, string docName = "default");

    /**
     * Remove an API in specified doc.
     *
     * The operation will discard the parsed results of the doc forest. Should call parseAPI() again for parsing
     * @see parseAPI()
     * @param pathName: the path of the API to remove
     * @param method: the request method of the API to remove. Should be 'get' or 'post'.
     * @param docName: name of specified doc; default: 'default'
     * @return success remove or fail
     */
    bool removeAPI(string pathName, string method, string docName = "default");

    /**
     * Add a data schema object to the 'definitions' field in specified doc.
     *
     * The operation will discard the parsed results of the doc forest. Should call parseAPI() again for parsing
     * @see parseAPI()
     * @param obj: the data schema object
     * @param name: the data schema name
     * @param docName: name of specified doc; default: 'default'
     * @return success add or fail
     */
    bool addDataSchema(BaseDataObject *obj, string name, string docName = "default");

    /**
     * Remove a data schema object from the 'parameters' field in specified doc.
     *
     * The operation will discard the parsed results of the doc forest. Should call parseAPI() again for parsing
     * @see parseAPI()
     * @param name: the data schema name
     * @param docName: name of specified doc; default: 'default'
     * @return success demove or fail
     */
    bool removeDataSchema(string name, string docName = "default");

    /**
     * Add a parameter object to the 'parameters' field in specified doc.
     *
     * The operation will discard the parsed results of the doc forest. Should call parseAPI() again for parsing
     * @see parseAPI()
     * @param obj: the parameter object
     * @param name: the parameter name
     * @param docName: name of specified doc; default: 'default'
     * @return success add or fail
     */
    bool addParameter(BaseDataObject *obj, string name, string docName = "default");

    /**
     * Remove a parameter object from 'parameters' field in specified doc.
     *
     * The operation will discard the parsed results of the doc forest. Should call parseAPI() again for parsing
     * @see parseAPI()
     * @param name: the parameter name
     * @param docName: name of specified doc; defualt: 'default'
     * @return success remove or fail
     */
    bool removeParameter(string name, string docName = "default");

    /**
     * Add a response object to the 'responses' field in specified doc.
     *
     * The operation will discard the parsed results of the doc forest. Should call parseAPI() again for parsing
     * @see parseAPI()
     * @param obj: the response object
     * @param name: the name in 'responses' field
     * @param docName: name of specified doc; default: 'default'
     * @return success add or fail
     */
    bool addResponse(BaseDataObject *obj, string name, string docName = "default");

    /**
     * Remove a response object from 'responses' field in specified doc.
     *
     * The operation will discard the parsed results of the doc forest. Should call parseAPI() again for parsing
     * @see parseAPI()
     * @param name: the name in 'responses' field
     * @param docName: name of specified doc; default: 'default'
     * @return success remove or fail
     */
    bool removeResponse(string name, string docName = "default");

/** @cond --- Parse to API --- @endcond **/

    /**
     * Parse the doc forest to inner class representation.
     *
     * It inspects all fields carefully except those related to scenario definition
     *
     * It gets fail when there's any error in error list, \
     no matter whether it's from other operation or from this parsing operation. \
     So please call cleanErrors() before calling this API.
     * @return success or fail
     */
    bool parseAPI();

/** @cond --- API Info Acquire --- @endcond **/

    /**
     * Get the Info Object.
     *
     * Should call parseAPI() before to get parsed results
     *
     * If not parsed, return NULL(None for Python) instead of the object and add an error to error list
     * @see parseAPI()
     * @return the object(derived from inner representation, may differ slightly from the input)
     */
    BaseDataObject *getInfo();

    /**
     * Get the host.
     *
     * Should call parseAPI() before to get parsed results
     *
     * If not parsed, return NULL(None for Python) instead of the object and add an error to error list
     * @see parseAPI()
     * @return the host(StringDataObject)
     */
    BaseDataObject *getHost();

    /**
     * Get the basePath.
     *
     * Should call parseAPI() before to get parsed results
     *
     * If not parsed, return NULL(None for Python) instead of the object and add an error to error list
     * @see parseAPI()
     * @return the basePath(StringDataObject)
     */
    BaseDataObject *getBasePath();

    /**
     * Get the schemes object.
     *
     * Should call parseAPI() before to get parsed results
     *
     * If not parsed, return NULL(None for Python) instead of the object and add an error to error list
     * @see parseAPI()
     * @return the object(derived from inner representation, may differ slightly from the input)
     */
    BaseDataObject *getSchemes();

    /**
     * Get the consumes object.
     *
     * Should call parseAPI() before to get parsed results
     *
     * If not parsed, return NULL(None for Python) instead of the object and add an error to error list
     * @see parseAPI()
     * @return the object(derived from inner representation, may differ slightly from the input)
     */
    BaseDataObject *getConsumes();

    /**
     * Get the produces object.
     *
     * Should call parseAPI() before to get parsed results
     *
     * If not parsed, return NULL(None for Python) instead of the object and add an error to error list
     * @see parseAPI()
     * @return the object(derived from inner representation, may differ slightly from the input)
     */
    BaseDataObject *getProduces();

    /**
     * Get the API <name, method> list(in SequenceDataObject).
     *
     * The name is the key of outer node of API Operation Object({path} in '/{path}')
     *
     * The method is either 'get' or 'post'
     *
     * Should call parseAPI() before to get parsed results
     *
     * If not parsed, return NULL(None for Python) instead of the object and add an error to error list
     * @see parseAPI()
     * @see getAPI(string name, string method)
     * @return the API <name, method> sequence
     */
    BaseDataObject *getAPINames();

    /**
     * Get the API Operation Object from name & method.
     *
     * <name, method> should be an element of getAPINames()
     *
     * Should call parseAPI() before to get parsed results
     *
     * If not parsed, return NULL(None for Python) instead of the object and add an error to error list
     * @see parseAPI()
     * @param name: the API name({path} in '/{path}')
     * @param method: the API request method; either 'get' or 'post'
     * @see getAPINames()
     * @return the object(derived from inner representation, may differ slightly from the input)
     */
    BaseDataObject *getAPI(string name, string method);

    /**
     * Get the data schema name list(key set of 'definitions' node).
     *
     * Should call parseAPI() before to get parsed results
     *
     * If not parsed, return NULL(None for Python) instead of the object and add an error to error list
     * @see parseAPI()
     * @see getDataSchema(string name)
     * @return the data schema name(in string type) sequence
     */
    BaseDataObject *getDataSchemaNames();

    /**
     * Get the data schema object from name.
     *
     * name should be an element of getDataSchemaNames()
     *
     * Should call parseAPI() before to get parsed results
     *
     * If not parsed, return NULL(None for Python) instead of the object and add an error to error list
     * @see parseAPI()
     * @see getDataSchemaNames()
     * @param name: the data schema name
     * @return the object(derived from inner representation, may differ slightly from the input)
     */
    BaseDataObject *getDataSchema(string name);

    /**
     * Generate a data object sample using the random policy from an existing data schema.
     *
     * Data schema is specified by name in 'definitions', and the name should be an element of getDataSchemaNames()
     *
     * Should call parseAPI() before to get parsed results
     *
     * If not parsed, return NULL(None for Python) instead of the object and add an error to error list
     * @see parseAPI()
     * @see getDataSchemaNames()
     * @param name: the data schema name
     * @return the object
     */
    BaseDataObject *generateRandomDataFromDataSchema(string name);

    /**
     * Verify whether the data object conforms to the data schema.
     *
     * Data schema is specified by name in 'definitions', and the name should be an element of getDataSchemaNames()
     *
     * Should call parseAPI() before to get results. Otherwise return false whatever
     * @see parseAPI()
     * @see getDataSchemaNames()
     * @param data: the data object to verify
     * @param name: the data schema name
     * @return legal or not
     */
    bool verifyDataByDataSchema(BaseDataObject *data, string name);

    /**
     * Get the parameter name list(key set of 'parameters' node).
     *
     * Should call parseAPI() before to get parsed results
     *
     * If not parsed, return NULL(None for Python) instead of the object and add an error to error list
     * @see parseAPI()
     * @see getParameter(string name)
     * @return the parameter name(in string type) sequence
     */
    BaseDataObject *getParameterNames();

    /**
     * Get the parameter object from name.
     *
     * name should be an element of getParameterNames()
     *
     * Should call parseAPI() before to get parsed results
     *
     * If not parsed, return NULL(None for Python) instead of the object and add an error to error list
     * @see parseAPI()
     * @see getParameterNames()
     * @param name: the parameter name
     * @return the object(derived from inner representation, may differ slightly from the input)
     */
    BaseDataObject *getParameter(string name);

    /**
     * Get the response name list(key set of 'responses' node).
     *
     * Should call parseAPI() before to get parsed results
     *
     * If not parsed, return NULL(None for Python) instead of the object and add an error to error list
     * @see parseAPI()
     * @see getResponse(string name)
     * @return the response name(in string type) sequence
     */
    BaseDataObject *getResponseNames();

    /**
     * Get the response object from name.
     *
     * name should be an element of getResponsesNames()
     *
     * Should call parseAPI() before to get parsed results
     *
     * If not parsed, return NULL(None for Python) instead of the object and add an error to error list
     * @see parseAPI()
     * @see getResponseNames()
     * @param name: the response name
     * @return the object(derived from inner representation, may differ slightly from the input)
     */
    BaseDataObject *getResponse(string name);

    /**
     * Get the Tags Object.
     *
     * Should call parseAPI() before to get parsed results
     *
     * If not parsed, return NULL(None for Python) instead of the object and add an error to error list
     * @see parseAPI()
     * @return the object(derived from inner representation, may differ slightly from the input)
     */
    BaseDataObject *getTags();

    /**
     * Get the External Docs Object.
     *
     * Should call parseAPI() before to get parsed results
     *
     * If not parsed, return NULL(None for Python) instead of the object and add an error to error list
     * @see parseAPI()
     * @return the object(derived from inner representation, may differ slightly from the input)
     */
    BaseDataObject *getExternalDocs();

/** @cond --- Scenario Subitem Editor --- @endcond **/

/** @cond --- Parse to Scenario --- @endcond **/

    /**
     * Parse the doc forest to inner class representation.
     *
     * It inspects all scenario-related fields carefully.
     *
     * It gets fail when there's any error in error list, \
     no matter whether it's from other operation or from this parsing operation. \
     So please call cleanErrors() before calling this API.
     *
     * Should call parseAPI() before. Then can you parse the scenario.
     *
     * @see parseAPI()
     * @return success or fail
     */
    bool parseScenario();


/** @cond --- Scenario Info Acquire --- @endcond **/

    /**
     * Get the scenario name list.
     *
     * Should call parseScenario() before to get parsed results
     *
     * If not parsed, return NULL(None for Python) instead of the object and add an error to error list
     * @see parseScenario()
     * @see getScenario(string name)
     * @return the scenario name(in string type) sequence
     */
    BaseDataObject *getScenarioNames();

    /**
     * Get the parameter object from name.
     *
     * name should be an element of getParameterNames()
     *
     * Should call parseScenario() before to get parsed results
     *
     * If not parsed, return NULL(None for Python) instead of the object and add an error to error list
     * @see parseScenario()
     * @see getScenarioNames()
     * @param name: the scenario name
     * @return the object (derived from inner representation, may differ slightly from the input)
     */
    BaseDataObject *getScenario(string name);

/** @cond --- Config Subitem Editor --- @endcond **/

/** @cond --- Parse to Config --- @endcond **/

    /**
     * Parse the doc forest to inner class representation.
     *
     * It inspects "x-config" field carefully.
     *
     * It gets fail when there's any error in error list, \
     no matter whether it's from other operation or from this parsing operation. \
     So please call cleanErrors() before calling this API.
     *
     * Should call parseScenario() before. Then can you parse the configs with existing API and scenario definitions.
     *
     * @see parseAPI()
     * @see parseConfig()
     * @return success or fail
     */
    bool parseConfig();

/** @cond --- Config Info Acquire --- @endcond **/

    /**
     * Get the TestConfigObject
     *
     * Should call parseConfig() before to get parsed results
     *
     * If not parsed, return NULL(None for Python) instead of the object and add an error to error list
     * @see parseConfig()
     * @return the object (derived from inner representation, may differ slightly from the input)
     */
    BaseDataObject *getConfig();

/** @cond --- Run Single API --- @endcond **/

    /**
     * Run a single API
     *
     * If it runs successfully,
     *  the return data is {response: response type (string), data: response data}
     * Otherwise,
     *  the return data is NULL
     *
     * @param api: the API Name
     * @param method: the method
     * @return the object or NULL (see description above)
     */
    BaseDataObject *runSingleAPI(string api, string method);

    /**
     * The same API interface for Ali API
     * @param api: the API Name
     * @param method: the method
     * @param secretKey: Ali Secret Key
     * @return the object or NULL (see description above)
     */
    BaseDataObject *runSingleAPIforAli(string api, string method, string secretKey);


/** @cond --- Run Scenario ---- @endcond **/

    bool runScenario();

/** @cond --- Errors & Report --- @endcond **/

    SequenceDataObject *getRuntimeErrors();

    SequenceDataObject *getReport();

/** @cond --- Locals --- @endcond **/
    /**
     * Back to DOC_TREE stage, to prepare for reparsing to class representation
     */
    static void cleanToDocStage();

    /**
     * Back to API stage, to prepare for reparsing scenario fields
     */
    static void cleanToAPIStage();

    /**
     * Back to Scenario stage, to prepare for reparsing config fields
     */
    static void cleanToScenarioStage();

#endif //VPARSER_INTERFACE_H
