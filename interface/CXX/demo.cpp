//
// Created by lly on 01/08/2017.
//

#include <iostream>
#include "../Interface.h"

#define print_obj(y, x) DataObjectAdapter::toDocElement(x)->printTo(y)

using namespace std;

int main(int argc, char** argv) {
    init();

    if (addDocFromFile("real_demo.yaml"))
        cerr << "Add success." << endl;
    if (parseAPI())
        cerr << "Parse success." << endl;
    else
        print_obj(cout, getErrors());
    if (!parseScenario()) {
        cerr << "Scenario parse failed." << endl;
        print_obj(cout, getErrors());
        return 0;
    } else {
        cerr << "Scenario success." << endl;
    }
    if (!parseConfig()) {
        cerr << "Config parse failed." << endl;
        print_obj(cout, getErrors());
        return 0;
    } else {
        cerr << "Config success" << endl;
    }

    /** -------------------- */
    print_obj(cout, getAPINames());
//    runSingleAPI("map/gen_test1", "get");
//    runSingleAPI("add/{op1}/{op2}", "get");
//    runSingleAPI("DescribeInstanceStatus", "get");
//    runSingleAPI("test/arrtest", "get");
//    runSingleAPI("mul/{op1}/{op2}", "get");
    runSingleAPI("map/insert", "get");
    runSingleAPI("map/get_map", "get");

    return 0;
}