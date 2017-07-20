//
// Created by lly on 28/05/2017.
//

#include "InfoObject.h"
#include "../error/Error.h"
#include "../error/FieldMissError.h"
#include "../doc/DocScalarElement.h"
#include "../error/FieldInvalidError.h"
#include "../data/ObjectDataObject.h"
#include "../data/StringDataObject.h"

using namespace std;

InfoObject::InfoObject() {
    this->hasDescription = false;
    this->hasTermsOfService = false;
    this->hasContact = false;
    this->hasLicense = false;
    this->contact.hasUrl = false;
    this->contact.hasName = false;
    this->contact.hasEmail = false;
    this->license.hasUrl = false;
}

InfoObject *InfoObjectFactory::create(string filePath, DocObjectElement *obj) {
    InfoObject *info = new InfoObject();

    DocElement *titleEle = obj->get("title");
    if ((titleEle == NULL) || (titleEle->type != DOC_SCALAR)) {
        Error::addError(new FieldMissError(filePath, obj->line, obj->col, "info.title"));
        delete info;
        return NULL;
    }
    info->title = ((DocScalarElement*)titleEle)->getValue();

    info->hasDescription = false;
    DocElement *descriptionEle = obj->get("description");
    if (descriptionEle) {
        if (descriptionEle->type != DOC_SCALAR) {
            Error::addError(new FieldInvalidError(filePath, obj->line, obj->col, "info.description", descriptionEle->type, DOC_SCALAR));
            delete info;
            return NULL;
        } else {
            info->description = ((DocScalarElement*)descriptionEle)->getValue();
            info->hasDescription = true;
        }
    }

    info->hasTermsOfService = false;
    DocElement *termsOfServiceEle = obj->get("termsOfService");
    if (termsOfServiceEle) {
        if (termsOfServiceEle->type != DOC_SCALAR) {
            Error::addError(new FieldInvalidError(filePath, obj->line, obj->col, "info.termsOfService", termsOfServiceEle->type, DOC_SCALAR));
            delete info;
            return NULL;
        } else {
            info->termsOfService = ((DocScalarElement*)termsOfServiceEle)->getValue();
            info->hasTermsOfService = true;
        }
    }

    info->hasContact = false;
    DocElement *contactEle = obj->get("contact");
    if (contactEle) {
        if (contactEle->type != DOC_OBJECT) {
            Error::addError(new FieldInvalidError(filePath, obj->line, obj->col, "info.contact", contactEle->type, DOC_OBJECT));
            delete info;
            return NULL;
        } else {
            info->hasContact = true;

            DocObjectElement *objEle = (DocObjectElement*)contactEle;

            info->contact.hasName = false;
            DocElement *nameEle = objEle->get("name");
            if (nameEle) {
                if (nameEle->type != DOC_SCALAR) {
                    Error::addError(new FieldInvalidError(filePath, nameEle->line, nameEle->col, "info.contact.name", nameEle->type, DOC_SCALAR));
                    delete info;
                    return NULL;
                } else {
                    info->contact.name = ((DocScalarElement*)nameEle)->getValue();
                    info->contact.hasName = true;
                }
            }

            info->contact.hasUrl = false;
            DocElement *urlEle = objEle->get("url");
            if (urlEle) {
                if (urlEle->type != DOC_SCALAR) {
                    Error::addError(new FieldInvalidError(filePath, urlEle->line, urlEle->col, "info.contact.url", urlEle->type, DOC_SCALAR));
                    delete info;
                    return NULL;
                } else {
                    info->contact.url = ((DocScalarElement*)urlEle)->getValue();
                    info->contact.hasUrl = true;
                }
            }

            info->contact.hasEmail = false;
            DocElement *emailEle = objEle->get("email");
            if (emailEle) {
                if (emailEle->type != DOC_SCALAR) {
                    Error::addError(new FieldInvalidError(filePath, emailEle->line, emailEle->col, "info.contact.email", emailEle->type, DOC_SCALAR));
                    delete info;
                    return NULL;
                } else {
                    info->contact.email = ((DocScalarElement*)emailEle)->getValue();
                    info->contact.hasEmail = true;
                }
            }
        }
    }

    info->hasLicense = false;
    DocElement *licenseEle = obj->get("license");
    if (licenseEle) {
        if (licenseEle->type != DOC_OBJECT) {
            Error::addError(new FieldInvalidError(filePath, obj->line, obj->col, "info.license", licenseEle->type, DOC_OBJECT));
            delete info;
            return NULL;
        } else {
            info->hasLicense = true;

            DocObjectElement *objEle = (DocObjectElement*)licenseEle;

            DocElement *nameEle = objEle->get("name");
            if ((nameEle == NULL) || (nameEle->type != DOC_SCALAR)) {
                Error::addError(new FieldMissError(filePath, objEle->line, objEle->col, "info.license.name"));
                delete info;
                return NULL;
            }
            info->license.name = ((DocScalarElement*)nameEle)->getValue();


            info->license.hasUrl = false;
            DocElement *urlEle = objEle->get("url");
            if (urlEle) {
                if (urlEle->type != DOC_SCALAR) {
                    Error::addError(new FieldInvalidError(filePath, urlEle->line, urlEle->col, "info.license.url", urlEle->type, DOC_SCALAR));
                    delete info;
                    return NULL;
                } else {
                    info->license.url = ((DocScalarElement*)urlEle)->getValue();
                    info->license.hasUrl = true;
                }
            }
        }
    }

    DocElement *versionEle = obj->get("version");
    if ((versionEle == NULL) || (versionEle->type != DOC_SCALAR)) {
        Error::addError(new FieldMissError(filePath, obj->line, obj->col, "info.version"));
        delete info;
        return NULL;
    }
    info->version = ((DocScalarElement*)versionEle)->getValue();

    return info;
}

BaseDataObject* InfoObject::toDataObject() {
    ObjectDataObject *obj = new ObjectDataObject();
    (*obj)["title"] = new StringDataObject(this->title);
    if (this->hasDescription)
        (*obj)["description"] = new StringDataObject(this->description);
    if (this->hasTermsOfService)
        (*obj)["termsOfService"] = new StringDataObject(this->termsOfService);
    if (this->hasContact) {
        ObjectDataObject *objC = new ObjectDataObject();
        if (this->contact.hasName)
            (*objC)["name"] = new StringDataObject(this->contact.name);
        if (this->contact.hasUrl)
            (*objC)["url"] = new StringDataObject(this->contact.url);
        if (this->contact.hasEmail)
            (*objC)["email"] = new StringDataObject(this->contact.email);
        (*obj)["contact"] = objC;
    }
    if (this->hasLicense) {
        ObjectDataObject *objL = new ObjectDataObject();
        (*objL)["name"] = new StringDataObject(this->license.name);
        if (this->license.hasUrl)
            (*objL)["url"] = new StringDataObject(this->license.url);
        (*obj)["license"] = objL;
    }
    (*obj)["version"] = new StringDataObject(this->version);
    return obj;
}