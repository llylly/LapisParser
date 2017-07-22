//
// Created by lly on 17/07/2017.
//

#include "FileSchema.h"

FileSchema::FileSchema(): DataSchemaObject() {
    type = TYPE_FILE;

}

BaseDataObject *FileSchema::toDataObject() {
    BaseDataObject *obj = DataSchemaObject::toDataObject();
    return obj;
}

bool FileSchema::check(BaseDataObject *obj) {
    if (DataSchemaObject::check(obj)) {
        if (obj == NULL) return true;
        // allow empty value
        // --- above are routine ---
        if (obj->type == BYTEFILE)
            return true;
    }
    return false;
}

BaseDataObject *FileSchema::transform(DocElement *ele) {
    if (!this->valid) return NULL;
    if (ele == NULL) return NULL;
    // --- above are routine ---
    pair<string, bool> res = DocElementHelper::parseToString(ele);
    if (res.second) {
        string &s = res.first;
        int len = (int)s.length();
        unsigned char *c = new unsigned char[len];
        for (int i=0; i<len; ++i)
            c[i] = (unsigned char)s[i];
        FileDataObject *f = new FileDataObject(c, len);
        delete[] c;
        return f;
    }
    return NULL;
}

BaseDataObject *FileSchema::generate() {
    BaseDataObject *enumGen = DataSchemaObject::generate();
    if (enumGen) return enumGen;
    if (!this->valid) return NULL;
    if ((DataSchemaObject::randomReal() < emptyProbability) && (this->allowEmptyValue)) return NULL;
    // --- above are routine ---
    int len = rand() % (1048576);
    unsigned char *uarr = new unsigned char[len];
    for (int i=0; i<len; ++i)
        uarr[i] = (unsigned char)(rand() & 0xFF);
    FileDataObject *f = new FileDataObject(uarr, len);
    delete[] uarr;
    return f;
}

bool FileSchema::init(string filePath, DocObjectElement *obj, int schemaType) {
    return true;
}