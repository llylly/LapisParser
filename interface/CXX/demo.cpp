//
// Created by lly on 01/08/2017.
//

#include <iostream>
#include "../Interface.h"

#define print_obj(y, x) DataObjectAdapter::toDocElement(x)->printTo(y)

string secretKey = "BA8FKxajrR9w47fmzala0ifgUcao7G";

using namespace std;

void sendReq(string name, string method) {
    BaseDataObject *res = runSingleAPI(name, method);
    if (res == NULL) {
        cerr << "Request failed" << endl;
        print_obj(cout, getRuntimeErrors());
    } else {
        cerr << "Request success" << endl;
        print_obj(cout, res);
    }
}

int main(int argc, char** argv) {
    init();

    addDocFromFile("bug1_sample.yaml");
    parseAPI();
    getAPINames();
    parseScenario();
    parseConfig();
    print_obj(cerr, getErrors());
//    Logger::printLog(cerr);

    return 0;

//
////    if (addDocFromFile("map_server.yaml"))
//    if (addDocFromFile("ali_sample.yaml"))
//        cerr << "Add success." << endl;
//    if (parseAPI())
//        cerr << "Parse success." << endl;
//    else
//        print_obj(cout, getErrors());
////    if (!parseScenario()) {
////        cerr << "Scenario parse failed." << endl;
////        print_obj(cout, getErrors());
////        return 0;
////    } else {
////        cerr << "Scenario success." << endl;
////    }
////    if (!parseConfig()) {
////        cerr << "Config parse failed." << endl;
////        print_obj(cout, getErrors());
////        return 0;
////    } else {
////        cerr << "Config success." << endl;
////    }
//
//    /** -------------------- */
//    print_obj(cout, getAPINames());
//
//    BaseDataObject *res = NULL;
//
////    res = runScenario(true);
////    if (res == NULL) {
////        cerr << "Run scenario failed." << endl;
////        print_obj(cout, getRuntimeErrors());
////    } else {
////        cerr << "Run scenario success." << endl;
////        print_obj(cout , res);
////    }
//
//    res = runSingleAPIforAli("DescribeInstances", "get", secretKey);
//    if (res == NULL) {
//        RuntimeError::printError(cerr);
//    } else {
//        print_obj(cout, res);
//    }
//
//    Logger::printLog(cout);
//
//    return 0;
}