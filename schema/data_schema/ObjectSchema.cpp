//
// Created by lly on 17/07/2017.
//

#include "ObjectSchema.h"


ObjectSchema::ObjectSchema(): DataSchemaObject() {
    type = TYPE_OBJECT;
    properties.clear();
    serialType = JSON;
}

BaseDataObject *ObjectSchema::toDataObject() {
    BaseDataObject *obj = DataSchemaObject::toDataObject();
    // --- above are routine ---
    if (obj) {
        ObjectDataObject *o = (ObjectDataObject*)obj;
        ObjectDataObject *prop = new ObjectDataObject();
        for (map<string, DataSchemaObject*>::iterator ite = properties.begin();
                ite != properties.end();
                ++ite) {
            (*prop)[ite->first] = ite->second->toDataObject();
        }
        (*o)["properties"] = prop;
        switch (serialType) {
            case JSON:
                (*o)["x-serial"] = new StringDataObject("JSON");
                break;
            case YAML:
                (*o)["x-serial"] = new StringDataObject("YAML");
                break;
            case XML:
                (*o)["x-serial"] = new StringDataObject("XML");
                break;
        }
    }
    return obj;
}

bool ObjectSchema::check(BaseDataObject *obj) {
    if (DataSchemaObject::check(obj)) {
        if (obj == NULL) return true;
        // allow empty value
        // --- above are routine ---
        if (obj->type == OBJECT) {
            ObjectDataObject *o = (ObjectDataObject*)obj;
            map<string, BaseDataObject*> &om = o->m;
            for (map<string, BaseDataObject*>::iterator ite = om.begin();
                    ite != om.end();
                    ++ite) {
                // every member in given DataObject, should has corresponding schema in definition
                if (this->properties.count(ite->first) != 1)
                    return false;
                // every member should conform to the schema defined
                if (!this->properties[ite->first]->check(ite->second))
                    return false;
            }
            for (map<string, DataSchemaObject*>::iterator ite = properties.begin();
                    ite != properties.end();
                    ++ite) {
                // every member in schema, if not has corresponding data member, should permit empty
                if (om.count(ite->first) == 0)
                    if (!ite->second->check(NULL))
                        return false;
            }
            return true;
        }
    }
    return false;
}

BaseDataObject *ObjectSchema::transform(DocElement *ele) {
    if (!this->valid) return NULL;
    if (ele == NULL) return NULL;
    // --- above are routine ---
    if (ele->type == DOC_OBJECT) {
        DocObjectElement *e = (DocObjectElement*)ele;
        map<string, DocElement*> *m = e->getMemberMap();
        ObjectDataObject *ans = new ObjectDataObject();
        for (map<string, DocElement*>::iterator ite = m->begin();
                ite != m->end();
                ++ite) {
            if (properties.count(ite->first) != 1) {
                // just ignore the member
                continue;
            }
            BaseDataObject *now = properties[ite->first]->transform(ite->second);
            if (now == NULL) {
                delete ans;
                return NULL;
            }
            (*ans)[ite->first] = now;
        }
        if (!check(ans)) {
            delete ans;
            return NULL;
        }
        return ans;
    }
    return NULL;
}

BaseDataObject *ObjectSchema::generate() {
    BaseDataObject *enumGen = DataSchemaObject::generate();
    if (enumGen) return enumGen;
    if (!this->valid) return NULL;
    if ((DataSchemaObject::randomReal() < emptyProbability) && (this->allowEmptyValue)) return NULL;
    // --- above are routine ---
    ObjectDataObject *obj = new ObjectDataObject();
    for (map<string, DataSchemaObject*>::iterator ite = properties.begin();
            ite != properties.end();
            ++ite) {
        BaseDataObject *o = ite->second->generate();
        if (o)
            (*obj)[ite->first] = o;
            // only insert non-null member
    }
    return obj;
}

bool ObjectSchema::init(string filePath, DocObjectElement *obj, int schemaType) {
    if (schemaType != DataSchemaObjectFactory::NORMAL_SCHEMA) {
        return false;
    }

    /* properties */
    DocElement *propEle = obj->get("properties");
    if (propEle == NULL) {
        Error::addError(
                new FieldMissError(filePath, obj->line, obj->col, "schema.properties")
        );
        return false;
    }
    if (propEle->type != DOC_OBJECT) {
        Error::addError(
                new FieldInvalidError(filePath, propEle->line, propEle->col, "schema.properties",
                propEle->type, DOC_OBJECT)
        );
        return false;
    }

    DocObjectElement *pEle = (DocObjectElement*)propEle;
    map<string, DocElement*> *pMap = pEle->getMemberMap();
    for (map<string, DocElement*>::iterator ite = pMap->begin();
            ite != pMap->end();
            ++ite) {
        if (ite->second->type != DOC_OBJECT) {
            Error::addError(
                    new FieldInvalidError(filePath, ite->second->line, ite->second->col, "schema.properties",
                    ite->second->type, DOC_OBJECT)
            );
            return false;
        }
        DocObjectElement *item = (DocObjectElement*)ite->second;
        DataSchemaObject *nowObj =
                DataSchemaObjectFactory::create(filePath, item, DataSchemaObjectFactory::NORMAL_SCHEMA, true);
        if (nowObj == NULL)
            return false;
        this->properties[ite->first] = nowObj;
    }

    /* x-serial */
    DocElement *serialEle = obj->get("x-serial");
    if (serialEle) {
        pair<string, bool> ret = DocElementHelper::parseToString(serialEle);
        if (ret.second) {
            string &s = ret.first;
            if (s == "JSON")
                this->serialType = JSON;
            else if (s == "YAML")
                this->serialType = YAML;
            else if (s == "XML")
                this->serialType = XML;
            else {
                Error::addError(
                        new FieldIllegalError(filePath, serialEle->line, serialEle->col, "schema.x-serial")
                );
                return false;
            }
        } else {
            Error::addError(
                    new FieldInvalidError(filePath, serialEle->line, serialEle->col, "schema.x-serial",
                    serialEle->type, DOC_SCALAR)
            );
            return false;
        }
    }

    return true;
}

ObjectSchema::~ObjectSchema() {
    for (map<string, DataSchemaObject*>::iterator ite = properties.begin();
            ite != properties.end();
            ++ite) {
        delete ite->second;
    }
}

DataSchemaObject *ObjectSchema::findField(const vector<string> &fieldVec, int index) {
    if (index == fieldVec.size())
        return this;
    else {
        if (properties.count(fieldVec[index]) > 0)
            return properties[fieldVec[index]]->findField(fieldVec, index + 1);
        else
            return NULL;
    }
}