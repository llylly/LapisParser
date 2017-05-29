//
// Created by lly on 29/05/2017.
//

#include "TagsObject.h"
#include "../error/FieldInvalidError.h"
#include "../doc/DocScalarElement.h"
#include "../error/FieldMissError.h"

TagsObject *TagsObjectFactory::create(string filePath, DocSequenceElement *ele) {
    TagsObject *obj = new TagsObject();
    obj->vec.clear();
    for (int i=0; i<ele->getLength(); ++i) {
        DocElement *priItem;
        if ((priItem = ele->get(i))->type != DOC_OBJECT) {
            Error::addError(new FieldInvalidError(filePath, priItem->line, priItem->col, "tags", priItem->type, DOC_OBJECT));
            delete obj;
            return NULL;
        } else {
            DocObjectElement *item = (DocObjectElement*)priItem;
            TagItem tagItem;
            tagItem.hasDescription = false;
            tagItem.hasExternalDocObject = false;
            DocElement *name = item->get("name");
            if ((name == NULL) || (name->type != DOC_SCALAR)) {
                if (name == NULL)
                    Error::addError(new FieldMissError(filePath, item->line, item->col, "tags.name"));
                else
                    Error::addError(new FieldInvalidError(filePath, item->line, item->col, "tags.name", name->type, DOC_SCALAR));
                delete obj;
                return NULL;
            } else {
                tagItem.name = ((DocScalarElement*)name)->getValue();
            }
            DocElement *description = item->get("description");
            if (description != NULL) {
                if (description->type != DOC_SCALAR) {
                    Error::addError(new FieldInvalidError(filePath, description->line, description->col, "tags.description", description->type, DOC_SCALAR));
                    delete obj;
                    return NULL;
                } else {
                    tagItem.hasDescription = true;
                    tagItem.description = ((DocScalarElement*)description)->getValue();
                }
            }
            DocElement *externalDocs = item->get("externalDocs");
            if (externalDocs != NULL) {
                if (externalDocs->type != DOC_OBJECT) {
                    Error::addError(new FieldInvalidError(filePath, externalDocs->line, externalDocs->col, "tags.externalDocs", externalDocs->type, DOC_OBJECT));
                    return obj;
                    return NULL;
                } else {
                    ExternalDocObject *ans = ExternalDocObjectFactory::create(filePath, "tags.externalDocs", (DocObjectElement*)externalDocs);
                    if (ans == NULL) {
                        delete obj;
                        return NULL;
                    } else {
                        tagItem.hasExternalDocObject = true;
                        tagItem.externalDocObject = *ans;
                        delete ans;
                    }
                }
            }
            obj->vec.push_back(tagItem);
        }
    }
    return obj;
}