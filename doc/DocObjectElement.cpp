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

DocElement *DocObjectElement::get(std::string key) {
    if (this->member.find(key) == this->member.end())
        return NULL;
    else
        return this->member[key];
}

int DocObjectElement::getSize() {
    return (int)this->member.size();
}

void DocObjectElement::printTo(std::ostream &os) {
    for (int i=0; i<this->level; ++i) os << "  ";
    os << "[object]" << endl;
    for (std::map<std::string, DocElement*>::iterator ite = this->member.begin();
            ite != this->member.end(); ++ite) {
        for (int i=0; i<=this->level; ++i) os << "  ";
        os << "key: " << ite->first << endl;
        ite->second->printTo(os);
    }
}