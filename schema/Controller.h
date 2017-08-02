//
// Created by lly on 28/05/2017.
//

#ifndef VPARSER_CONTROLLER_H
#define VPARSER_CONTROLLER_H

#include "InfoObject.h"
#include "SchemesObject.h"
#include "EnumObject.h"
#include "APIPool.h"
#include "DataSchemaPool.h"
#include "ParameterPool.h"
#include "ResponsePool.h"
#include "TagsObject.h"
#include "ExternalDocObject.h"
#include "ScenarioPool.h"
#include <string>

using namespace std;

/**
 * Management of the class representation of input data
 * Transform doc representation to class
 */
class Controller {

public:
    /** Info Object **/
    InfoObject *infoObject;
    /** host(Required) **/
    string host;
    /** basePath(Optional, Default: "") **/
    string basePath;
    /** schemes([String], Optional: empty array) **/
    SchemesObject *schemes;
    /** consumes([String], Optional: empty array) **/
    EnumObject *consumes;
    /** produces([String], Optional: empty array) **/
    EnumObject *produces;

    /** paths(Required) **/
    APIPool *paths;
    /** definitions(Optional) **/
    DataSchemaPool *definitions;
    /** parameters(Optional) **/
    ParameterPool *parameters;
    /** responses(Optional) **/
    ResponsePool *responses;
    /** x-scenario(Optional) **/
    ScenarioPool *scenario;

    /** tags([Tag Object], Optional) **/
    TagsObject *tags;
    /** externalDocs(External Documentation Object, Optional) **/
    ExternalDocObject *externalDocs;


    Controller();
    ~Controller();

    bool work();

    void printTo(std::ostream &os);
};


#endif //VPARSER_CONTROLLER_H
