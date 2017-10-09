//
// Created by lly on 07/10/2017.
//

#include "ExecTransformer.h"

string ExecTransformer::serailize(BaseDataObject *obj, DataSchemaObject *schema, string paramName) {
    if (schema->type == DataSchemaObject::TYPE_ARRAY) {
        ArraySchema *arrSchema = (ArraySchema*)schema;
        if (obj->type != SEQUENCE) return "";
        string ans = "";
        SequenceDataObject *seqObj = (SequenceDataObject*)obj;
        switch (arrSchema->collectionFormat) {
            case CSV:
                for (int i=0; i<seqObj->length(); ++i) {
                    if (i > 0) ans += ",";
                    ans += ExecTransformer::baseSerialize(seqObj->vec[i], arrSchema->items, paramName);
                }
                break;
            case SSV:
                for (int i=0; i<seqObj->length(); ++i) {
                    if (i > 0) ans += " ";
                    ans += ExecTransformer::baseSerialize(seqObj->vec[i], arrSchema->items, paramName);
                }
                break;
            case TSV:
                for (int i=0; i<seqObj->length(); ++i) {
                    if (i > 0) ans += "\t";
                    ans += ExecTransformer::baseSerialize(seqObj->vec[i], arrSchema->items, paramName);
                }
                break;
            case PIPES:
                for (int i=0; i<seqObj->length(); ++i) {
                    if (i > 0) ans += "|";
                    ans += ExecTransformer::baseSerialize(seqObj->vec[i], arrSchema->items, paramName);
                }
                break;
            case MULTI:
                for (int i=0; i<seqObj->length(); ++i) {
                    if (i > 0) ans += "&";
                    ans += paramName + "=" + ExecTransformer::baseSerialize(seqObj->vec[i], arrSchema->items, paramName);
                }
                break;
        }
        return ans;
    } else {
        string ans = ExecTransformer::baseSerialize(obj, schema, paramName);
        if (schema->type == DataSchemaObject::TYPE_OBJECT) {
            ObjectSchema *objSchema = (ObjectSchema*)schema;
            if (objSchema->serialType == ObjectSerialType::XML)
                ans.insert(0, "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n");
        }
        return ans;
    }
}

string ExecTransformer::baseSerialize(
        BaseDataObject *obj,
        DataSchemaObject *schema,
        string paramName,
        int type) {
    if (obj == NULL) return "";

    string pres = "";
    string sufs = "";
    string XMLName = paramName;
    if ((schema->xml) && (schema->xml->hasName))
        XMLName = schema->xml->name;
    if ((schema->xml) && (schema->xml->hasPrefix))
        XMLName = schema->xml->prefix + ":" + XMLName;
    if (type == 2) {
        // only work for XML
        if ((schema->xml) && (schema->xml->attribute)) {
            pres = XMLName + "=\"";
            sufs = "\"";
        } else {
            pres = "<";
            if ((schema->xml) && (schema->xml->hasNameSpace)) {
                pres += "xmlns=\"";
                pres += schema->xml->_namespace;
                pres += "\" ";
            }
            pres += XMLName;
            pres += ">\n";
            sufs = "</" + XMLName + ">\n";
        }
    }

    if (schema->type == DataSchemaObject::TYPE_OBJECT) {
        if (obj->type != OBJECT) return "";
        ObjectSchema *objSchema = (ObjectSchema*)schema;
        ObjectDataObject *objObj = (ObjectDataObject*)obj;
        if ((type == 1) ||
                ((type == 0) && ((objSchema->serialType == ObjectSerialType::JSON) || (objSchema->serialType == ObjectSerialType::YAML)))) {
            stringstream s;
            s.clear();
            s << "{";
            bool needComma = false;
            map<string, DataSchemaObject*> &properties = objSchema->properties;
            for (map<string, DataSchemaObject*>::iterator ite = properties.begin();
                    ite != properties.end();
                    ++ite)
                if (objObj->hasKey(ite->first)) {
                    if (needComma) s << ", \n"; else needComma = true;
                    s << "\"" << ite->first << "\"" << ": ";
                    if ((*objObj)[ite->first]->type == STRING)
                        s << "\"" << ExecTransformer::baseSerialize((*objObj)[ite->first], ite->second, ite->first, 1) << "\"";
                    else
                        s << ExecTransformer::baseSerialize((*objObj)[ite->first], ite->second, ite->first, 1);
                }
            s << "}";
            s.flush();
            return s.str();
        }
        else if ((type == 2) ||
                ((type == 0) && (objSchema->serialType == ObjectSerialType::XML))) {
            stringstream s;
            s.clear();

            map<string, DataSchemaObject*> &properties = objSchema->properties;
            map<string, string> *fieldRes = new map<string, string>();
            for (map<string, DataSchemaObject*>::iterator ite = properties.begin();
                    ite != properties.end();
                    ++ite)
                if (objObj->hasKey(ite->first)) {
                    (*fieldRes)[ite->first] = ExecTransformer::baseSerialize((*objObj)[ite->first], ite->second, ite->first, 2);
                }

            s << "<";
            s << XMLName;
            if ((objSchema->xml) && (objSchema->xml->hasNameSpace))
                s << " xmlns=\"" << objSchema->xml->_namespace << "\"";

            for (map<string, string>::iterator ite = fieldRes->begin(); ite != fieldRes->end(); ++ite) {
                if ((properties[ite->first]->xml) && (properties[ite->first]->xml->attribute)) {
                    // in attribute fields
                    s << " " << ite->second;
                }
            }

            s << ">";

            for (map<string, string>::iterator ite = fieldRes->begin(); ite != fieldRes->end(); ++ite) {
                if ((properties[ite->first]->xml == NULL) || (!properties[ite->first]->xml->attribute)) {
                    // out attribute fields
                    s << ite->second;
                }
            }

            s << "</" + XMLName + ">\n";
            return s.str();
        }
    }
    else if (schema->type == DataSchemaObject::TYPE_ARRAY) {
        if (obj->type != SEQUENCE) return "";
        ArraySchema *arraySchema = (ArraySchema*)schema;
        SequenceDataObject *seqObj = (SequenceDataObject*)obj;
        stringstream s;
        s.clear();
        if ((type == 0) || (type == 1)) {
            // JSON style
            s << "[";
            for (int i=0; i<seqObj->length(); ++i) {
                if (i > 0) s << ", ";
                s << ExecTransformer::baseSerialize(seqObj->vec[i], arraySchema->items, paramName, type);
            }
            s << "]\n";
        } else {
            // XML style
            if ((schema->xml) && (schema->xml->wrapped))
                s << pres;
            for (int i=0; i<seqObj->length(); ++i) {
                s << ExecTransformer::baseSerialize(seqObj->vec[i], arraySchema->items, paramName, type);
            }
            if ((schema->xml) && (schema->xml->wrapped))
                s << sufs;
        }
        return s.str();
    }
    else if (schema->type == DataSchemaObject::TYPE_STRING) {
        if (obj->type == STRING)
            return pres + ((StringDataObject*)obj)->str + sufs;
        else
            return "";
    }
    else if ((schema->type == DataSchemaObject::TYPE_NUMBER) || (schema->type == DataSchemaObject::TYPE_INTEGER)) {
        stringstream s;
        s.clear();
        s << pres;
        if (obj->type == INTEGER)
            s << ((IntegerDataObject*)obj)->value;
        if (obj->type == NUMBER)
            s << ((NumberDataObject*)obj)->value;
        s << sufs;
        s.flush();
        return s.str();
    }
    else if (schema->type == DataSchemaObject::TYPE_BOOLEAN) {
        BooleanSchema *boolSchema = (BooleanSchema*)schema;
        stringstream s;
        s.clear();
        s << pres;
        if (obj->type == STRING)
            s << ((StringDataObject*)obj)->str;
        if (obj->type == BOOLEAN) {
            BooleanDataObject *boolObj = (BooleanDataObject*)obj;
            if (boolObj->value)
                s << boolSchema->trueString;
            else
                s << boolSchema->falseString;
        }
        s << sufs;
        s.flush();
        return s.str();
    }
    else if ((schema->type == DataSchemaObject::TYPE_FILE) || (schema->type == DataSchemaObject::TYPE_CUSTOMIZED)) {
        stringstream s;
        s.clear();
        s << pres;
        if (obj->type == BYTEFILE)
            s << ((FileDataObject*)obj)->data;
        if (obj->type == CUSTOMIZED)
            s << ((CustomizedDataObject*)obj)->data;
        s << sufs;
        s.flush();
        return s.str();
    }
}

DocElement *ExecTransformer::deserialize(string s, DataSchemaObject *schema) {
    if (schema == NULL) return NULL;
    if (schema->type == DataSchemaObject::TYPE_ARRAY) {
        DocSequenceElement *seqEle = new DocSequenceElement(1, 1);
        ArraySchema *arrSchema = (ArraySchema*)schema;
        int pl = 0, pr = 0;
        while (pl < s.length()) {
            switch (arrSchema->collectionFormat) {
                case CSV:
                    while ((pr < s.length() - 1) && (s[pr] != ','))
                        ++pr;
                    break;
                case SSV:
                    while ((pr < s.length() - 1) && (s[pr] != ' '))
                        ++pr;
                    break;
                case TSV:
                    while ((pr < s.length() - 1) && (s[pr] != '\t'))
                        ++pr;
                    break;
                case PIPES:
                    while ((pr < s.length() - 1) && (s[pr] != '|'))
                        ++pr;
                    break;
                case MULTI:
                    while ((pl < s.length()) && (s[pl] != '='))
                        ++pl;
                    pl += 1;
                    pr = pl;
                    while ((pr < s.length() - 1) && (s[pr] != '&'))
                        ++pr;
                    break;
            }
            if ((pl < s.length()) && (pl <= pr)) {
                DocElement *now = ExecTransformer::baseDeserialize(
                        s.substr((unsigned)pl, (unsigned)(pr-pl+1)), arrSchema->items, 0);
                if (now != NULL)
                    seqEle->add(now);
                else {
                    delete seqEle;
                    return NULL;
                }
            }
            pl = pr + 1;
        }
        return seqEle;
    } else
        return ExecTransformer::baseDeserialize(s, schema, 0);
}

DocElement *ExecTransformer::baseDeserialize(
        string s,
        DataSchemaObject *schema,
        int type) {
    if (type == 0) {
        if (schema->type == DataSchemaObject::TYPE_OBJECT) {
            ObjectSchema *objSchema = (ObjectSchema *) schema;
            if ((objSchema->serialType == ObjectSerialType::JSON) || (objSchema->serialType == ObjectSerialType::YAML))
                type = 1;
            if (objSchema->serialType == ObjectSerialType::XML)
                type = 2;
        }
    }

    if ((type == 0) || (type == 1)) {
        return YAMLAdapter::parseStr((const unsigned char*)s.c_str(), (int)s.length());
    }
    if (type == 2) {
        xmlDocPtr doc;
        xmlNodePtr cur;
        doc = xmlParseDoc(BAD_CAST s.c_str());
        if (doc == NULL)
            return NULL;
        cur = xmlDocGetRootElement(doc);
        if (cur == NULL)
            return NULL;
        DocElement *ans = ExecTransformer::xmlParse(cur, schema);
        xmlFreeDoc(doc);
        return ans;
    }

    return NULL;
}

DocElement *ExecTransformer::xmlParse(
        xmlNodePtr ptr,
        DataSchemaObject *schema) {
    if (schema->type == DataSchemaObject::TYPE_OBJECT) {
        ObjectSchema *objSchema = (ObjectSchema*)schema;
        map<string, DataSchemaObject*> &properties = objSchema->properties;

        DocObjectElement *objEle = new DocObjectElement(1, 1);

        xmlAttrPtr attrPtr = ptr->properties;
        while (attrPtr != NULL) {
            const xmlChar *xmlAttrName = attrPtr->name;
            string prefix = "";
            string attrName = string((const char*)xmlAttrName);
            string realName = "";
            int comma = -1;
            for (int i=(int)attrName.length() - 1; i>=0; --i)
                if (attrName[i] == ':')
                    comma = i;
            if (comma != -1)
                prefix = attrName.substr(0, (unsigned)comma);
            attrName = attrName.substr((unsigned)comma + 1);

            for (map<string, DataSchemaObject*>::iterator ite = properties.begin();
                    ite != properties.end();
                    ++ite) {
                DataSchemaObject *nowSchema = ite->second;
                string target_pref = "";
                string target_name = ite->first;
                if ((nowSchema->xml) && (nowSchema->xml->attribute)) {
                    if (nowSchema->xml->hasName)
                        target_name = nowSchema->xml->name;
                    if (nowSchema->xml->hasPrefix)
                        target_pref = nowSchema->xml->prefix;
                }
                if ((prefix == target_pref) && (attrName == target_name)) {
                    realName = ite->first;
                    break;
                }
            }

            if (realName != "") {
                xmlChar *attrValue = xmlGetProp(ptr, xmlAttrName);
                objEle->add(realName, new DocScalarElement(1, 1, string((const char*)attrValue)));
                xmlFree(attrValue);
            }

            attrPtr = attrPtr->next;
        }

        xmlNodePtr child = ptr->children;
        while (child != NULL) {
            if (child->type == XML_ELEMENT_NODE) {
                const xmlChar *memberName = child->name;
                string prefix = "";
                string attrName = string((const char *) memberName);
                string realName = "";
                int comma = -1;
                for (int i = (int) attrName.length() - 1; i >= 0; --i)
                    if (attrName[i] == ':')
                        comma = i;
                if (comma != -1)
                    prefix = attrName.substr(0, (unsigned) comma);
                attrName = attrName.substr((unsigned) comma + 1);

                for (map<string, DataSchemaObject *>::iterator ite = properties.begin();
                     ite != properties.end();
                     ++ite) {
                    DataSchemaObject *nowSchema = ite->second;
                    string target_pref = "";
                    string target_name = ite->first;
                    if (nowSchema->xml) {
                        if (nowSchema->xml->hasName)
                            target_name = nowSchema->xml->name;
                        if (nowSchema->xml->hasPrefix)
                            target_pref = nowSchema->xml->prefix;
                    }
                    if ((prefix == target_pref) && (attrName == target_name)) {
                        realName = ite->first;
                        break;
                    }
                }

                if (realName != "") {
                    if (properties[realName]->type == DataSchemaObject::TYPE_ARRAY) {
                        if ((properties[realName]->xml == NULL) || (!properties[realName]->xml->wrapped)) {
                            // unwrapped
                            if (objEle->get(realName) == NULL)
                                objEle->add(realName, new DocSequenceElement(1, 1));
                            else {
                                DocElement *childEle = ExecTransformer::xmlParse(child, ((ArraySchema*)(properties[realName]))->items);
                                if (childEle != NULL)
                                    ((DocSequenceElement*)objEle->get(realName))->add(childEle);
                            }
                        } else {
                            // wrapped
                            DocElement *childEle = ExecTransformer::xmlParse(child, properties[realName]);
                            if (childEle != NULL)
                                objEle->add(realName, childEle);
                        }
                    } else {
                        DocElement *childEle = ExecTransformer::xmlParse(child, properties[realName]);
                        if (childEle != NULL)
                            objEle->add(realName, childEle);
                    }
                }
            }

            child = child->next;
        }
        return objEle;
    }
    else if (schema->type == DataSchemaObject::TYPE_ARRAY) {
        ArraySchema *arrSchema = (ArraySchema*)schema;
        DataSchemaObject *itemSchema = arrSchema->items;
        DocSequenceElement *seqEle = new DocSequenceElement(1, 1);

        xmlNodePtr child = ptr->children;
        while (child != NULL) {
            if (child->type == XML_ELEMENT_NODE) {
                DocElement *itemEle = ExecTransformer::xmlParse(child, itemSchema);
                if (itemEle != NULL)
                    seqEle->add(itemEle);
            }
            child = child->next;
        }
        return seqEle;
    }
    else {
        // just treat like scalar
        string content = "";
        xmlNodePtr child = ptr->children;
        while (child != NULL) {
            if (child->type == XML_TEXT_NODE)
                content += (const char*)child->content;
            child = child->next;
        }
        if (ptr->content != NULL)
            content += (const char*)ptr->content;
        return new DocScalarElement(1, 1, content);
    }
}
