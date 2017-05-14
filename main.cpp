#include <iostream>
#include "doc/DocElement.h"
#include "doc/YAMLAdapter.h"
#include "doc/XMLAdapter.h"
#include "RefExpand/RefExpand.h"

using namespace std;

int main() {
    /*
    Error::curFileName = "example-ossgroup.yaml";
    DocElement *root1 = YAMLAdapter::parseDoc("example-ossgroup.yaml");

    if (Error::hasError()) {
        Error::printError(cerr);
        return 0;
    }

    DocElement::docs[string("example-ossgroup.yaml")] = root1;

    XMLAdapter::saveFile("example-ossgroup.xml", root1);
     */

    Error::curFileName = "xmlTest.xml";
    DocElement *root2 = XMLAdapter::parseDoc("xmlTest.xml");

    if (Error::hasError()) {
        Error::printError(cerr);
        return 0;
    }
    root2->printTo(cout);

    RefExpand *refExpand = new RefExpand();
    refExpand->work();

    return 0;
}