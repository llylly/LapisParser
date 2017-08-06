//
// Created by lly on 29/07/2017.
//

#include "Interface.h"

Controller *controller = NULL;
InterfaceState state = InterfaceState::DOC_TREE;


/** --- Init --- **/
void init() {
    cleanToDocStage();
    DocElement::docs.clear();
    state = DOC_TREE;
}

/** --- Error Related --- **/
SequenceDataObject *getErrors() {
    vector<Error*> *errorVec = Error::getErrors();
    SequenceDataObject *ans = new SequenceDataObject();
    if (errorVec != NULL) {
        for (vector<Error*>::iterator ite = errorVec->begin();
                ite != errorVec->end(); ++ite) {
            Error &item = **ite;
            ObjectDataObject *now = new ObjectDataObject();
            (*now)["errno"] = new IntegerDataObject((int)item.errorType);
            (*now)["fileName"] = new StringDataObject(item.fileName);
            (*now)["line"] = new IntegerDataObject(item.line);
            (*now)["col"] = new IntegerDataObject(item.col);
            (*now)["msg"] = new StringDataObject(item.msg);
            ans->push(now);
        }
    }
    return ans;
}

void cleanErrors() {
    vector<Error*> *errorVec = Error::getErrors();
    for (vector<Error*>::iterator ite = errorVec->begin();
            ite != errorVec->end();
            ++ite)
        delete *ite;
    errorVec->clear();
}

/** --- DocNode Editor --- **/
bool addDocFromFile(string filePath, string docName) {
    if (access(filePath.c_str(), F_OK | R_OK) != 0) {
        Error::addError(
                new FileNotExistError(filePath)
        );
        return false;
    }
    string suffix = filePath.substr(filePath.length() - 4);
    if ((suffix == ".xml") || (suffix == ".XML")) {
        return addXMLDocFromFile(filePath, docName);
    } else {
        return addYAMLDocFromFile(filePath, docName);
    }
}

bool addYAMLDocFromFile(string filePath, string docName) {
    if (access(filePath.c_str(), F_OK | R_OK) != 0) {
        Error::addError(
                new FileNotExistError(filePath)
        );
        return false;
    }
    DocElement *nowDoc = YAMLAdapter::parseDoc(filePath.c_str());
    if (nowDoc == NULL)
        return false;
    if (docName == "")
        docName = filePath;

    cleanToDocStage();
    if (DocElement::docs.count(docName))
        delete DocElement::docs[docName];

    DocElement::docs[docName] = nowDoc;
    return true;
}

bool addXMLDocFromFile(string filePath, string docName) {
    if (access(filePath.c_str(), F_OK | R_OK) != 0) {
        Error::addError(
                new FileNotExistError(filePath)
        );
        return false;
    }
    DocElement *nowDoc = XMLAdapter::parseDoc(filePath.c_str());
    if (nowDoc == NULL)
        return false;
    if (docName == "")
        docName = filePath;

    cleanToDocStage();
    if (DocElement::docs.count(docName))
        delete DocElement::docs[docName];

    DocElement::docs[docName] = nowDoc;
    return true;
}

bool addDoc(BaseDataObject *obj, string docName) {
    if (obj == NULL) return false;
    cleanToDocStage();
    map<string, DocElement *> &docMap = DocElement::docs;
    if (docMap.count(docName) != 0) {
        delete docMap[docName];
        docMap.erase(docName);
    }
    docMap[docName] = DataObjectAdapter::toDocElement(obj);
    return true;
}

BaseDataObject *getDoc(string docName) {
    cleanToDocStage();
    map<string, DocElement *> &docMap = DocElement::docs;
    if (docMap.count(docName) != 0) {
        return DataObjectAdapter::defaultFromDocElement(docMap[docName]);
    } else
        return NULL;
}

bool removeDoc(string docName) {
    map<string, DocElement*> &docMap = DocElement::docs;
    if (docMap.count(docName) == 0)
        return false;
    else {
        delete docMap[docName];
        docMap.erase(docName);
        return true;
    }
}

SequenceDataObject *getDocList() {
    SequenceDataObject *seq = new SequenceDataObject();
    map<string, DocElement*> &docMap = DocElement::docs;
    for (map<string, DocElement*>::iterator ite = docMap.begin();
            ite != docMap.end();
            ++ite)
        seq->push(new StringDataObject(ite->first));
    return seq;
}

bool saveAsYAML(string destFileName, string docName) {
    map<string, DocElement*> &docMap = DocElement::docs;
    if (docMap.count(docName) > 0) {
        DocElement *ele = docMap[docName];
        return (YAMLAdapter::saveFile(destFileName.c_str(), ele) == 0);
    } else {
        Error::addError(
                new DocNodeNotExistError(docName)
        );
        return false;
    }
}

bool saveAsXML(string destFileName, string docName) {
    map<string, DocElement*> &docMap = DocElement::docs;
    if (docMap.count(docName) > 0) {
        DocElement *ele = docMap[docName];
        return (XMLAdapter::saveFile(destFileName.c_str(), ele) == 0);
    } else {
        Error::addError(
                new DocNodeNotExistError(docName)
        );
        return false;
    }
}

/** --- API Subitem Editor --- **/

bool reservedFieldsInit(string docName) {
    cleanToDocStage();
    map<string, DocElement *> &docMap = DocElement::docs;
    if (docMap.count(docName) == 0)
        docMap[docName] = new DocObjectElement(1, 1, 0);
    if (docMap[docName]->type != DOC_OBJECT) {
        Error::addError(
                new FieldInvalidError(docName, 1, 1, "/", docMap[docName]->type, DOC_OBJECT)
        );
        return false;
    }
    DocObjectElement *ro = (DocObjectElement *) docMap[docName];

    if (ro->get("swagger") != NULL)
        delete ro->get("swagger");
    ro->add("swagger", new DocScalarElement(1, 1, "2.0"));
    if (ro->get("x-vee") != NULL)
        delete ro->get("x-vee");
    ro->add("x-vee", new DocScalarElement(1, 1, "2.0 AS"));
    return true;
}

bool setInfo(BaseDataObject *obj, string docName) {
    if (obj == NULL) return false;
    cleanToDocStage();
    map<string, DocElement *> &docMap = DocElement::docs;
    if (docMap.count(docName) == 0)
        docMap[docName] = new DocObjectElement(1, 1, 0);
    if (docMap[docName]->type != DOC_OBJECT) {
        Error::addError(
                new FieldInvalidError(docName, 1, 1, "/", docMap[docName]->type, DOC_OBJECT)
        );
        return false;
    }
    DocObjectElement *ro = (DocObjectElement *) docMap[docName];

    if (ro->get("info") != NULL)
        delete ro->get("info");
    ro->add("info", DataObjectAdapter::toDocElement(obj));
    return true;
}

bool setHost(string host, string docName) {
    cleanToDocStage();
    map<string, DocElement *> &docMap = DocElement::docs;
    if (docMap.count(docName) == 0)
        docMap[docName] = new DocObjectElement(1, 1, 0);
    if (docMap[docName]->type != DOC_OBJECT) {
        Error::addError(
                new FieldInvalidError(docName, 1, 1, "/", docMap[docName]->type, DOC_OBJECT)
        );
        return false;
    }
    DocObjectElement *ro = (DocObjectElement *) docMap[docName];

    if (ro->get("host") != NULL)
        delete ro->get("host");
    ro->add("host", new DocScalarElement(1, 1, host));
    return true;
}

bool setBasePath(string basePath, string docName) {
    cleanToDocStage();
    map<string, DocElement *> &docMap = DocElement::docs;
    if (docMap.count(docName) == 0)
        docMap[docName] = new DocObjectElement(1, 1, 0);
    if (docMap[docName]->type != DOC_OBJECT) {
        Error::addError(
                new FieldInvalidError(docName, 1, 1, "/", docMap[docName]->type, DOC_OBJECT)
        );
        return false;
    }
    DocObjectElement *ro = (DocObjectElement *) docMap[docName];

    if (ro->get("basePath") != NULL)
        delete ro->get("basePath");
    ro->add("basePath", new DocScalarElement(1, 1, basePath));
    return true;
}

bool removeBasePath(string docName) {
    cleanToDocStage();
    map<string, DocElement *> &docMap = DocElement::docs;
    if (docMap.count(docName) == 0)
        return false;
    if (docMap[docName]->type != DOC_OBJECT) {
        Error::addError(
                new FieldInvalidError(docName, 1, 1, "/", docMap[docName]->type, DOC_OBJECT)
        );
        return false;
    }
    DocObjectElement *ro = (DocObjectElement *) docMap[docName];

    if (ro->get("basePath") != NULL) {
        delete ro->get("basePath");
        ro->erase("basePath");
        return true;
    }
    return false;
}

bool setSchemes(BaseDataObject *obj, string docName) {
    if (obj == NULL) return false;
    cleanToDocStage();
    map<string, DocElement *> &docMap = DocElement::docs;
    if (docMap.count(docName) == 0)
        docMap[docName] = new DocObjectElement(1, 1, 0);
    if (docMap[docName]->type != DOC_OBJECT) {
        Error::addError(
                new FieldInvalidError(docName, 1, 1, "/", docMap[docName]->type, DOC_OBJECT)
        );
        return false;
    }
    DocObjectElement *ro = (DocObjectElement *) docMap[docName];

    if (ro->get("schemes") != NULL)
        delete ro->get("schemes");
    ro->add("schemes", DataObjectAdapter::toDocElement(obj));
    return true;
}

bool removeSchemes(string docName) {
    cleanToDocStage();
    map<string, DocElement *> &docMap = DocElement::docs;
    if (docMap.count(docName) == 0)
        return false;
    if (docMap[docName]->type != DOC_OBJECT) {
        Error::addError(
                new FieldInvalidError(docName, 1, 1, "/", docMap[docName]->type, DOC_OBJECT)
        );
        return false;
    }
    DocObjectElement *ro = (DocObjectElement *) docMap[docName];

    if (ro->get("schemes") != NULL) {
        delete ro->get("schemes");
        ro->erase("schemes");
        return true;
    }
    return false;
}

bool setConsumes(BaseDataObject *obj, string docName) {
    if (obj == NULL) return false;
    cleanToDocStage();
    map<string, DocElement *> &docMap = DocElement::docs;
    if (docMap.count(docName) == 0)
        docMap[docName] = new DocObjectElement(1, 1, 0);
    if (docMap[docName]->type != DOC_OBJECT) {
        Error::addError(
                new FieldInvalidError(docName, 1, 1, "/", docMap[docName]->type, DOC_OBJECT)
        );
        return false;
    }
    DocObjectElement *ro = (DocObjectElement *) docMap[docName];

    if (ro->get("consumes") != NULL)
        delete ro->get("consumes");
    ro->add("consumes", DataObjectAdapter::toDocElement(obj));
    return true;
}

bool removeConsumes(string docName) {
    cleanToDocStage();
    map<string, DocElement *> &docMap = DocElement::docs;
    if (docMap.count(docName) == 0)
        return false;
    if (docMap[docName]->type != DOC_OBJECT) {
        Error::addError(
                new FieldInvalidError(docName, 1, 1, "/", docMap[docName]->type, DOC_OBJECT)
        );
        return false;
    }
    DocObjectElement *ro = (DocObjectElement *) docMap[docName];

    if (ro->get("consumes") != NULL) {
        delete ro->get("consumes");
        ro->erase("consumes");
        return true;
    }
    return false;
}

bool setProduces(BaseDataObject *obj, string docName) {
    if (obj == NULL) return false;
    cleanToDocStage();
    map<string, DocElement *> &docMap = DocElement::docs;
    if (docMap.count(docName) == 0)
        docMap[docName] = new DocObjectElement(1, 1, 0);
    if (docMap[docName]->type != DOC_OBJECT) {
        Error::addError(
                new FieldInvalidError(docName, 1, 1, "/", docMap[docName]->type, DOC_OBJECT)
        );
        return false;
    }
    DocObjectElement *ro = (DocObjectElement *) docMap[docName];

    if (ro->get("produces") != NULL)
        delete ro->get("produces");
    ro->add("produces", DataObjectAdapter::toDocElement(obj));
    return true;
}

bool removeProduces(string docName) {
    cleanToDocStage();
    map<string, DocElement *> &docMap = DocElement::docs;
    if (docMap.count(docName) == 0)
        return false;
    if (docMap[docName]->type != DOC_OBJECT) {
        Error::addError(
                new FieldInvalidError(docName, 1, 1, "/", docMap[docName]->type, DOC_OBJECT)
        );
        return false;
    }
    DocObjectElement *ro = (DocObjectElement *) docMap[docName];

    if (ro->get("produces") != NULL) {
        delete ro->get("produces");
        ro->erase("produces");
        return true;
    }
    return false;
}

bool setTags(BaseDataObject *obj, string docName) {
    if (obj == NULL) return false;
    cleanToDocStage();
    map<string, DocElement *> &docMap = DocElement::docs;
    if (docMap.count(docName) == 0)
        docMap[docName] = new DocObjectElement(1, 1, 0);
    if (docMap[docName]->type != DOC_OBJECT) {
        Error::addError(
                new FieldInvalidError(docName, 1, 1, "/", docMap[docName]->type, DOC_OBJECT)
        );
        return false;
    }
    DocObjectElement *ro = (DocObjectElement *) docMap[docName];

    if (ro->get("tags") != NULL)
        delete ro->get("tags");
    ro->add("tags", DataObjectAdapter::toDocElement(obj));
    return true;
}

bool removeTags(string docName) {
    cleanToDocStage();
    map<string, DocElement *> &docMap = DocElement::docs;
    if (docMap.count(docName) == 0)
        return false;
    if (docMap[docName]->type != DOC_OBJECT) {
        Error::addError(
                new FieldInvalidError(docName, 1, 1, "/", docMap[docName]->type, DOC_OBJECT)
        );
        return false;
    }
    DocObjectElement *ro = (DocObjectElement *) docMap[docName];

    if (ro->get("tags") != NULL) {
        delete ro->get("tags");
        ro->erase("tags");
        return true;
    }
    return false;
}

bool setExternalDocs(BaseDataObject *obj, string docName) {
    if (obj == NULL) return false;
    cleanToDocStage();
    map<string, DocElement *> &docMap = DocElement::docs;
    if (docMap.count(docName) == 0)
        docMap[docName] = new DocObjectElement(1, 1, 0);
    if (docMap[docName]->type != DOC_OBJECT) {
        Error::addError(
                new FieldInvalidError(docName, 1, 1, "/", docMap[docName]->type, DOC_OBJECT)
        );
        return false;
    }
    DocObjectElement *ro = (DocObjectElement *) docMap[docName];

    if (ro->get("externalDocs") != NULL)
        delete ro->get("externalDocs");
    ro->add("externalDocs", DataObjectAdapter::toDocElement(obj));
    return true;
}

bool removeExternalDocs(string docName) {
    cleanToDocStage();
    map<string, DocElement *> &docMap = DocElement::docs;
    if (docMap.count(docName) == 0)
        return false;
    if (docMap[docName]->type != DOC_OBJECT) {
        Error::addError(
                new FieldInvalidError(docName, 1, 1, "/", docMap[docName]->type, DOC_OBJECT)
        );
        return false;
    }
    DocObjectElement *ro = (DocObjectElement *) docMap[docName];

    if (ro->get("externalDocs") != NULL) {
        delete ro->get("externalDocs");
        ro->erase("externalDocs");
        return true;
    }
    return false;
}

bool addAPISharedParameter(BaseDataObject *obj, string pathName, string docName) {
    if (obj == NULL) return false;
    cleanToDocStage();
    map<string, DocElement *> &docMap = DocElement::docs;
    if (docMap.count(docName) == 0)
        docMap[docName] = new DocObjectElement(1, 1, 0);
    if (docMap[docName]->type != DOC_OBJECT) {
        Error::addError(
                new FieldInvalidError(docName, 1, 1, "/", docMap[docName]->type, DOC_OBJECT)
        );
        return false;
    }
    DocObjectElement *ro = (DocObjectElement *) docMap[docName];

    if (ro->get("paths") == NULL) {
        ro->add("paths", new DocObjectElement(1, 1));
    }
    if (ro->get("paths")->type != DOC_OBJECT) {
        Error::addError(
                new FieldInvalidError(docName, 1, 1, "paths", ro->get("paths")->type, DOC_OBJECT)
        );
        return false;
    }
    DocObjectElement *pathso = (DocObjectElement *) ro->get("paths");

    pathName.insert(0, "/");
    if (pathso->get(pathName) == NULL) {
        pathso->add(pathName, new DocObjectElement(1, 1));
    }
    if (pathso->get(pathName)->type != DOC_OBJECT) {
        Error::addError(
                new FieldInvalidError(docName, 1, 1, string("paths") + pathName,
                pathso->get(pathName)->type, DOC_OBJECT)
        );
        return false;
    }
    DocObjectElement *patho = (DocObjectElement *) pathso->get(pathName);

    if (patho->get("parameter") == NULL) {
        patho->add("parameter", new DocSequenceElement(1, 1));
    }
    if (patho->get("parameter")->type != DOC_SEQUENCE) {
        Error::addError(
                new FieldInvalidError(docName, 1, 1, string("paths") + pathName + "paramter",
                patho->get("parameter")->type, DOC_SEQUENCE)
        );
        return false;
    }

    DocSequenceElement *o = (DocSequenceElement *) patho->get("parameter");

    o->add(DataObjectAdapter::toDocElement(obj));
    return true;
}

bool removeAPISharedParameters(string pathName, string docName) {
    cleanToDocStage();
    map<string, DocElement *> &docMap = DocElement::docs;
    if (docMap.count(docName) == 0)
        return false;
    if (docMap[docName]->type != DOC_OBJECT) {
        Error::addError(
                new FieldInvalidError(docName, 1, 1, "/", docMap[docName]->type, DOC_OBJECT)
        );
        return false;
    }
    DocObjectElement *ro = (DocObjectElement *) docMap[docName];

    if (ro->get("paths") == NULL) {
        return false;
    }
    if (ro->get("paths")->type != DOC_OBJECT) {
        Error::addError(
                new FieldInvalidError(docName, 1, 1, "paths", ro->get("paths")->type, DOC_OBJECT)
        );
        return false;
    }
    DocObjectElement *pathso = (DocObjectElement *) ro->get("paths");

    pathName.insert(0, "/");
    if (pathso->get(pathName) == NULL) {
        return false;
    }
    if (pathso->get(pathName)->type != DOC_OBJECT) {
        Error::addError(
                new FieldInvalidError(docName, 1, 1, string("paths") + pathName,
                                      pathso->get(pathName)->type, DOC_OBJECT)
        );
        return false;
    }
    DocObjectElement *patho = (DocObjectElement *) pathso->get(pathName);

    if (patho->get("parameter") != NULL) {
        delete patho->get("parameter");
        patho->erase("parameter");
        return true;
    }
    return false;
}

bool addAPI(BaseDataObject *obj, string pathName, string method, string docName) {
    if (obj == NULL) return false;
    if ((method == "get") || (method == "GET")) method = "get";
        else if ((method == "post") || (method == "POST")) method = "post";
            else return false;
    cleanToDocStage();
    map<string, DocElement *> &docMap = DocElement::docs;
    if (docMap.count(docName) == 0)
        docMap[docName] = new DocObjectElement(1, 1, 0);
    if (docMap[docName]->type != DOC_OBJECT) {
        Error::addError(
                new FieldInvalidError(docName, 1, 1, "/", docMap[docName]->type, DOC_OBJECT)
        );
        return false;
    }
    DocObjectElement *ro = (DocObjectElement *) docMap[docName];

    if (ro->get("paths") == NULL) {
        ro->add("paths", new DocObjectElement(1, 1));
    }
    if (ro->get("paths")->type != DOC_OBJECT) {
        Error::addError(
                new FieldInvalidError(docName, 1, 1, "paths", ro->get("paths")->type, DOC_OBJECT)
        );
        return false;
    }
    DocObjectElement *pathso = (DocObjectElement *) ro->get("paths");

    pathName.insert(0, "/");
    if (pathso->get(pathName) == NULL) {
        pathso->add(pathName, new DocObjectElement(1, 1));
    }
    if (pathso->get(pathName)->type != DOC_OBJECT) {
        Error::addError(
                new FieldInvalidError(docName, 1, 1, string("paths") + pathName,
                                      pathso->get(pathName)->type, DOC_OBJECT)
        );
        return false;
    }
    DocObjectElement *patho = (DocObjectElement *) pathso->get(pathName);

    if (patho->get(method) != NULL) {
        delete patho->get(method);
    }
    patho->add(method, DataObjectAdapter::toDocElement(obj));

    return true;
}

bool removeAPI(string pathName, string method, string docName) {
    if ((method == "get") || (method == "GET")) method = "get";
        else if ((method == "post") || (method == "POST")) method = "post";
            else return false;
    cleanToDocStage();
    map<string, DocElement *> &docMap = DocElement::docs;
    if (docMap.count(docName) == 0)
        return false;
    if (docMap[docName]->type != DOC_OBJECT) {
        Error::addError(
                new FieldInvalidError(docName, 1, 1, "/", docMap[docName]->type, DOC_OBJECT)
        );
        return false;
    }
    DocObjectElement *ro = (DocObjectElement *) docMap[docName];

    if (ro->get("paths") == NULL) {
        return false;
    }
    if (ro->get("paths")->type != DOC_OBJECT) {
        Error::addError(
                new FieldInvalidError(docName, 1, 1, "paths", ro->get("paths")->type, DOC_OBJECT)
        );
        return false;
    }
    DocObjectElement *pathso = (DocObjectElement *) ro->get("paths");

    pathName.insert(0, "/");
    if (pathso->get(pathName) == NULL) {
        return false;
    }
    if (pathso->get(pathName)->type != DOC_OBJECT) {
        Error::addError(
                new FieldInvalidError(docName, 1, 1, string("paths") + pathName,
                                      pathso->get(pathName)->type, DOC_OBJECT)
        );
        return false;
    }
    DocObjectElement *patho = (DocObjectElement *) pathso->get(pathName);
    if (patho->get(method)) {
        delete patho->get(method);
        patho->erase(method);
        return true;
    }
    return false;
}

bool addDataSchema(BaseDataObject *obj, string name, string docName) {
    if (obj == NULL) return false;
    cleanToDocStage();
    map<string, DocElement *> &docMap = DocElement::docs;
    if (docMap.count(docName) == 0)
        docMap[docName] = new DocObjectElement(1, 1);
    if (docMap[docName]->type != DOC_OBJECT) {
        Error::addError(
                new FieldInvalidError(docName, 1, 1, "/", docMap[docName]->type, DOC_OBJECT)
        );
        return false;
    }
    DocObjectElement *ro = (DocObjectElement *) docMap[docName];

    if (ro->get("definitions") == NULL) {
        ro->add("definitions", new DocObjectElement(1, 1));
    }
    if (ro->get("definitions")->type != DOC_OBJECT) {
        Error::addError(
                new FieldInvalidError(docName, 1, 1, "definitions", ro->get("definitions")->type, DOC_OBJECT)
        );
        return false;
    }
    DocObjectElement *difio = (DocObjectElement *) ro->get("definitions");

    if (difio->get(name) != NULL)
        delete difio->get(name);
    difio->add(name, DataObjectAdapter::toDocElement(obj));

    return true;
}

bool removeDataSchema(string name, string docName) {
    cleanToDocStage();
    map<string, DocElement *> &docMap = DocElement::docs;
    if (docMap.count(docName) == 0)
        return false;
    if (docMap[docName]->type != DOC_OBJECT) {
        Error::addError(
                new FieldInvalidError(docName, 1, 1, "/", docMap[docName]->type, DOC_OBJECT)
        );
        return false;
    }
    DocObjectElement *ro = (DocObjectElement *) docMap[docName];

    if (ro->get("definitions") == NULL) {
        return false;
    }
    if (ro->get("definitions")->type != DOC_OBJECT) {
        Error::addError(
                new FieldInvalidError(docName, 1, 1, "definitions", ro->get("definitions")->type, DOC_OBJECT)
        );
        return false;
    }
    DocObjectElement *difio = (DocObjectElement *) ro->get("definitions");

    if (difio->get(name) != NULL) {
        delete difio->get(name);
        difio->erase(name);
        return true;
    }
    return false;
}

bool addParameter(BaseDataObject *obj, string name, string docName) {
    if (obj == NULL) return false;
    cleanToDocStage();
    map<string, DocElement *> &docMap = DocElement::docs;
    if (docMap.count(docName) == 0)
        docMap[docName] = new DocObjectElement(1, 1);
    if (docMap[docName]->type != DOC_OBJECT) {
        Error::addError(
                new FieldInvalidError(docName, 1, 1, "/", docMap[docName]->type, DOC_OBJECT)
        );
        return false;
    }
    DocObjectElement *ro = (DocObjectElement *) docMap[docName];

    if (ro->get("parameters") == NULL) {
        ro->add("parameters", new DocObjectElement(1, 1));
    }
    if (ro->get("parameters")->type != DOC_OBJECT) {
        Error::addError(
                new FieldInvalidError(docName, 1, 1, "parameters", ro->get("parameters")->type, DOC_OBJECT)
        );
        return false;
    }
    DocObjectElement *paramo = (DocObjectElement *) ro->get("parameters");

    if (paramo->get(name) != NULL)
        delete paramo->get(name);
    paramo->add(name, DataObjectAdapter::toDocElement(obj));

    return true;
}

bool removeParameter(string name, string docName) {
    cleanToDocStage();
    map<string, DocElement *> &docMap = DocElement::docs;
    if (docMap.count(docName) == 0)
        return false;
    if (docMap[docName]->type != DOC_OBJECT) {
        Error::addError(
                new FieldInvalidError(docName, 1, 1, "/", docMap[docName]->type, DOC_OBJECT)
        );
        return false;
    }
    DocObjectElement *ro = (DocObjectElement *) docMap[docName];

    if (ro->get("parameters") == NULL) {
        return false;
    }
    if (ro->get("parameters")->type != DOC_OBJECT) {
        Error::addError(
                new FieldInvalidError(docName, 1, 1, "parameters", ro->get("parameters")->type, DOC_OBJECT)
        );
        return false;
    }
    DocObjectElement *paramo = (DocObjectElement *) ro->get("parameters");

    if (paramo->get(name) != NULL) {
        delete paramo->get(name);
        paramo->erase(name);
        return true;
    }
    return false;
}

bool addResponse(BaseDataObject *obj, string name, string docName) {
    if (obj == NULL) return false;
    cleanToDocStage();
    map<string, DocElement *> &docMap = DocElement::docs;
    if (docMap.count(docName) == 0)
        docMap[docName] = new DocObjectElement(1, 1);
    if (docMap[docName]->type != DOC_OBJECT) {
        Error::addError(
                new FieldInvalidError(docName, 1, 1, "/", docMap[docName]->type, DOC_OBJECT)
        );
        return false;
    }
    DocObjectElement *ro = (DocObjectElement *) docMap[docName];

    if (ro->get("responses") == NULL) {
        ro->add("responses", new DocObjectElement(1, 1));
    }
    if (ro->get("responses")->type != DOC_OBJECT) {
        Error::addError(
                new FieldInvalidError(docName, 1, 1, "responses", ro->get("responses")->type, DOC_OBJECT)
        );
        return false;
    }
    DocObjectElement *respo = (DocObjectElement *) ro->get("responses");

    if (respo->get(name) != NULL)
        delete respo->get(name);
    respo->add(name, DataObjectAdapter::toDocElement(obj));

    return true;
}

bool removeResponse(string name, string docName) {
    cleanToDocStage();
    map<string, DocElement *> &docMap = DocElement::docs;
    if (docMap.count(docName) == 0)
        return false;
    if (docMap[docName]->type != DOC_OBJECT) {
        Error::addError(
                new FieldInvalidError(docName, 1, 1, "/", docMap[docName]->type, DOC_OBJECT)
        );
        return false;
    }
    DocObjectElement *ro = (DocObjectElement *) docMap[docName];

    if (ro->get("responses") == NULL) {
        return false;
    }
    if (ro->get("responses")->type != DOC_OBJECT) {
        Error::addError(
                new FieldInvalidError(docName, 1, 1, "responses", ro->get("responses")->type, DOC_OBJECT)
        );
        return false;
    }
    DocObjectElement *respo = (DocObjectElement *) ro->get("responses");

    if (respo->get(name) != NULL) {
        delete respo->get(name);
        respo->erase(name);
        return true;
    }
    return false;
}


/** --- Parse to API --- **/
bool parseAPI() {
    cleanToDocStage();

    RefExpand *refExpand = new RefExpand();
    if (!refExpand->work()) {
        delete refExpand;
        return false;
    }
    delete refExpand;

    controller = new Controller();
    if (!controller->work()) {
        delete controller;
        controller = NULL;
        return false;
    }

    state = API_PARSED;
    return true;
}

/** --- API Info Acquire --- **/

BaseDataObject *getInfo() {
    if (state == DOC_TREE || controller == NULL) {
        Error::addError(
                new NotParsedError()
        );
        return NULL;
    }
    if (controller->infoObject != NULL)
        return controller->infoObject->toDataObject();
    else
        return NULL;
}

BaseDataObject *getHost() {
    if (state == DOC_TREE || controller == NULL) {
        Error::addError(
                new NotParsedError()
        );
        return NULL;
    }
    return new StringDataObject(controller->host);
}

BaseDataObject *getBasePath() {
    if (state == DOC_TREE || controller == NULL) {
        Error::addError(
                new NotParsedError()
        );
        return NULL;
    }
    return new StringDataObject(controller->basePath);
}

BaseDataObject *getSchemes() {
    if (state == DOC_TREE || controller == NULL) {
        Error::addError(
                new NotParsedError()
        );
        return NULL;
    }
    if (controller->schemes != NULL)
        return controller->schemes->toDataObject();
    else
        return NULL;
}

BaseDataObject *getConsumes() {
    if (state == DOC_TREE || controller == NULL) {
        Error::addError(
                new NotParsedError()
        );
        return NULL;
    }
    if (controller->consumes != NULL)
        return controller->consumes->toDataObject();
    else
        return NULL;
}

BaseDataObject *getProduces() {
    if (state == DOC_TREE || controller == NULL) {
        Error::addError(
                new NotParsedError()
        );
        return NULL;
    }
    if (controller->produces != NULL)
        return controller->produces->toDataObject();
    else
        return NULL;
}

BaseDataObject *getAPINames() {
    if (state == DOC_TREE || controller == NULL) {
        Error::addError(
                new NotParsedError()
        );
        return NULL;
    }
    if (controller->paths != NULL) {
        map<pair<string, APIRequestMethod>, APIObject*> &nameObjMap =
                controller->paths->getNameObjectMap();
        SequenceDataObject *ans = new SequenceDataObject();
        for (map<pair<string, APIRequestMethod>, APIObject*>::iterator ite = nameObjMap.begin();
                ite != nameObjMap.end(); ++ite) {
            ObjectDataObject *item = new ObjectDataObject();
            (*item)["name"] = new StringDataObject(ite->first.first);
            if (ite->first.second == APIRequestMethod::GET)
                (*item)["method"] = new StringDataObject("get");
            if (ite->first.second == APIRequestMethod::POST)
                (*item)["method"] = new StringDataObject("post");
            ans->push(item);
        }
        return ans;
    } else
        return NULL;
}

BaseDataObject *getAPI(string name, string method) {
    if (state == DOC_TREE || controller == NULL) {
        Error::addError(
                new NotParsedError()
        );
        return NULL;
    }
    if (controller->paths != NULL) {
        pair<string, APIRequestMethod> requestName;
        requestName.first = name;
        if ((method == "get") || (method == "GET"))
            requestName.second = APIRequestMethod::GET;
        else if ((method == "post") || (method == "POST"))
            requestName.second = APIRequestMethod::POST;
        else
            return NULL;
        APIObject *obj = controller->paths->getObjectByName(requestName);
        if (obj != NULL)
            return obj->toDataObject();
        else
            return NULL;
    } else
        return NULL;
}

BaseDataObject *getDataSchemaNames() {
    if (state == DOC_TREE || controller == NULL) {
        Error::addError(
                new NotParsedError()
        );
        return NULL;
    }
    if (controller->definitions != NULL) {
        const map<string, DocElement*> &nameMap = controller->definitions->getNameMap();
        SequenceDataObject *ans = new SequenceDataObject();
        for (map<string, DocElement*>::const_iterator ite = nameMap.cbegin();
                ite != nameMap.cend();
                ++ite) {
            StringDataObject *item = new StringDataObject(ite->first);
            ans->push(item);
        }
        return ans;
    } else
        return NULL;
}

BaseDataObject *getDataSchema(string name) {
    if (state == DOC_TREE || controller == NULL) {
        Error::addError(
                new NotParsedError()
        );
        return NULL;
    }
    if (controller->definitions != NULL) {
        DataSchemaObject *obj = controller->definitions->getSchemaByName(name);
        if (obj != NULL)
            return obj->toDataObject();
        else
            return NULL;
    } else
        return NULL;
}

BaseDataObject *generateRandomDataFromDataSchema(string name) {
    if (state == DOC_TREE || controller == NULL) {
        Error::addError(
                new NotParsedError()
        );
        return NULL;
    }
    if (controller->definitions != NULL) {
        DataSchemaObject *obj = controller->definitions->getSchemaByName(name);
        if (obj != NULL)
            return obj->generate();
        else
            return NULL;
    } else
        return NULL;
}

bool verifyDataByDataSchema(BaseDataObject *data, string name) {
    if (state == DOC_TREE || controller == NULL) {
        Error::addError(
                new NotParsedError()
        );
        return false;
    }
    if (controller->definitions != NULL) {
        DataSchemaObject *obj = controller->definitions->getSchemaByName(name);
        if (obj != NULL)
            return obj->check(data);
        else
            return false;
    } else
        return false;
}

BaseDataObject *getParameterNames() {
    if (state == DOC_TREE || controller == NULL) {
        Error::addError(
                new NotParsedError()
        );
        return NULL;
    }
    if (controller->parameters != NULL) {
        const map<string, DocElement*> &nameMap = controller->parameters->getNameMap();
        SequenceDataObject *ans = new SequenceDataObject();
        for (map<string, DocElement*>::const_iterator ite = nameMap.cbegin();
             ite != nameMap.cend();
             ++ite) {
            StringDataObject *item = new StringDataObject(ite->first);
            ans->push(item);
        }
        return ans;
    } else
        return NULL;
}

BaseDataObject *getParameter(string name) {
    if (state == DOC_TREE || controller == NULL) {
        Error::addError(
                new NotParsedError()
        );
        return NULL;
    }
    if (controller->parameters != NULL) {
        ParameterObject *obj = controller->parameters->getParameterByName(name);
        if (obj != NULL)
            return obj->toDataObject();
        else
            return NULL;
    } else
        return NULL;
}

BaseDataObject *getResponseNames() {
    if (state == DOC_TREE || controller == NULL) {
        Error::addError(
                new NotParsedError()
        );
        return NULL;
    }
    if (controller->responses != NULL) {
        const map<string, DocElement*> &nameMap = controller->responses->getNameMap();
        SequenceDataObject *ans = new SequenceDataObject();
        for (map<string, DocElement*>::const_iterator ite = nameMap.cbegin();
             ite != nameMap.cend();
             ++ite) {
            StringDataObject *item = new StringDataObject(ite->first);
            ans->push(item);
        }
        return ans;
    } else
        return NULL;
}

BaseDataObject *getResponse(string name) {
    if (state == DOC_TREE || controller == NULL) {
        Error::addError(
                new NotParsedError()
        );
        return NULL;
    }
    if (controller->responses != NULL) {
        AbstractResponseObject *obj = controller->responses->getResponseByName(name);
        if (obj != NULL)
            return obj->toDataObject();
        else
            return NULL;
    } else
        return NULL;
}

BaseDataObject *getTags() {
    if (state == DOC_TREE || controller == NULL) {
        Error::addError(
                new NotParsedError()
        );
        return NULL;
    }
    if (controller->tags != NULL)
        return controller->tags->toDataObject();
    else
        return NULL;
}

BaseDataObject *getExternalDocs() {
    if (state == DOC_TREE || controller == NULL) {
        Error::addError(
                new NotParsedError()
        );
        return NULL;
    }
    if (controller->externalDocs != NULL)
        return controller->externalDocs->toDataObject();
    else
        return NULL;
}

/** --- Locals --- **/
void cleanToDocStage() {
    if (state == SCENARIO_PARSED)
        /** TODO **/;
    if (state == API_PARSED)
        if (controller != NULL) delete controller;
    state = DOC_TREE;
}