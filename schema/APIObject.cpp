//
// Created by lly on 26/07/2017.
//

#include "APIObject.h"
#include "api/APIConstraintObject.h"

APIObject::APIObject() {

}

APIObject::~APIObject() {

}

BaseDataObject *APIObject::toDataObject() {

}

bool APIObject::create(string filePath, DocObjectElement *ele, SchemesObject *rootSchemes,
                       vector<ParameterObject *> *commonParams, ParameterPool *paramPool, ResponsePool *responsePool) {

}