#include <iostream>
#include "doc/DocElement.h"
#include "doc/YAMLAdapter.h"
#include "doc/XMLAdapter.h"
#include "ref_expand/RefExpand.h"
#include "schema/Controller.h"

using namespace std;

int main() {
    //----- phase I -----

    Error::curFileName = "input2.yaml";
    DocElement *root1 = YAMLAdapter::parseDoc("input2.yaml");

    if (Error::hasError()) {
        Error::printError(cerr);
        return 0;
    }

    DocElement::docs[string("input2.yaml")] = root1;

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

    controller->printTo(cout);

    return 0;
}