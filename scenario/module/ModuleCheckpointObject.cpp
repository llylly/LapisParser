//
// Created by lly on 23/09/2017.
//

#include "ModuleCheckpointObject.h"

ModuleCheckpointObject::ModuleCheckpointObject() {
    this->fieldVec.clear();
    this->type = INVALID_CHECKPOINT;
    this->setName = "";
    this->value = NULL;
    this->excluding.clear();
    this->including.clear();
}

ModuleCheckpointObject::~ModuleCheckpointObject() {
    this->fieldVec.clear();
    this->excluding.clear();
    this->including.clear();
}

string ModuleCheckpointObject::getKeyName() {
    string fieldStr = "";
    for (int i=0; i < this->fieldVec.size(); ++i) {
        fieldStr += this->fieldVec[i];
        if (i != this->fieldVec.size() - 1) fieldStr += '.';
    }
    return fieldStr;
}

BaseDataObject *ModuleCheckpointObject::toDataObject() {
    ObjectDataObject *res = new ObjectDataObject();

    string typeStr = "";
    switch (this->type) {
        case INSET_CHECKPOINT:
            typeStr = "inSet";
            break;
        case OUTSET_CHECKPOINT:
            typeStr = "outSet";
            break;
        case EQUAL_CHECKPOINT:
            typeStr = "equal";
            break;
        case UNEQUAL_CHECKPOINT:
            typeStr = "unequal";
            break;
        default:
            ;
    }
    (*res)["type"] = new StringDataObject(typeStr);

    if ((this->type == INSET_CHECKPOINT) || (this->type == OUTSET_CHECKPOINT)) {
        (*res)["setName"] = new StringDataObject(this->setName);
    }
    if ((this->type == EQUAL_CHECKPOINT) || (this->type == UNEQUAL_CHECKPOINT)) {
        (*res)["value"] = DataObjectAdapter::defaultFromDocElement(this->value);
    }
    if (this->excluding.size() > 0) {
        SequenceDataObject *exSeq = new SequenceDataObject();
        for (vector<string>::iterator ite = this->excluding.begin();
                ite != this->excluding.end();
                ++ite) {
            exSeq->push(new StringDataObject(*ite));
        }
        (*res)["excluding"] = exSeq;
    }
    if (this->including.size() > 0) {
        SequenceDataObject *inSeq = new SequenceDataObject();
        for (vector<string>::iterator ite = this->including.begin();
                ite != this->including.end();
                ++ite) {
            inSeq->push(new StringDataObject(*ite));
        }
        (*res)["including"] = inSeq;
    }
    return res;
}

ModuleCheckpointObject *ModuleCheckpointObjectFactory::create(string filePath, string key, DocElement *ele) {
    if ((ele == NULL) || (key == "")) {
        return NULL;
    }
    ModuleCheckpointObject *res = new ModuleCheckpointObject();

    /** key parse **/
    int key_s = 0;
    for (int i=0; i <= key.length(); ++i) {
        char c = (i == key.length())? '.': key[i];
        if (c == '.') {
            if (key_s < i)
                res->fieldVec.push_back(key.substr((unsigned)key_s, (unsigned)(i - key_s)));
            key_s = i + 1;
        }
    }
    if (res->fieldVec[0] != "out") {
        Error::addError(new FieldIllegalError(filePath, ele->line, ele->col, "x-scenario.modules.checkpoint.key"));
        delete res;
        return NULL;
    }

    if (ele->type != DOC_OBJECT) {
        Error::addError(new FieldInvalidError(filePath, ele->line, ele->col, "x-scenario.modules.checkpoint", ele->type, DOC_OBJECT));
        delete res;
        return NULL;
    }
    DocObjectElement *objEle = (DocObjectElement*)ele;

    /** type (required) **/
    DocElement *typeEle = objEle->get("type");
    if (typeEle == NULL) {
        Error::addError(new FieldMissError(filePath, ele->line, ele->col, "x-scenario.modules.checkpoint.type"));
        delete res;
        return NULL;
    } else {
        pair<string, bool> typeParse = DocElementHelper::parseToString(typeEle);
        if (!typeParse.second) {
            Error::addError(new FieldInvalidError(filePath, typeEle->line, typeEle->col, "x-scenario.modules.checkpoint.type",
            typeEle->type, DOC_SCALAR));
            delete res;
            return NULL;
        } else {
            string &typeStr = typeParse.first;
            if (typeStr == "inSet")
                res->type = INSET_CHECKPOINT;
            else if (typeStr == "outSet")
                res->type = OUTSET_CHECKPOINT;
            else if (typeStr == "equal")
                res->type = EQUAL_CHECKPOINT;
            else if (typeStr == "unequal")
                res->type = UNEQUAL_CHECKPOINT;
            else {
                Error::addError(new FieldIllegalError(filePath, typeEle->line, typeEle->col, "x-scenario.modules.checkpoint.type"));
                delete res;
                return NULL;
            }
        }
    }

    if ((res->type == INSET_CHECKPOINT) || (res->type == OUTSET_CHECKPOINT)) {
        /** setName (required) **/
        DocElement *setNameEle = objEle->get("setName");
        if (setNameEle == NULL) {
            Error::addError(new FieldMissError(filePath, ele->line, ele->col, "x-scenario.modules.checkpoint.setName"));
            delete res;
            return NULL;
        } else {
            if (setNameEle->type != DOC_SCALAR) {
                Error::addError(new FieldInvalidError(filePath, setNameEle->line, setNameEle->col, "x-scenario.modules.checkpoint.setName",
                setNameEle->type, DOC_SCALAR));
                delete res;
                return NULL;
            } else {
                res->setName = ((DocScalarElement*)setNameEle)->getValue();
            }
        }
    }

    if ((res->type == EQUAL_CHECKPOINT) ||(res->type == UNEQUAL_CHECKPOINT)) {
        /** value (required) **/
        DocElement *valueEle = objEle->get("value");
        if (valueEle == NULL) {
            Error::addError(new FieldMissError(filePath, ele->line, ele->col, "x-scenario.modules.checkpoint.value"));
            delete res;
            return NULL;
        } else {
            res->value = valueEle;
        }
    }

    /** excluding (optional) **/
    DocElement *exEle = objEle->get("excluding");
    if (exEle != NULL) {
        if (exEle->type != DOC_SEQUENCE) {
            Error::addError(new FieldInvalidError(filePath, exEle->line, exEle->col, "x-scenario.modules.checkpoint.excluding",
                                                  exEle->type, DOC_SEQUENCE));
            delete res;
            return NULL;
        } else {
            DocSequenceElement *seqExEle = (DocSequenceElement*)exEle;
            int len = seqExEle->getLength();
            for (int i=0; i<len; ++i) {
                DocElement *nowEle = seqExEle->get(i);
                if (nowEle->type != DOC_SCALAR) {
                    Error::addError(new FieldInvalidError(filePath, nowEle->line, nowEle->col, "x-scenario.modules.checkpoint.excluding",
                                                          nowEle->type, DOC_SCALAR));
                    delete res;
                    return NULL;
                } else {
                    res->excluding.push_back(((DocScalarElement*)nowEle)->getValue());
                }
            }
        }
    }

    /** including (optional) **/
    DocElement *inEle = objEle->get("including");
    if (inEle != NULL) {
        if (inEle->type != DOC_SEQUENCE) {
            Error::addError(new FieldInvalidError(filePath, inEle->line, inEle->col, "x-scenario.modules.checkpoint.including",
                                                  inEle->type, DOC_SEQUENCE));
            delete res;
            return NULL;
        } else {
            DocSequenceElement *seqInEle = (DocSequenceElement*)inEle;
            int len = seqInEle->getLength();
            for (int i=0; i<len; ++i) {
                DocElement *nowEle = seqInEle->get(i);
                if (nowEle->type != DOC_SCALAR) {
                    Error::addError(new FieldInvalidError(filePath, nowEle->line, nowEle->col, "x-scenario.modules.checkpoint.including",
                                                          nowEle->type, DOC_SCALAR));
                    delete res;
                    return NULL;
                } else {
                    res->including.push_back(((DocScalarElement*)nowEle)->getValue());
                }
            }
        }
    }

    return res;
}