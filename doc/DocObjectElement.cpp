//
// Created by lly on 23/04/2017.
//

#include "DocObjectElement.h"

DocObjectElement::DocObjectElement(int line, int col, int level) {
    this->line = line;
    this->col = col;
    this->type = DOC_OBJECT;
    this->level = level;
}

void DocObjectElement::add(std::string key, DocElement *ele) {
    if (this->member.find(key) != this->member.end())
        this->member.erase(key);
    this->member[key] = ele;
}

bool DocObjectElement::erase(std::string key) {
    if (this->member.find(key) != this->member.end()) {
        this->member.erase(key);
        return true;
    }
    return false;
}

DocElement *DocObjectElement::get(std::string key) {
    if (this->member.find(key) == this->member.end())
        return NULL;
    else
        return this->member[key];
}

DocElement **DocObjectElement::editGet(std::string key) {
    if (this->member.find(key) == this->member.end())
        return NULL;
    else
        return &(this->member[key]);
}

int DocObjectElement::getSize() {
    return (int)this->member.size();
}

std::map<std::string, DocElement*> *DocObjectElement::getMemberMap() {
    return &member;
}

void DocObjectElement::printTo(std::ostream &os) {
    for (int i=0; i<this->level; ++i) os << "  ";
    os << "{" << endl;
    for (std::map<std::string, DocElement*>::iterator ite = this->member.begin();
            ite != this->member.end(); ++ite) {
        for (int i=0; i<=this->level; ++i) os << "  ";
        os << ite->first << ": " << endl;
        ite->second->printTo(os);
    }
    for (int i=0; i<this->level; ++i) os << "  ";
    os << "}" << endl;
}

DocObjectElement::~DocObjectElement() {
    for (map<std::string, DocElement*>::iterator ite = this->member.begin();
            ite != this->member.end(); ++ite) {
        /** CAUTION: This place causes memory dirty!
         * After the reference expansion, some doc elements are shared by multiple nodes.
         * But I haven't maintained a reference count...
         * So I don't know when to delete the node...
         * Then I could only give up and leave them unrecycled...
         */
        //delete ite->second;
    }
}

bool DocObjectElement::equals(DocElement *ele) {
    if (ele == NULL) return false;
    if (ele->type != DOC_OBJECT) return false;
    DocObjectElement *objEle = (DocObjectElement*)ele;
    if (this->getSize() != objEle->getSize()) return false;
    for (map<std::string, DocElement*>::iterator ite = this->member.begin();
            ite != this->member.end(); ++ite) {
        if (objEle->get(ite->first) == NULL) return false;
        if (ite->second == NULL) return false;
        if (!ite->second->equals(objEle->get(ite->first))) return false;
    }
    return true;
}