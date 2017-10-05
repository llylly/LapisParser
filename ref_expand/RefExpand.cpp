//
// Created by lly on 08/05/2017.
//

#include "RefExpand.h"
#include "../doc/DocSequenceElement.h"
#include "../doc/DocObjectElement.h"
#include "../doc/DocScalarElement.h"
#include "../error/RefRecursiveError.h"
#include "../error/RefNotFoundError.h"

int RefExpand::MAX_RECURSIVE_TIME = 100;

bool RefExpand::work() {
    pathStack.clear();
    eleStack.clear();
    refObjRepo.clear();
    bool expanded;
    int time = 0;
    do {
        expanded = false;
        for (map<string, DocElement *>::iterator ite = DocElement::docs.begin(); ite != DocElement::docs.end(); ++ite) {
            curFile = ite->first;
            expanded |= visit(curFile, &(ite->second));
        }
        // If find error when expanding, stop
        if (Error::hasError()) return false;
        ++time;
    } while ((expanded) && (time <= MAX_RECURSIVE_TIME));
    // Delete all reference object
    for (set<DocElement*>::iterator ite = refObjRepo.begin(); ite != refObjRepo.end(); ++ite)
        delete *ite;
    if (expanded && (time > MAX_RECURSIVE_TIME)) {
        Error::addError(new RefRecursiveError(1, 1));
        return false;
    }
    return true;
}

bool RefExpand::visit(string currentDoc, DocElement **ele) {
    DocElement *dele = *ele;
    if (dele == NULL) return false;
    if (dele->type == DOC_SCALAR) {
        return false;
    }
    if (dele->type == DOC_SEQUENCE) {
        DocSequenceElement *seqEle = (DocSequenceElement*)dele;
        bool tmp = false;
        for (int i=0; i<seqEle->getLength(); ++i) {
            tmp |= visit(currentDoc, seqEle->editGet(i));
        }
        return tmp;
    }
    if (dele->type == DOC_OBJECT) {
        DocObjectElement *objEle = (DocObjectElement*)dele;
        // Check whether is a Reference Object
        if (objEle->getSize() == 1) {
            DocElement *pathEle;
            if ((pathEle = objEle->get("$ref"))) {
                if (pathEle->type == DOC_SCALAR) {
                    string path = ((DocScalarElement *) pathEle)->getValue();
                    DocElement *realEle = findByPath(currentDoc, path, dele->line, dele->col);
                    refObjRepo.insert(dele);
                    if (realEle != NULL) {
                        *ele = realEle;
                        return true;
                    } else
                        return false;
                }
            }
        }
        // Not a Reference Object
        std::map<std::string, DocElement*> *map = objEle->getMemberMap();
        bool tmp = false;
        for (std::map<std::string, DocElement*>::iterator ite = map->begin(); ite != map->end(); ++ite) {
            tmp |= visit(currentDoc, &(ite->second));
        }
        return tmp;
    }
    return false;
}

DocElement* RefExpand::findByPath(string currentDoc, string path, int line, int col) {
    int lastP = -1;
    vector<string> pathVec;
    pathVec.clear();
    bool insideQuote = false;
    int quote1 = -1, quote2 = -1;
    for (int i=0; i<path.length(); ++i) {
        // For constant '/' like "//", we recognize the first '/' as separator and second one as part of name
        // For the entire name inside a quote (' or "), will ignore all '/' inside it
        if ((path[i] == '\'') || (path[i] == '\"')) {
            if (!insideQuote) {
                insideQuote = true;
                quote1 = i;
            } else {
                insideQuote = false;
                quote2 = i;
            }
        }
        if ((path[i] == '/') && (i > lastP + 1) && (!insideQuote)) {
            if (i-lastP-1 > 0) {
                string nowstr = "";
                for (int j = lastP + 1; j < i; ++j)
                    if ((j != quote1) and (j != quote2))
                        nowstr += path[j];
                pathVec.push_back(nowstr);
            }
            lastP = i;
        }
    }
    if (path.length() - lastP > 1) {
        string nowstr = "";
        for (int j = lastP + 1; j < path.length(); ++j) {
            if ((j != quote1) and (j != quote2))
                nowstr += path[j];
        }
        pathVec.push_back(nowstr);
    }
    if (pathVec.size() <= 0) return NULL;

    string doc = currentDoc;
    string fileName = pathVec[0];
    while ((fileName.length() > 0) && (fileName[fileName.length()-1] == '#')) {
        fileName.erase(fileName.length()-1, 1);
    }
    if (fileName.length() > 0)
        doc = fileName;

    if (DocElement::docs.find(doc) == DocElement::docs.end()) {
        // failed
        Error::addError(new RefNotFoundError(currentDoc, line, col));
        return NULL;
    }

    DocElement *ele = DocElement::docs[doc];

    for (int i=1; i<pathVec.size(); ++i) {
        string now = pathVec[i];
        if (ele->type == DOC_SEQUENCE) {
            // for array, explore its first element
            do {
                DocSequenceElement *seqEle = (DocSequenceElement *) ele;
                if (seqEle->getLength() > 0) {
                    ele = seqEle->get(0);
                }
            } while (ele->type != DOC_SEQUENCE);
        }
        if (ele->type == DOC_SCALAR) {
            //failed
            Error::addError(new RefNotFoundError(currentDoc, line, col));
            return NULL;
        }
        if (ele->type == DOC_OBJECT) {
            DocObjectElement *objEle = (DocObjectElement *)ele;
            if (objEle->get(now) != NULL) {
                ele = objEle->get(now);
            } else {
                //failed
                Error::addError(new RefNotFoundError(currentDoc, line, col));
                return NULL;
            }
        }
    }

    return ele;
}
