//
// Created by lly on 29/05/2017.
//

#include "EnumObject.h"
#include "../error/FieldInvalidError.h"
#include "../doc/DocScalarElement.h"

EnumObject *EnumObjectFactory::create(string filePath, string nodeName, DocSequenceElement *ele) {
    EnumObject *obj = new EnumObject();
    obj->vec.clear();
    for (int i=0; i<ele->getLength(); ++i) {
        DocElement *item = ele->get(i);
        if (item->type == DOC_SCALAR) {
            obj->vec.push_back(((DocScalarElement*)item)->getValue());
        } else {
            Error::addError(new FieldInvalidError(filePath, item->line, item->col, nodeName, item->type, DOC_SCALAR));
            delete obj;
            return NULL;
        }
    }
    return obj;
}