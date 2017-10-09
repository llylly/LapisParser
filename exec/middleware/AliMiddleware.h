//
// Created by lly on 09/10/2017.
//

#ifndef VPARSER_ALIMIDDLEWARE_H
#define VPARSER_ALIMIDDLEWARE_H

#include <string>
#include <map>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <ctime>
#include <cstring>
#include <cstdio>
#include <cmath>
#include "HMAC_SHA1/HMAC_SHA1.h"
#include "HMAC_SHA1/base64.h"

using namespace std;

class AliMiddleware {
public:

    /** Provide real values for following parameters:
     * - Signature,
     * - SignatureMethod,
     * - Timestamp,
     * - SignatureVersion,
     * - SignatureNonce
     * **/
    static map<string, string> *main(map<string, string> *input, void *userp);

    static string URLEncode(string s, bool urlComma = false);
};

#endif //VPARSER_ALIMIDDLEWARE_H
