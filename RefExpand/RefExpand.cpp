//
// Created by lly on 08/05/2017.
//

#include "RefExpand.h"
#include "../doc/DocElement.h"
#include "../doc/DocSequenceElement.h"
#include "../doc/DocObjectElement.h"


void RefExpand::work() {
    pathStack.clear();
    eleStack.clear();
    bool expanded;
    do {
        expanded = false;
        for (map<string, DocElement *>::iterator ite = DocElement::docs.begin(); ite != DocElement::docs.end(); ++ite) {
            curFile = ite->first;
            expanded |= visitor(ite->second);
        }
    } while (expanded);
}

bool RefExpand::visitor(DocElement *ele) {
    if (ele == NULL) return false;
    if (ele->type == DOC_SCALAR) {
        return false;
    }
    if (ele->type == DOC_SEQUENCE) {
        DocSequenceElement *seqEle = (DocSequenceElement*)ele;
        //for (std::vector<DocElement*>::iterator ite = ca)
    }
    if (ele->type == DOC_OBJECT) {

    }
    return false;
}

DocElement* RefExpand::finder(string path) {

}
