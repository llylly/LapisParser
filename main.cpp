#include <iostream>
#include "parser/libyaml/yaml.h"
#include "doc/DocElement.h"
#include "doc/YAMLAdapter.h"

using namespace std;

int main() {
    FILE *fin = fopen("input.yaml", "r");
    DocElement *root = YAMLAdapter::parseDoc(fin);
    fclose(fin);
    DocElement::docs.push_back(root);
    root->printTo(cout);
    return 0;
}