//
// Created by lly on 09/10/2017.
//

#include <vector>
#include "AliMiddleware.h"

map<string, string> *AliMiddleware::main(map<string, string> *input, void *userp) {
    string secretKey = *((string*)userp);

    (*input)["SignatureMethod"] = "HMAC-SHA1";

    time_t nowTime;
    time(&nowTime);
    char buf[sizeof "2011-10-08T07:07:09Z"];
    strftime(buf, sizeof buf, "%FT%TZ", gmtime(&nowTime));
    string timestamp = buf;
    (*input)["Timestamp"] = timestamp;

    (*input)["SignatureVersion"] = "1.0";

    (*input)["SignatureNonce"] = to_string(rand());

    input->erase("Signature");

    /* calculate signature begin */
    vector<string> params;
    params.clear();
    for (map<string, string>::iterator ite = input->begin();
            ite != input->end();
            ++ite)
        params.push_back(ite->first);
    sort(params.begin(), params.end());

    stringstream stream;
    stream.clear();
    stream << "GET&" << "%2F";

    for (vector<string>::iterator ite = params.begin();
            ite != params.end();
            ++ite) {
        if (ite == params.begin()) stream << "&"; else stream << "%26";
        string now = AliMiddleware::URLEncode(*ite);
        stream << now;

        stream << "%3D";

        (*input)[*ite] = AliMiddleware::URLEncode((*input)[*ite]);
        now = AliMiddleware::URLEncode((*input)[*ite], true);
        stream << now;
    }

    stream.flush();

    string key = secretKey + "&";
    string the_str = stream.str();

    unsigned char *Key;
    Key = new unsigned char[key.size()+2];
    strcpy((char*) Key, key.c_str());
    unsigned char *test;
    test = new unsigned char[the_str.size()+2];
    strcpy((char*) test, the_str.c_str());
    unsigned char digest[40];

    CHMAC_SHA1 HMAC_SHA1;

    HMAC_SHA1.HMAC_SHA1(test, (int)the_str.length(), Key, (int)key.length(), digest);
    delete []Key;
    delete []test;
    string Base64String;

    Base64String = base64_encode(digest, 20);

    Base64String = AliMiddleware::URLEncode(Base64String);

    (*input)["Signature"] = Base64String;
    /* calculate signature end */

    return input;
}

string AliMiddleware::URLEncode(string s, bool urlComma) {
    stringstream stream;
    stream.clear();
    char tmp[10];
    for (int i = 0; i < s.length(); ++i)
        if ((s[i] == ':') && (urlComma))
            stream << "%253A";
        else if (((s[i] >= '0') && (s[i] <= '9')) || ((s[i] >= 'a') && (s[i] <= 'z'))
            || ((s[i] >= 'A') && (s[i] <= 'Z')) || (s[i] == '-') || (s[i] == '_')
            || (s[i] == '.') || (s[i] == '~'))
            stream << s[i];
        else {
            sprintf(tmp, "%%%02X", s[i]);
            stream << tmp;
        }
    stream.flush();
    return stream.str();
}
