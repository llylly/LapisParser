//
// Created by lly on 23/04/2017.
//

#include <sstream>
#include "DocElement.h"
#include "DocScalarElement.h"

map <string, DocElement*> DocElement::docs;

DocElement::~DocElement() { }

void DocElement::cleanAllDocs() {
    for (map<string, DocElement*>::iterator ite = docs.begin();
            ite != docs.end();
            ++ite) {
        delete ite->second;
    }
    docs.clear();
}

pair<long long, bool> DocElementHelper::parseToInt(DocElement *ele) {
    long long ans;
    if (ele->type == DOC_SCALAR) {
        string str = ((DocScalarElement*)ele)->getValue();
        for (int i=0; i<str.length(); ++i) {
            if ((str[i] >= '0') && (str[i] <= '9')) continue;
            if (((str[i] == '-') || (str[i] == '+')) && (i == 0) && (str.length() > 1)) continue;
            return make_pair(-1LL, false);
        }
        stringstream s;
        s.clear();
        s << str;
        s >> ans;
        return make_pair(ans, true);
    }
    return make_pair(-1LL, false);
}

pair<double, bool> DocElementHelper::parseToDouble(DocElement *ele) {
    double ans;
    if (ele->type == DOC_SCALAR) {
        string str = ((DocScalarElement*)ele)->getValue();
        for (int i=0; i<str.length(); ++i) {
            if ((str[i] >= '0') && (str[i] <= '9')) continue;
            if (str[i] == '.') continue;
            if (((str[i] == '-') || (str[i] == '+')) && (i == 0) && (str.length() > 1)) continue;
            return make_pair(-1.0, false);
        }
        stringstream s;
        s.clear();
        s << str;
        s >> ans;
        return make_pair(ans, true);
    }
    return make_pair(-1.0, false);
}

pair<string, bool> DocElementHelper::parseToString(DocElement *ele) {
    string ans;
    if (ele->type == DOC_SCALAR) {
        string str = ((DocScalarElement*)ele)->getValue();
        return make_pair(str, true);
    }
    return make_pair(string(), false);
}

pair<bool, bool> DocElementHelper::parserToBool(DocElement *ele, string trueString, string falseString) {
    if (ele->type == DOC_SCALAR) {
        string str = ((DocScalarElement*)ele)->getValue();
        if (str == trueString)
            return make_pair(true, true);
        if (str == falseString)
            return make_pair(false, true);
    }
    return make_pair(false, false);
}
