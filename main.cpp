#include <iostream>
#include "doc/DocElement.h"
#include "doc/YAMLAdapter.h"
#include "doc/XMLAdapter.h"
#include "ref_expand/RefExpand.h"
#include "schema/Controller.h"
#include "error/Error.h"

using namespace std;

bool run = true;

int main() {
    if (!run)
        printf("Build succeed!");
    else {

        string files[] = {"test.yaml"};
        int totFile = 1;

        //----- Clean all -----
        for (map<string, DocElement*>::iterator ite = DocElement::docs.begin(); ite != DocElement::docs.end(); ++ite)
            delete ite->second;
        DocElement::docs.clear();

        std::vector<Error*> *errs = Error::getErrors();
        for (std::vector<Error*>::iterator ite = errs->begin(); ite != errs->end(); ++ite)
            delete *ite;
        errs->clear();

        //----- Stage I -----
        for (int i=0; i<totFile; ++i) {
            Error::curFileName = files[i];
            DocElement *curDoc = YAMLAdapter::parseDoc(files[i].c_str());
            DocElement::docs[files[i]] = curDoc;
        }
        if (Error::hasError()) {
            Error::printError(cerr);
            return 0;
        }

        //----- phase II -----
        RefExpand *refExpand = new RefExpand();
        refExpand->work();
        if (Error::hasError()) {
            Error::printError(cerr);
            return 0;
        }

        //----- phase III -----
        Controller *controller = new Controller();
        controller->work();
        if (Error::hasError()) {
            Error::printError(cerr);
            return 0;
        }
    }
    return 0;
}