//
// Created by lly on 17/07/2017.
//

#include "ArraySchema.h"

ArraySchema::ArraySchema(): DataSchemaObject() {
    type = TYPE_ARRAY;
    items = NULL;
    collectionFormat = CSV;
    maxItems = 255;
    minItems = 0;
    uniqueItems = false;
}

BaseDataObject *ArraySchema::toDataObject() {
    /**
     * TODO
     */
    return NULL;
}

bool ArraySchema::check(BaseDataObject *obj) {
    /**
     * TODO
     */
     return false;
}

BaseDataObject *ArraySchema::transform(DocElement *ele) {
    /**
     * TODO
     */
     return NULL;
}

BaseDataObject *ArraySchema::generate() {
    /**
     * TODO
     */
    return NULL;
}

bool ArraySchema::init(string filePath, DocObjectElement *obj, int schemaType) {
    /**
     * TODO
     */
    return false;
}