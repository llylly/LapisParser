//
// Created by lly on 17/07/2017.
//

#include "ObjectSchema.h"


ObjectSchema::ObjectSchema(): DataSchemaObject() {
    type = TYPE_OBJECT;

}

BaseDataObject *ObjectSchema::toDataObject() {
    /**
     * TODO
     */
    return NULL;
}

bool ObjectSchema::check(BaseDataObject *obj) {
    /**
     * TODO
     */
    return false;
}

BaseDataObject *ObjectSchema::transform(DocElement *ele) {
    /**
     * TODO
     */
    return NULL;
}

BaseDataObject *ObjectSchema::generate() {
    /**
     * TODO
     */
    return NULL;
}

bool ObjectSchema::init(string filePath, DocObjectElement *obj, int schemaType) {
    /**
     * TODO
     */
    return false;
}