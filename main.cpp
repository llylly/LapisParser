#include <iostream>
#include "doc/DocElement.h"
#include "doc/YAMLAdapter.h"

using namespace std;

int main() {
    FILE *fin = fopen("input.yaml", "r");
    DocElement *root = YAMLAdapter::parseDoc(fin);
    fclose(fin);
    if (Error::hasError()) {
        Error::printError(cerr);
        return 0;
    }
    DocElement::docs[string("input.yaml")] = root;
    root->printTo(cout);
    return 0;
}