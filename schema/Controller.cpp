//
// Created by lly on 28/05/2017.
//

#include "Controller.h"
#include "../doc/DocElement.h"
#include "../error/Error.h"
#include "../error/InvalidRootNodeError.h"
#include "../doc/DocScalarElement.h"
#include "../error/InvalidXVeeSymbolError.h"
#include "../error/InvalidSwaggerSymbolError.h"
#include "../error/FieldInvalidError.h"
#include "../error/RootFieldDuplicateError.h"
#include "../doc/DocSequenceElement.h"
#include "../error/FieldMissError.h"

Controller::Controller() {
    srand((unsigned int)time(0));
    definitions = new DataSchemaPool();
    parameters = new ParameterPool(definitions);
    responses = new ResponsePool(definitions);
    paths = new APIPool(parameters, responses);
}

Controller::~Controller() {
    if (infoObject)
        delete infoObject;
    if (schemes)
        delete schemes;
    if (consumes)
        delete consumes;
    if (produces)
        delete produces;
    if (tags)
        delete tags;
    if (externalDocs)
        delete externalDocs;

    delete definitions;
    delete parameters;
    delete responses;
    delete paths;
}

void Controller::work() {
    bool infoFind = false;
    bool hostFind = false;
    bool basePathFind = false;
    bool schemesFind = false;
    bool consumesFind = false;
    bool producesFind = false;
    bool tagsFind = false;
    bool externalDocsFind = false;

    for (map<string, DocElement*>::iterator ite=DocElement::docs.begin(); ite != DocElement::docs.end(); ++ite) {
        DocElement *priDocRoot = ite->second;
        if (priDocRoot->type == DOC_OBJECT) {
            DocObjectElement *docRoot = (DocObjectElement*)priDocRoot;
            /** x-vee & swagger should appear in each doc **/
            /** x-vee check **/
            bool xveeValid = false;
            DocElement *xveeEle = docRoot->get("x-vee");
            if ((xveeEle != NULL) && (xveeEle->type == DOC_SCALAR)) {
                if (((DocScalarElement*)xveeEle)->getValue() == "2.0 AS")
                    xveeValid = true;
            }
            if (!xveeValid) {
                Error::addError(new InvalidXVeeSymbolError(ite->first, priDocRoot->line, priDocRoot->col));
                return;
            }
            /** swagger check **/
            bool swaggerValid = false;
            DocElement *swaggerEle = docRoot->get("swagger");
            if ((swaggerEle != NULL) && (swaggerEle->type == DOC_SCALAR)) {
                if (((DocScalarElement*)swaggerEle)->getValue() == "2.0")
                    swaggerValid = true;
            }
            if (!swaggerValid) {
                Error::addError(new InvalidSwaggerSymbolError(ite->first, priDocRoot->line, priDocRoot->col));
                return;
            }
            /** Below this line are fields which can only appear in one doc **/
            /** Info Object **/
            DocElement *infoRoot = docRoot->get("info");
            if (infoRoot != NULL) {
                if (infoRoot->type != DOC_OBJECT) {
                    Error::addError(new FieldInvalidError(ite->first, infoRoot->line, infoRoot->col, "info", infoRoot->type, DOC_OBJECT));
                    return;
                } else {
                    if (infoFind) {
                        Error::addError(new RootFieldDuplicateError(ite->first, infoRoot->line, infoRoot->col, "info"));
                        return;
                    } else {
                        infoFind = true;
                        infoObject = InfoObjectFactory::create(ite->first, (DocObjectElement*)infoRoot);
                        if (infoObject == NULL)
                            return;
                    }
                }
            }
            /** host **/
            DocElement *hostEle = docRoot->get("host");
            if (hostEle != NULL) {
                if (hostEle->type != DOC_SCALAR) {
                    Error::addError(new FieldInvalidError(ite->first, hostEle->line, hostEle->col, "host", hostEle->type, DOC_SCALAR));
                    return;
                } else {
                    if (hostFind) {
                        Error::addError(new RootFieldDuplicateError(ite->first, hostEle->line, hostEle->col, "host"));
                        return;
                    } else {
                        hostFind = true;
                        host = ((DocScalarElement*)hostEle)->getValue();
                    }
                }
            }
            /** basePath **/
            DocElement *basePathEle = docRoot->get("basePath");
            if (basePathEle != NULL) {
                if (basePathEle->type != DOC_SCALAR) {
                    Error::addError(new FieldInvalidError(ite->first, basePathEle->line, basePathEle->col, "basePath", basePathEle->type, DOC_SCALAR));
                    return;
                } else {
                    if (basePathFind) {
                        Error::addError(new RootFieldDuplicateError(ite->first, basePathEle->line, basePathEle->col, "basePath"));
                        return;
                    } else {
                        basePathFind = true;
                        basePath = ((DocScalarElement*)basePathEle)->getValue();
                    }
                }
            }
            /** schemes **/
            DocElement *schemesEle = docRoot->get("schemes");
            if (schemesEle != NULL) {
                if (schemesEle->type != DOC_SEQUENCE) {
                    Error::addError(new FieldInvalidError(ite->first, schemesEle->line, schemesEle->col, "schemes", schemesEle->type, DOC_SEQUENCE));
                    return;
                } else {
                    if (schemesFind) {
                        Error::addError(new RootFieldDuplicateError(ite->first, schemesEle->line, schemesEle->col, "schemes"));
                        return;
                    } else {
                        schemesFind = true;
                        schemes = SchemesObjectFactory::create(ite->first, (DocSequenceElement*)schemesEle);
                        if (schemes == NULL)
                            return;
                    }
                }
            }
            /** consumes **/
            DocElement *consumesEle = docRoot->get("consumes");
            if (consumesEle != NULL) {
                if (consumesEle->type != DOC_SEQUENCE) {
                    Error::addError(new FieldInvalidError(ite->first, consumesEle->line, consumesEle->col, "consumes", consumesEle->type, DOC_SEQUENCE));
                    return;
                } else {
                    if (consumesFind) {
                        Error::addError(new RootFieldDuplicateError(ite->first, consumesEle->line, consumesEle->col, "consumes"));
                        return;
                    } else {
                        consumesFind = true;
                        consumes = EnumObjectFactory::create(ite->first, "consumes", (DocSequenceElement*)consumesEle);
                        if (consumes == NULL)
                            return;
                    }
                }
            }
            /** produces **/
            DocElement *producesEle = docRoot->get("produces");
            if (producesEle != NULL) {
                if (producesEle->type != DOC_SEQUENCE) {
                    Error::addError(new FieldInvalidError(ite->first, producesEle->line, producesEle->col, "produces", producesEle->type, DOC_SEQUENCE));
                    return;
                } else {
                    if (producesFind) {
                        Error::addError(new RootFieldDuplicateError(ite->first, producesEle->line, producesEle->col, "produces"));
                        return;
                    } else {
                        producesFind = true;
                        produces = EnumObjectFactory::create(ite->first, "produces", (DocSequenceElement*)producesEle);
                        if (produces == NULL)
                            return;
                    }
                }
            }
            /** tags **/
            DocElement *tagsEle = docRoot->get("tags");
            if (tagsEle != NULL) {
                if (tagsEle->type != DOC_SEQUENCE) {
                    Error::addError(new FieldInvalidError(ite->first, tagsEle->line, tagsEle->col, "tags", tagsEle->type, DOC_SEQUENCE));
                    return;
                } else {
                    if (tagsFind) {
                        Error::addError(new RootFieldDuplicateError(ite->first, tagsEle->line, tagsEle->col, "tags"));
                        return;
                    } else {
                        tagsFind = true;
                        tags = TagsObjectFactory::create(ite->first, (DocSequenceElement*)tagsEle);
                        if (tags == NULL)
                            return;
                    }
                }
            }
            /** externalDocs **/
            DocElement *externalDocsEle = docRoot->get("externalDocs");
            if (externalDocsEle != NULL) {
                if (externalDocsEle->type != DOC_OBJECT) {
                    Error::addError(new FieldInvalidError(ite->first, externalDocsEle->line, externalDocsEle->col, "externalDocs", externalDocsEle->type, DOC_OBJECT));
                    return;
                } else {
                    if (externalDocsFind) {
                        Error::addError(new RootFieldDuplicateError(ite->first, externalDocsEle->line, externalDocsEle->col, "externalDocs"));
                        return;
                    } else {
                        externalDocsFind = true;
                        externalDocs = ExternalDocObjectFactory::create(ite->first, "externalDocs", (DocObjectElement*)externalDocsEle);
                        if (externalDocs == NULL)
                            return;
                    }
                }
            }
            /** Below this line are fields which can spread over several docs **/
            /** definitions **/
            DocElement *definitionsEle = docRoot->get("definitions");
            if (definitionsEle != NULL) {
                if (definitionsEle->type != DOC_OBJECT) {
                    Error::addError(new FieldInvalidError(ite->first, definitionsEle->line, definitionsEle->col, "definitions", definitionsEle->type, DOC_OBJECT));
                    return;
                } else {
                    DocObjectElement *o = (DocObjectElement*)definitionsEle;
                    map<string, DocElement*> *m = o->getMemberMap();
                    for (map<string, DocElement*>::iterator iite = m->begin(); iite != m->end(); ++iite) {
                        if (definitions->parseDataSchema(ite->first, iite->second, iite->first) == NULL)
                            return;
                    }
                }
            }

            /** parameters **/
            DocElement *paramEle = docRoot->get("parameters");
            if (paramEle != NULL) {
                if (paramEle->type != DOC_OBJECT) {
                    Error::addError(new FieldInvalidError(ite->first, paramEle->line, paramEle->col, "parameters", paramEle->type, DOC_OBJECT));
                    return;
                } else {
                    DocObjectElement *o = (DocObjectElement*)paramEle;
                    map<string, DocElement*> *m = o->getMemberMap();
                    for (map<string, DocElement*>::iterator iite = m->begin(); iite != m->end(); ++iite) {
                        if (parameters->parseParameter(ite->first, iite->second, iite->first) == NULL)
                            return;
                    }
                }
            }

            /** responses **/
            DocElement *responseEle = docRoot->get("responses");
            if (responseEle != NULL) {
                if (responseEle->type != DOC_OBJECT) {
                    Error::addError(new FieldInvalidError(ite->first, responseEle->line, responseEle->col, "responses", responseEle->type, DOC_OBJECT));
                    return;
                } else {
                    DocObjectElement *o = (DocObjectElement*)responseEle;
                    map<string, DocElement*> *m = o->getMemberMap();
                    for (map<string, DocElement*>::iterator iite = m->begin(); iite != m->end(); ++iite) {
                        const string &paramName = iite->first;
                        DocElement *nowEle = iite->second;
                        if (nowEle->type != DOC_OBJECT) {
                            Error::addError(new FieldInvalidError(ite->first, nowEle->line, nowEle->col, "responses", nowEle->type, DOC_OBJECT));
                            return;
                        } else {
                            DocObjectElement *oEle = (DocObjectElement*)nowEle;
                            if (oEle->get("name")) {
                                // must be Response Extension Object
                                if (responses->parseResponseExtension(ite->first, oEle, paramName) == NULL)
                                    return;
                            } else {
                                // must be Response Object
                                if (responses->parseResponse(ite->first, oEle, paramName) == NULL)
                                    return;
                            }
                        }
                    }
                }
            }

            /** paths **/
            DocElement *pathsEle = docRoot->get("paths");
            if (pathsEle != NULL) {
                if (pathsEle->type != DOC_OBJECT) {
                    Error::addError(new FieldInvalidError(ite->first, pathsEle->line, pathsEle->col, "paths", pathsEle->type, DOC_OBJECT));
                    return;
                } else {
                    DocObjectElement *o_pathsEle = (DocObjectElement*)pathsEle;
                    map<string, DocElement*> *memberMap = o_pathsEle->getMemberMap();

                    for (map<string, DocElement*>::iterator iite = memberMap->begin();
                            iite != memberMap->end();
                            ++iite) {
                        string key = ite->first;
                        DocElement *item = ite->second;

                        if (item == NULL) {
                            Error::addError(new FieldMissError(ite->first, pathsEle->line, pathsEle->col, "paths.value"));
                            return;
                        }
                        if ((key.length() <= 0) || (key[0] != '/')) {
                            Error::addError(new FieldIllegalError(ite->first, item->line, item->col, "paths.key"));
                            return;
                        }
                        if (item->type != DOC_OBJECT) {
                            Error::addError(new FieldInvalidError(ite->first, item->line, item->col, "paths.value", item->type, DOC_OBJECT));
                            return;
                        }

                        DocObjectElement *o_itemEle = (DocObjectElement*)item;

                        vector<ParameterObject*> *paramsVec = new vector<ParameterObject*>();
                        /** parameter under paths **/
                        DocElement *p_paramEle = o_itemEle->get("parameter");
                        if (p_paramEle != NULL) {
                            if (p_paramEle->type != DOC_SEQUENCE) {
                                Error::addError(new FieldInvalidError(ite->first, p_paramEle->line, p_paramEle->col, "paths.parameter", p_paramEle->type, DOC_SEQUENCE));
                                paramsVec->clear();
                                delete paramsVec;
                                return;
                            } else {
                                DocSequenceElement *s_p_paramEle = (DocSequenceElement*)p_paramEle;
                                int len = s_p_paramEle->getLength();
                                for (int i=0; i<len; ++i) {
                                    DocElement *param_item = s_p_paramEle->get(i);
                                    ParameterObject *o = parameters->parseParameter(ite->first, param_item);
                                    if (o == NULL) {
                                        paramsVec->clear();
                                        delete paramsVec;
                                        return;
                                    }
                                    paramsVec->push_back(o);
                                }
                            }
                        }

                        /** get **/
                        DocElement *getEle = o_itemEle->get("get");
                        if (getEle != NULL) {
                            APIObject *get_o = paths->parseAPI(ite->first, getEle, key.substr(1), GET, schemes,
                                                               paramsVec);
                            if (get_o == NULL) {
                                paramsVec->clear();
                                delete paramsVec;
                                return;
                            }
                        }

                        /** post **/
                        DocElement *postEle = o_itemEle->get("post");
                        if (postEle != NULL) {
                            APIObject *post_o = paths->parseAPI(ite->first, postEle, key.substr(1), POST, schemes,
                                                               paramsVec);
                            if (post_o == NULL) {
                                paramsVec->clear();
                                delete paramsVec;
                                return;
                            }
                        }

                        paramsVec->clear();
                        delete paramsVec;
                    }
                }
            }

            /** x-scenario (working on...) **/

        } else {
            /** Doc root node is NOT of DocObject type */
            Error::addError(new InvalidRootNodeError(ite->first, priDocRoot->line, priDocRoot->col));
            return;
        }
    }

    // Required
    if (!infoFind) {
        Error::addError(new FieldMissError("/", 1, 1, "info"));
        return;
    }
    // Required
    if (!hostFind) {
        Error::addError(new FieldMissError("/", 1, 1, "host"));
        return;
    }
    // Below are optional
    if (!basePathFind) {
        basePath = "";
    }
    if (!schemesFind) {
        schemes = NULL;
    }
    if (!consumesFind) {
        consumes = NULL;
    }
    if (!producesFind) {
        produces = NULL;
    }
    if (!tagsFind) {
        tags = NULL;
    }
    if (!externalDocsFind) {
        externalDocs = NULL;
    }
}

void Controller::printTo(std::ostream &os) {
    os << "----- General Info -----" << endl;
    os << "Info:" << endl;
    os << "  title: " << infoObject->title << endl;
    if (infoObject->hasDescription)
        os << "  description: " << infoObject->description << endl;
    if (infoObject->hasTermsOfService)
        os << "  termsOfService: " << infoObject->termsOfService << endl;
    os << "  version: " << infoObject->version << endl;
    if (infoObject->hasContact) {
        os << "Contact:" << endl;
        if (infoObject->contact.hasName)
            os << "  name: " << infoObject->contact.name << endl;
        if (infoObject->contact.hasUrl)
            os << "  url: " << infoObject->contact.url << endl;
        if (infoObject->contact.hasEmail)
            os << "  email: " << infoObject->contact.email << endl;
    }
    if (infoObject->hasLicense) {
        os << "License:" << endl;
        os << "  name: " << infoObject->license.name << endl;
        if (infoObject->license.hasUrl)
            os << "  url: " << infoObject->license.url << endl;
    }
    os << "Host: " << host << endl;
    os << "BasePath: " << basePath << endl;
    if (schemes) {
        os << "Schemes:";
        if (schemes->http) os << " [http]";
        if (schemes->https) os << " [https]";
        os << endl;
    }
    if (consumes) {
        os << "Consumes:" << endl;
        for (int i=0; i<consumes->vec.size(); ++i)
            os << "  " << consumes->vec[i] << endl;
    }
    if (produces) {
        os << "Produces:" << endl;
        for (int i=0; i<produces->vec.size(); ++i)
            os << "  " << produces->vec[i] << endl;
    }
    if (tags) {
        os << "Tags:" << endl;
        for (int i=0; i<tags->vec.size(); ++i) {
            os << "  " << tags->vec[i].name << endl;
            if (tags->vec[i].hasDescription)
                os << "    Description: " << tags->vec[i].description << endl;
            if (tags->vec[i].hasExternalDocObject)
                os << "    ExternalDoc: " << tags->vec[i].externalDocObject.url << endl;
        }
    }
    if (externalDocs) {
        os << "External Docs:" << endl;
        os << "  " << externalDocs->url << endl;
        if (externalDocs->hasDescription)
            os << "  Description: " << externalDocs->description << endl;
    }
}
