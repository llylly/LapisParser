//
// Created by lly on 01/08/2017.
//

#include <iostream>
#include "../Interface.h"

#define print_obj(y, x) DataObjectAdapter::toDocElement(x)->printTo(y)

using namespace std;

int main(int argc, char** argv) {
    init();

    if (addDocFromFile("official_demo.yaml"))
        cerr << "Add success." << endl;
    if (parseAPI())
        cerr << "Parse success." << endl;
    else
        print_obj(cout, getErrors());
    if (DataObjectAdapter::toDocElement(getInfo()))
        DataObjectAdapter::toDocElement(getInfo())->printTo(cout);
    if (!parseScenario()) {
        cerr << "Scenario parse failed." << endl;
        print_obj(cout, getErrors());
        return 0;
    } else {
        DataObjectAdapter::toDocElement(getErrors())->printTo(cout);
    }
    print_obj(cout, getScenarioNames());
    print_obj(cout, getScenario("a"));
    print_obj(cout, getScenario("b"));
    return 0;
}