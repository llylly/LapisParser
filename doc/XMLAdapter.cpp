//
// Created by lly on 23/04/2017.
//

#include "XMLAdapter.h"
#include "../error/XMLEmitterError.h"

DocElement *XMLAdapter::parseDoc(const char *fileName) {

    xmlDocPtr doc;
    xmlNodePtr cur;

    doc = xmlParseFile(fileName);

    if (doc == NULL) {
        XMLFormatError *err = new XMLFormatError(1, 1);

        // Read error message from stderr
        err->msg = "";
        char *errMsgBuf = new char[10000];
        FILE *errIn = fopen("/dev/stderr", "r");
        while (fscanf(errIn, "%s\n", errMsgBuf) > 0)
            err->msg += errMsgBuf, err->msg += "\n";
        delete[] errMsgBuf;
        fclose(errIn);

        Error::addError(err);
        return NULL;
    }


    cur = xmlDocGetRootElement(doc);

    if (cur == NULL) {
        // Whole doc is empty
        XMLFormatError *err = new XMLFormatError(1, 1);
        err->msg = "Empty document node.";
        Error::addError(err);
        xmlFreeDoc(doc);
        return NULL;
    }

    if (xmlStrcmp(cur->name, BAD_CAST "root") != 0) {
        // Root element name is not "root"
        XMLFormatError *err = new XMLFormatError(cur->line, 1);
        err->msg = "Illegal root element.";
        Error::addError(err);
        xmlFreeDoc(doc);
        return NULL;
    }

    DocElement *ans = XMLAdapter::mainParse(cur, 0);
    xmlFreeDoc(doc);

    return ans;
}

DocElement *XMLAdapter::mainParse(xmlNodePtr ptr, int level) {
    bool isScalar = true;
    xmlNodePtr child = ptr->children;
    while (child != NULL) {
        if (child->type != XML_TEXT_NODE) isScalar = false;
        child = child->next;
    }
    bool isSequence = false;
    if (xmlHasProp(ptr, BAD_CAST "nodeType")) {
        xmlChar *nodeType = xmlGetProp(ptr, BAD_CAST "nodeType");
        if (xmlStrcmp(nodeType, BAD_CAST "array") == 0)
            isSequence = true;
        xmlFree(nodeType);
    }
    if (xmlHasProp(ptr, BAD_CAST "value") || isScalar) {
        // A Scalar Element
        DocScalarElement *ele;
        if (xmlHasProp(ptr, BAD_CAST "value")) {
            ele = new DocScalarElement(ptr->line, 1, string((const char*)xmlGetProp(ptr, BAD_CAST "value")), level);
        } else {
            child = ptr->children;
            string content = "";
            while (child != NULL) {
                content += (const char*)child->content;
                child = child->next;
            }
            ele = new DocScalarElement(ptr->line, 1, content, level);
        }
        return ele;
    } else
    if (isSequence) {
        // Check whether a sequence element
        DocSequenceElement *ele = new DocSequenceElement(ptr->line, 1, level);
        if (parseSequence(ptr, ele, level)) {
            return NULL;
        }
        return ele;
    } else {
        // A Object Element
        DocObjectElement *ele = new DocObjectElement(ptr->line, 1, level);
        if (parseObject(ptr, ele, level)) {
            return NULL;
        }
        return ele;
    }
}

int XMLAdapter::parseSequence(xmlNodePtr arrayPtr, DocSequenceElement *ele, int level) {
    const xmlChar *arrayName = arrayPtr->name;
    bool isAlternative = false;
    if (xmlStrcmp(arrayName, BAD_CAST "node") == 0) {
        if (xmlHasProp(arrayPtr, BAD_CAST "nodeName")) {
            arrayName = xmlGetProp(arrayPtr, BAD_CAST "nodeName");
            isAlternative = true;
        }
    }
    xmlNodePtr child = arrayPtr->children;
    while (child != NULL) {
        if (child->type == XML_ELEMENT_NODE) {
            const xmlChar *childName = child->name;
            bool isChildAlternative = false;
            if (xmlStrcmp(childName, BAD_CAST "node") == 0) {
                if (xmlHasProp(child, BAD_CAST "nodeName")) {
                    childName = xmlGetProp(child, BAD_CAST "nodeName");
                    isChildAlternative = true;
                }
            }
            if (xmlStrcmp(childName, arrayName) != 0) {
                XMLFormatError *err = new XMLFormatError(child->line, 1);
                err->msg = "Illegal array element label name.";
                Error::addError(err);
                if (isAlternative) xmlFree((xmlChar*)arrayName);
                if (isChildAlternative) xmlFree((xmlChar*)childName);
                return -1;
            }
            DocElement *item = mainParse(child, level + 1);
            if (item == NULL) {
                if (isAlternative) xmlFree((xmlChar*)arrayName);
                if (isChildAlternative) xmlFree((xmlChar*)childName);
                return -1;
            }
            ele->add(item);
        }
        child = child->next;
    }
    if (isAlternative) xmlFree((xmlChar*)arrayName);
    return 0;
}

int XMLAdapter::parseObject(xmlNodePtr objPtr, DocObjectElement *ele, int level) {
    xmlAttrPtr attrPtr = objPtr->properties;
    bool isAlternative = false;
    if (xmlStrcmp(objPtr->name, BAD_CAST "node") == 0) {
        if (xmlHasProp(objPtr, BAD_CAST "nodeName")) {
            isAlternative = true;
        }
    }
    while (attrPtr != NULL) {
        const xmlChar *attrName = attrPtr->name;
        xmlChar *attrValue = xmlGetProp(objPtr, attrName);
        if (!isAlternative || (xmlStrcmp(attrName, BAD_CAST "nodeName") != 0)) {
            DocScalarElement *scalar = new DocScalarElement(ele->line, 1, string((const char *) attrValue), level + 1);
            ele->add(string((const char *) attrName), scalar);
        }
        xmlFree(attrValue);
        attrPtr = attrPtr->next;
    }
    xmlNodePtr child = objPtr->children;
    while (child != NULL) {
        const xmlChar *memberName = child->name;
        bool isMemberAlternative = false;
        if (xmlStrcmp(child->name, BAD_CAST "node") == 0) {
            if (xmlHasProp(child, BAD_CAST "nodeName")) {
                isMemberAlternative = true;
                memberName = xmlGetProp(child, BAD_CAST "nodeName");
            }
        }
        if (child->type == XML_ELEMENT_NODE) {
            DocElement *childEle = XMLAdapter::mainParse(child, level + 1);
            if (childEle != NULL) {
                ele->add(string((const char *) memberName), childEle);
            } else {
                if (isMemberAlternative) xmlFree((xmlChar*)memberName);
                return -1;
            }
        }
        if (isMemberAlternative) xmlFree((xmlChar*)memberName);
        child = child->next;
    }
    return 0;
}

int XMLAdapter::saveFile(const char *fileName, DocElement *root) {
    xmlDocPtr doc = xmlNewDoc(BAD_CAST "1.0");
    xmlNodePtr rootNode = xmlNewNode(NULL, BAD_CAST "root");

    xmlDocSetRootElement(doc, rootNode);

    int res = saveObject(rootNode, root, string("root"));

    if (res == 0) {
        int saveStat = xmlSaveFormatFileEnc(fileName, doc, "UTF-8", 1);
        if (saveStat == -1) {
            XMLEmitterError *err = new XMLEmitterError(1, 1);
            err->msg = "XML file save error.";
            Error::addError(err);
            return -1;
        }
    } else {
        return -1;
    }

    xmlFreeDoc(doc);
    return 0;
}

int XMLAdapter::saveObject(xmlNodePtr objPtr, DocElement *ele, string fieldName) {
    if (ele->type == DOC_OBJECT) {
        DocObjectElement *objEle = (DocObjectElement*)ele;
        map<string, DocElement*> *member = objEle->getMemberMap();
        for (map<string, DocElement*>::iterator ite = member->begin();
                ite != member->end();
                ++ite) {
            string labelName = ite->first;
            bool alter = needAlternate(labelName);
            if (labelName.length() > 0) {
                xmlNodePtr child;
                if (!alter)
                    child = xmlNewNode(NULL, BAD_CAST labelName.c_str());
                else {
                    child = xmlNewNode(NULL, BAD_CAST "node");
                    xmlNewProp(child, BAD_CAST "nodeName", BAD_CAST labelName.c_str());
                }
                if (saveObject(child, ite->second, labelName)) {
                    xmlFreeNode(child);
                    return -1;
                }
                xmlAddChild(objPtr, child);
            } else {
                XMLEmitterError *err = new XMLEmitterError(ele->line, ele->col);
                err->msg = "Label name is empty.";
                Error::addError(err);
                return -1;
            }
        }
        return 0;
    }
    if (ele->type == DOC_SEQUENCE) {
        DocSequenceElement *seqEle = (DocSequenceElement*)ele;
        xmlNewProp(objPtr, BAD_CAST "nodeType", BAD_CAST "array");
        bool alter = needAlternate(fieldName);
        int len = seqEle->getLength();
        for (int i=0; i<len; ++i) {
            xmlNodePtr child;
            if (!alter)
                child = xmlNewNode(NULL, BAD_CAST fieldName.c_str());
            else {
                child = xmlNewNode(NULL, BAD_CAST "node");
                xmlNewProp(child, BAD_CAST "nodeName", BAD_CAST fieldName.c_str());
            }
            if (saveObject(child, seqEle->get(i), fieldName)) {
                xmlFreeNode(child);
                return -1;
            }
            xmlAddChild(objPtr, child);
        }
        return 0;
    }
    if (ele->type == DOC_SCALAR) {
        DocScalarElement *scaEle = (DocScalarElement*)ele;
        xmlNewProp(objPtr, BAD_CAST "value", BAD_CAST scaEle->getValue().c_str());
        return 0;
    }
}

bool XMLAdapter::needAlternate(string s) {
    if (s.length() > 0)
        if (((s[0] >= 'a') && (s[0] <= 'z')) || ((s[0] >= 'A') && (s[0] <= 'Z'))) {
            for (int i=0; i<s.length(); ++i)
                if ((s[i] == '(') || (s[i] == ')') || (s[i] == '{') || (s[i] == '}') || (s[i] == ' '))
                    return true;
            return false;
        }
    return true;
}