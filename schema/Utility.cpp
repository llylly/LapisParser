//
// Created by lly on 29/05/2017.
//

#include "Utility.h"

SUPPORT_SCHEME Utility::toScheme(string str) {
    if ((str == "HTTP") || (str == "http") || (str == "Http"))
        return HTTP;
    if ((str == "HTTPS") || (str == "https") || (str == "Https"))
        return HTTPS;
    return INVALID;
}