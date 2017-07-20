//
// Created by lly on 17/07/2017.
//

#include "CustomizedSchema.h"


CustomizedSchema::CustomizedSchema(): DataSchemaObject() {
    type = TYPE_CUSTOMIZED;

}

BaseDataObject *CustomizedSchema::toDataObject() {
    /**
     * TODO
     */
    return NULL;
}

bool CustomizedSchema::check(BaseDataObject *obj) {
    /**
     * TODO
     */
    return false;
}

BaseDataObject *CustomizedSchema::transform(DocElement *ele) {
    /**
     * TODO
     */
    return NULL;
}

BaseDataObject *CustomizedSchema::generate() {
    /**
     * TODO
     */
    return NULL;
}

bool CustomizedSchema::init(string filePath, DocObjectElement *obj, int schemaType) {
    /**
     * TODO
     */
    return false;
}