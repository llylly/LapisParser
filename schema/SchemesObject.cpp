//
// Created by lly on 29/05/2017.
//

#include "SchemesObject.h"
#include "Utility.h"
#include "../error/Error.h"
#include "../error/FieldInvalidError.h"
#include "../error/InvalidSchemeError.h"
#include "../data/SequenceDataObject.h"
#include "../data/StringDataObject.h"

SchemesObject::SchemesObject() {
    this->http = false;
    this->https = false;
}

SchemesObject *SchemesObjectFactory::create(string filePath, DocSequenceElement *obj) {
    SchemesObject *schemes = new SchemesObject();
    schemes->http = false;
    schemes->https = false;
    for (int i=0; i<obj->getLength(); ++i) {
        DocScalarElement *ele;
        if (obj->get(i)->type != DOC_SCALAR) {
            Error::addError(new FieldInvalidError(filePath, obj->get(i)->line, obj->get(i)->col, "schemes", obj->get(i)->type, DOC_SCALAR));
            delete schemes;
            return NULL;
        } else {
            ele = (DocScalarElement*) obj->get(i);
            SUPPORT_SCHEME v = Utility::toScheme(ele->getValue());
            if (v == INVALID) {
                Error::addError(new InvalidSchemeError(filePath, ele->line, ele->col));
                delete schemes;
                return NULL;
            } else
            if (v == HTTP) {
                schemes->http = true;
            } else
            if (v == HTTPS) {
                schemes->https = true;
            }
        }
    }
    return schemes;
}

BaseDataObject *SchemesObject::toDataObject() {
    SequenceDataObject *obj = new SequenceDataObject();
    if (this->http)
        obj->push(new StringDataObject("http"));
    if (this->https)
        obj->push(new StringDataObject("https"));
    return obj;
}