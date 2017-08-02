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

enum InterfaceState {
    DOC_TREE, API_PARSED, SCENARIO_PARSED
};

extern Controller *controller;
extern InterfaceState state;

/** --- Init --- **/
    /**
     * Interface Initialization
     * Should be executed whatever
     */
    void init();

/** --- Error Related --- **/
    /**
     * Get errors array in Data Object format
     * @return the errors array
     */
    SequenceDataObject *getErrors();

    /**
     * Clean the inner errors array
     * Empty error array will be obtained after it's called
     */
    void cleanErrors();

/** --- DocNode Editor --- **/
    /**
     * add a XML/YAML doc to the doc forest
     * Format is automatically deducted from format suffix
     * @param filePath: the file path
     * @param docName: the name in doc forest, if empty, use filePath
     * @return bool: successfully parsed or not
     */
    bool addDocFromFile(string filePath, string docName = "");

    /**
     * add a YAML doc to the doc forest
     * @param filePath: the file path
     * @param docName: the name in doc forest, if empty, use filePath
     * @return bool: successfully parsed or not
     */
    bool addYAMLDocFromFile(string filePath, string docName = "");

    /**
     * add a XML doc to the doc forest
     * @param filePath: the file path
     * @param docName: the name in doc forest, if empty, use filePath
     * @return bool: successfully parsed or not
     */
    bool addXMLDocFromFile(string filePath, string docName = "");

    /**
     * Delete a doc in the doc forest
     * @param docName: the name in doc forest
     * @return bool: successfully removed or not
     */
    bool removeDoc(string docName);

    /**
     * Get the array list of all doc names in doc forest
     * @return the doc name array
     */
    SequenceDataObject *getDocList();

    /**
     * Save the specified doc in doc forest to YAML file
     * @param destFileName: the output file path
     * @param docName: the doc name in doc forest
     * @return
     */
    bool saveAsYAML(string destFileName, string docName = "default");

    /**
    * Save the specified doc in doc forest to XML file
    * @param destFileName: the output file path
    * @param docName: the doc name in doc forest
    * @return
    */
    bool saveAsXML(string destFileName, string docName = "default");

/** --- API Subitem Editor --- **/

    bool addDoc(BaseDataObject *obj, string docName = "default");

    /**
     * Init 'swagger: 2.0' & 'x-vee: 2.0 AS' to the specified doc
     * @param docName: specified doc; default: 'default'
     * @return success or fail
     */
    bool reservedFieldsInit(string docName = "default");

    bool setInfo(BaseDataObject *obj, string docName = "default");

    bool setHost(string host, string docName = "default");

    bool setBasePath(string basePath, string docName = "default");

    bool removeBasePath(string docName = "default");

    bool setSchemes(BaseDataObject *obj, string docName = "default");

    bool removeSchemes(string docName = "default");

    bool setConsumes(BaseDataObject *obj, string docName = "default");

    bool removeConsumes(string docName = "default");

    bool setProduces(BaseDataObject *obj, string docName = "default");

    bool removeProduces(string docName = "default");

    bool setTags(BaseDataObject *obj, string docName = "default");

    bool removeTags(string docName = "default");

    bool setExternalDocs(BaseDataObject *obj, string docName = "default");

    bool removeExternalDocs(string docName = "default");

    bool addAPISharedParameter(BaseDataObject *obj, string pathName, string docName = "default");

    bool removeAPISharedParameters(string pathName, string docName = "default");

    bool addAPI(BaseDataObject *obj, string pathName, string method, string docName = "default");

    bool removeAPI(string pathName, string method, string docName = "default");

    bool addDataSchema(BaseDataObject *obj, string name, string docName = "default");

    bool removeDataSchema(string name, string docName = "default");

    bool addParameter(BaseDataObject *obj, string name, string docName = "default");

    bool removeParameter(string name, string docName = "default");

    bool addResponse(BaseDataObject *obj, string name, string docName = "default");

    bool removeResponse(string name, string docName = "default");

/** --- Parse to API --- **/
    /**
     * Parse the doc forest to inner class representation.
     * It inspects all fields carefully except those related to scenario definition
     * @return success or fail
     */
    bool parseAPI();

/** --- API Info Acquire --- **/

    BaseDataObject *getInfo();

    BaseDataObject *getHost();

    BaseDataObject *getBasePath();

    BaseDataObject *getSchemes();

    BaseDataObject *getConsumes();

    BaseDataObject *getProduces();

    BaseDataObject *getAPINames();

    BaseDataObject *getAPI(string name, string method);

    BaseDataObject *getDataSchemaNames();

    BaseDataObject *getDataSchema(string name);

    BaseDataObject *generateRandomDataFromDataSchema(string name);

    bool verifyDataByDataSchema(BaseDataObject *data, string name);

    BaseDataObject *getParameterNames();

    BaseDataObject *getParameter(string name);

    BaseDataObject *getResponseNames();

    BaseDataObject *getResponse(string name);

    BaseDataObject *getTags();

    BaseDataObject *getExternalDocs();

/** --- Scenario Subitem Editor --- **/

/** --- Parse to Scenario --- **/


/** --- Scenario Info Acquire --- **/


/** --- Run Single API --- **/


/** --- Run Scenario ---- **/



/** --- Locals --- **/
    /**
     * Back to DOC_TREE stage, to prepare for reparsing to class representation
     */
    static void cleanToDocStage();

#endif //VPARSER_INTERFACE_H
