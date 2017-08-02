//
// Created by lly on 17/07/2017.
//

#include "CustomizedSchema.h"


CustomizedSchema::CustomizedSchema(): DataSchemaObject() {
    type = TYPE_CUSTOMIZED;
    path = "";
    generateLabel = "-gen";
    checkLabel = "-check";
}

BaseDataObject *CustomizedSchema::toDataObject() {
    BaseDataObject *obj = DataSchemaObject::toDataObject();
    // --- above are routine ---
    if (obj) {
        ObjectDataObject *o = (ObjectDataObject*)obj;
        ObjectDataObject *cus = new ObjectDataObject();
        (*cus)["path"] = new StringDataObject(path);
        (*cus)["generateLabel"] = new StringDataObject(generateLabel);
        (*cus)["checkLabel"] = new StringDataObject(checkLabel);
        (*o)["x-customize"] = cus;
    }
    return obj;
}

bool CustomizedSchema::check(BaseDataObject *obj) {
    // -- no routine --
    if (obj == NULL) return false;
    if (obj->type == CUSTOMIZED) {
        CustomizedDataObject *o = (CustomizedDataObject*)obj;
        string cmd = "./";
        cmd += path;
        cmd += " ";
        cmd += checkLabel;
        cmd += " ";
        char *tmp = new char[o->len + 1];
        memcpy((void*)tmp, (void*)o->data, (size_t)o->len);
        tmp[o->len] = (char)0;
        cmd += tmp;
        delete[] tmp;

        FILE *fout = popen(cmd.c_str(), "r");
        int ret = -1;
        fscanf(fout, "%d", &ret);
        pclose(fout);

        return (ret == 0);
    }
    return false;
}

BaseDataObject *CustomizedSchema::transform(DocElement *ele) {
    if (!this->valid) return NULL;
    if (ele == NULL) return NULL;
    // --- above are routine ---
    pair<string, bool> ret = DocElementHelper::parseToString(ele);
    if (ret.second) {
        string &s = ret.first;
        int len = (int)s.length();
        unsigned char *buf = new unsigned char[len];
        for (int i=0; i<len; ++i)
            buf[i] = (unsigned char)s[i];
        CustomizedDataObject *c = new CustomizedDataObject(buf, len);
        delete[] buf;
        return c;
    }
    return NULL;
}

BaseDataObject *CustomizedSchema::generate() {
    if (!this->valid) return NULL;
    // --- changed routine ---
    string cmd = "./";
    cmd += path;
    cmd += " ";
    cmd += generateLabel;

    FILE *fout = popen(cmd.c_str(), "r");
    vector<void*> bufs;
    bufs.clear();
    int tot_rd = 0, rd = 0;
    do {
        void *tmp = malloc(1024);
        rd = (int)fread(tmp, 1024, 1, fout);
        if (rd != 0) {
            bufs.push_back(tmp);
            tot_rd += rd;
        }
    } while (rd != 0);
    pclose(fout);

    if (tot_rd == 0) return NULL;

    unsigned char *buf = new unsigned char[tot_rd];
    for (int i=0; i<bufs.size(); ++i) {
        int rest = tot_rd - (i * 1024);
        if (rest > 1024) rest = 1024;
        memcpy(buf + (i * 1024), bufs[i], (size_t)rest);
        free(bufs[i]);
    }

    CustomizedDataObject *c = new CustomizedDataObject(buf, tot_rd);
    delete[] buf;
    return c;
}

bool CustomizedSchema::init(string filePath, DocObjectElement *obj, int schemaType) {
    if (schemaType != DataSchemaObjectFactory::NORMAL_SCHEMA)
        return false;

    DocElement *ro = obj->get("x-customize");
    if (ro == NULL) {
        return false;
    }
    if (ro->type != DOC_OBJECT) {
        Error::addError(
                new FieldInvalidError(filePath, ro->line, ro->col, "schema.x-customize", ro->type, DOC_OBJECT)
        );
        return false;
    }
    DocObjectElement *r = (DocObjectElement*)ro;

    /* path */
    DocElement *pathEle = r->get("path");
    if (pathEle == NULL) {
        Error::addError(
                new FieldMissError(filePath, r->line, r->col, "schema.x-customize.path")
        );
        return false;
    }
    pair<string, bool> pathRet = DocElementHelper::parseToString(pathEle);
    if (pathRet.second) {
        string &s = pathRet.first;
        // exist, readable and executable
        int ret = access(s.c_str(), R_OK | F_OK | X_OK);
        if (ret != 0) {
            Error::addError(
                    new FieldIllegalError(filePath, pathEle->line, pathEle->col, "schema.x-customize.path")
            );
            return false;
        }
    } else {
        Error::addError(
                new FieldInvalidError(filePath, pathEle->line, pathEle->col, "schema.x-customize.path", pathEle->type, DOC_SCALAR)
        );
        return false;
    }

    /* generateLabel */
    DocElement *labelEle = r->get("generateLabel");
    if (labelEle) {
        pair<string, bool> ret = DocElementHelper::parseToString(labelEle);
        if (ret.second) {
            this->generateLabel = ret.first;
        } else {
            Error::addError(
                    new FieldInvalidError(filePath, labelEle->line, labelEle->col, "schema.x-customize.generateLabel", labelEle->type, DOC_SCALAR)
            );
            return false;
        }
    }

    /* checkLabel */
    DocElement *checkEle = r->get("checkLabel");
    if (checkEle) {
        pair<string, bool> ret = DocElementHelper::parseToString(checkEle);
        if (ret.second) {
            this->checkLabel = ret.first;
        } else {
            Error::addError(
                    new FieldInvalidError(filePath, checkEle->line, checkEle->col, "schema.x-customize.checkLabel", checkEle->type, DOC_SCALAR)
            );
            return false;
        }
    }

    return true;
}

DataSchemaObject *CustomizedSchema::findField(const vector<string> &fieldVec, int index) {
    if (index == fieldVec.size())
        return this;
    else
        return NULL;
}