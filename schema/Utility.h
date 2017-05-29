//
// Created by lly on 29/05/2017.
//

#ifndef VPARSER_UTILITY_H
#define VPARSER_UTILITY_H


#include "Controller.h"

class Utility {
public:
    /**
     *
     * @param str
     * @return SUPPORT_SCHEME
     * Translate scheme string to the enum
     * Supported Schemes: Http, Https
     */
    static SUPPORT_SCHEME toScheme(string str);
};


#endif //VPARSER_UTILITY_H
