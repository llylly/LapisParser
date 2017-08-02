//
// Created by lly on 01/08/2017.
//

#include <iostream>
#include "../Interface.h"

using namespace std;

int main(int argc, char** argv) {
    init();

    if (addDocFromFile("official_demo.yaml"))
        cerr << "Add success." << endl;
    if (parseAPI())
        cerr << "Parse success." << endl;
    else
        DataObjectAdapter::toDocElement(getErrors())->printTo(cout);
    if (DataObjectAdapter::toDocElement(getInfo()))
        DataObjectAdapter::toDocElement(getInfo())->printTo(cout);

    return 0;
}