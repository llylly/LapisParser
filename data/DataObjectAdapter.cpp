//
// Created by lly on 16/07/2017.
//

#include "DataObjectAdapter.h"
#include "SequenceDataObject.h"
#include "../doc/DocSequenceElement.h"
#include "ObjectDataObject.h"
#include "../doc/DocObjectElement.h"


DocElement *DataObjectAdapter::toDocElement(BaseDataObject *origin) {
    if (origin == NULL) return NULL;
    if (origin->type == BASE) {
        return NULL;
    }
    if (origin->type == INTEGER) {
        stringstream s;
        s << ((IntegerDataObject*)(origin))->value;
        s.flush();
        return new DocScalarElement(1, 1, s.str());
    }
    if (origin->type == NUMBER) {
        stringstream s;
        s << ((NumberDataObject*)(origin))->value;
        s.flush();
        return new DocScalarElement(1, 1, s.str());
    }
    if (origin->type == BOOLEAN) {
        stringstream s;
        s << ((IntegerDataObject*)(origin))->value;
        s.flush();
        return new DocScalarElement(1, 1, s.str());
    }
    if (origin->type == STRING) {
        return new DocScalarElement(1, 1, ((StringDataObject*)(origin))->str);
    }
    if (origin->type == BYTEFILE) {
        FileDataObject *o = (FileDataObject*)origin;
        char *c = (char*)o->data;
        char *newC = new char[o->len + 1];
        strcpy(newC, c);
        newC[o->len] = 0;
        return new DocScalarElement(1, 1, string(newC));
    }
    if (origin->type == CUSTOMIZED) {
        CustomizedDataObject *o = (CustomizedDataObject*)origin;
        char *c = (char*)o->data;
        char *newC = new char[o->len + 1];
        strcpy(newC, c);
        newC[o->len] = 0;
        return new DocScalarElement(1, 1, string(newC));
    }
    if (origin->type == SEQUENCE) {
        SequenceDataObject *o = (SequenceDataObject*)origin;
        DocSequenceElement *seq = new DocSequenceElement(1, 1);
        int len = o->length();
        for (int i=0; i<len; ++i) {
            BaseDataObject *item = (*o)[i];
            seq->add(DataObjectAdapter::toDocElement(item));
        }
        return seq;
    }
    if (origin->type == OBJECT) {
        ObjectDataObject *o = (ObjectDataObject*)origin;
        DocObjectElement *ele = new DocObjectElement(1, 1);
        for (map<string, BaseDataObject*>::iterator ite = o->m.begin();
                ite != o->m.end();
                ++ite) {
            ele->add(ite->first, DataObjectAdapter::toDocElement(ite->second));
        }
        return ele;
    }
    return NULL;
}

BaseDataObject *DataObjectAdapter::defaultFromDocElement(DocElement *ele) {
    if (ele->type == DOC_SCALAR) {
        DocScalarElement *e = (DocScalarElement*)ele;
        return new StringDataObject(e->getValue());
    }
    if (ele->type == DOC_SEQUENCE) {
        DocSequenceElement *e = (DocSequenceElement*)ele;
        SequenceDataObject *obj = new SequenceDataObject();
        int len = e->getLength();
        for (int i=0; i<len; ++i)
            obj->push(DataObjectAdapter::defaultFromDocElement(e->get(i)));
        return obj;
    }
    if (ele->type == DOC_OBJECT) {
        DocObjectElement *e = (DocObjectElement*)ele;
        ObjectDataObject *obj = new ObjectDataObject();
        map<string, DocElement*> *m = e->getMemberMap();
        for (map<string, DocElement*>::iterator ite = m->begin();
                ite != m->end();
                ++ite) {
            (*obj)[ite->first] = DataObjectAdapter::defaultFromDocElement(ite->second);
        }
        return obj;
    }
    return NULL;
}

BaseDataObject *DataObjectAdapter::deepCopy(const BaseDataObject *origin) {
    BaseDataObject *dest = NULL;
    if (origin == NULL) return dest;
    if (origin->type == NUMBER) {
        NumberDataObject *d = (NumberDataObject*)origin;
        dest = new NumberDataObject(*d);
    }
    if (origin->type == INTEGER) {
        IntegerDataObject *d = (IntegerDataObject*)origin;
        dest = new IntegerDataObject(*d);
    }
    if (origin->type == BOOLEAN) {
        BooleanDataObject *d = (BooleanDataObject*)origin;
        dest = new BooleanDataObject(*d);
    }
    if (origin->type == STRING) {
        StringDataObject *d = (StringDataObject*)origin;
        dest = new StringDataObject(*d);
    }
    if (origin->type == BYTEFILE) {
        FileDataObject *d = (FileDataObject*)origin;
        dest = new FileDataObject(*d);
    }
    if (origin->type == SEQUENCE) {
        SequenceDataObject *d = (SequenceDataObject*)origin;
        dest = new SequenceDataObject(*d);
    }
    if (origin->type == OBJECT) {
        ObjectDataObject *d = (ObjectDataObject*)origin;
        dest = new ObjectDataObject(*d);
    }
    if (origin->type == CUSTOMIZED) {
        CustomizedDataObject *d = (CustomizedDataObject*)origin;
        dest = new CustomizedDataObject(*d);
    }
    return dest;
}