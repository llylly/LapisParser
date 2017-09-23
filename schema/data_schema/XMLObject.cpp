//
// Created by lly on 17/07/2017.
//

#include "XMLObject.h"
#include "../../data/BaseDataObject.h"
#include "../../data/ObjectDataObject.h"
#include "../../data/StringDataObject.h"
#include "../../data/BooleanDataObject.h"

BaseDataObject *XMLObject::toDataObject() {
    ObjectDataObject *obj = new ObjectDataObject();
    if (this->hasName)
        (*obj)["name"] = new StringDataObject(this->name);
    if (this->hasNameSpace)
        (*obj)["namespace"] = new StringDataObject(this->_namespace);
    if (this->hasPrefix)
        (*obj)["prefix"] = new StringDataObject(this->prefix);
    (*obj)["attribute"] = new BooleanDataObject(this->attribute);
    (*obj)["wrapped"] = new BooleanDataObject(this->wrapped);

    // inner generated settings
    (*obj)["availAttribute"] = new BooleanDataObject(this->availAttribute);
    (*obj)["availWrapped"] = new BooleanDataObject(this->availWrapped);

    return obj;
}

XMLObject::XMLObject() {
    hasName = false;
    hasNameSpace = false;
    hasPrefix = false;
    availAttribute = false;
    attribute = false;
    availWrapped = false;
    wrapped = false;
}

XMLObject *XMLObjectFactory::create(string filePath, string fieldName, DocObjectElement *ele) {
    XMLObject *ans = new XMLObject();

    /* name */
    DocElement *nameEle = ele->get("name");
    if (nameEle) {
        pair<string, bool> nameRes = DocElementHelper::parseToString(nameEle);
        if (nameRes.second) {
            ans->name = nameRes.first;
        } else {
            Error::addError(
                    new FieldInvalidError(
                            filePath, nameEle->line, nameEle->col, fieldName + ".name", nameEle->type, DOC_SCALAR
                    )
            );
            delete ans;
            return NULL;
        }
    }

    /* namespace */
    DocElement *namespaceEle = ele->get("namespace");
    if (namespaceEle) {
        pair<string, bool> namespaceRes = DocElementHelper::parseToString(namespaceEle);
        if (namespaceRes.second) {
            ans->_namespace = namespaceRes.first;
        } else {
            Error::addError(
                    new FieldInvalidError(
                            filePath, namespaceEle->line, namespaceEle->col, fieldName + ".namespace", namespaceEle->type, DOC_SCALAR
                    )
            );
            delete ans;
            return NULL;
        }
    }

    /* prefix */
    DocElement *prefixEle = ele->get("prefix");
    if (prefixEle) {
        pair<string, bool> prefixRes = DocElementHelper::parseToString(prefixEle);
        if (prefixRes.second) {
            ans->prefix = prefixRes.first;
        } else {
            Error::addError(
                    new FieldInvalidError(
                            filePath, prefixEle->line, prefixEle->col, fieldName + ".prefix", prefixEle->type, DOC_SCALAR
                    )
            );
            delete ans;
            return NULL;
        }
    }

    /* attribute */
    DocElement *attributeEle = ele->get("attribute");
    if (attributeEle) {
        pair<bool, bool> attributeRes = DocElementHelper::parseToBool(attributeEle);
        if (attributeRes.second) {
            ans->attribute = attributeRes.first;
        } else {
            Error::addError(
                    new FieldIllegalError(
                            filePath, attributeEle->line, attributeEle->col, fieldName + ".attribute"
                    )
            );
            delete ans;
            return NULL;
        }
    }

    /* wrapped */
    DocElement *wrappedEle = ele->get("wrapped");
    if (wrappedEle) {
        pair<bool, bool> wrappedRes = DocElementHelper::parseToBool(wrappedEle);
        if (wrappedRes.second) {
            ans->wrapped = wrappedRes.first;
        } else {
            Error::addError(
                    new FieldIllegalError(
                            filePath, wrappedEle->line, wrappedEle->col, fieldName + ".wrapped"
                    )
            );
            delete ans;
            return NULL;
        }
    }

    return ans;
}