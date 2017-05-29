#include <iostream>
#include "doc/DocElement.h"
#include "doc/YAMLAdapter.h"
#include "doc/XMLAdapter.h"
#include "ref_expand/RefExpand.h"
#include "schema/Controller.h"

using namespace std;

int main() {
    //----- phase I -----

    Error::curFileName = "example-ossgroup.yaml";
    DocElement *root1 = YAMLAdapter::parseDoc("example-ossgroup.yaml");

    if (Error::hasError()) {
        Error::printError(cerr);
        return 0;
    }

    DocElement::docs[string("example-ossgroup.yaml")] = root1;


    Error::curFileName = "test.yaml";
    DocElement *root2 = YAMLAdapter::parseDoc("test.yaml");

    if (Error::hasError()) {
        Error::printError(cerr);
        return 0;
    }

    DocElement::docs[string("test.yaml")] = root2;

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