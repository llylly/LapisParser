//
// Created by lly on 23/04/2017.
//

#ifndef VPARSER_DOCELEMENT_H
#define VPARSER_DOCELEMENT_H

#include <string>
#include <map>
#include <iostream>

using namespace std;

enum DOC_ELEMENT_TYPE {
    DOC_OBJECT, DOC_SCALAR, DOC_SEQUENCE
};

class DocElement {
public:
    DOC_ELEMENT_TYPE type;
    int level;
    int line, col;

    virtual void printTo(std::ostream &os) = 0;
    virtual ~DocElement();

    static map<string, DocElement*> docs;
};

class DocElementHelper {
public:
    static pair<long long, bool> parseToInt(DocElement *ele);
    static pair<double, bool> parseToDouble(DocElement *ele);
    static pair<string, bool> parseToString(DocElement *ele);
    static pair<bool, bool> parserToBool(DocElement *ele, string trueString = "true", string falseString = "false");
};


#endif //VPARSER_DOCELEMENT_H
