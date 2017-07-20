//
// Created by lly on 28/05/2017.
//

#ifndef VPARSER_INFOOBJECT_H
#define VPARSER_INFOOBJECT_H

#include <string>
#include "BaseObject.h"
#include "../doc/DocObjectElement.h"

using namespace std;

struct Contact {
    string name;
    string url;
    string email;
    bool hasName;
    bool hasUrl;
    bool hasEmail;
};

struct License {
    string name;
    string url;
    bool hasUrl;
};

struct InfoObject: public BaseObject {
    string title;

    string description;
    bool hasDescription;

    string termsOfService;
    bool hasTermsOfService;

    Contact contact;
    bool hasContact;

    License license;
    bool hasLicense;

    string version;

public:
    InfoObject();
    BaseDataObject* toDataObject() override;
};

struct InfoObjectFactory {
    static InfoObject *create(string filePath, DocObjectElement *obj);
};

#endif //VPARSER_INFOOBJECT_H

