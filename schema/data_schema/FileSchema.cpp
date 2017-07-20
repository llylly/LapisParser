//
// Created by lly on 17/07/2017.
//

#include "FileSchema.h"



FileSchema::FileSchema(): DataSchemaObject() {
    type = TYPE_FILE;

}

BaseDataObject *FileSchema::toDataObject() {
    /**
     * TODO
     */
    return NULL;
}

bool FileSchema::check(BaseDataObject *obj) {
    /**
     * TODO
     */
    return false;
}

BaseDataObject *FileSchema::transform(DocElement *ele) {
    /**
     * TODO
     */
    return NULL;
}

BaseDataObject *FileSchema::generate() {
    /**
     * TODO
     */
    return NULL;
}

bool FileSchema::init(string filePath, DocObjectElement *obj, int schemaType) {
    /**
     * TODO
     */
    return false;
}