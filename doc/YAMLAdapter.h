//
// Created by lly on 23/04/2017.
//

#ifndef VPARSER_YAMLADAPTER_H
#define VPARSER_YAMLADAPTER_H

#include <cstdio>
#include "../parser/libyaml/yaml.h"
#include "DocElement.h"

class YAMLAdapter {
public:
    static DocElement* parseDoc(FILE *fin);
};


#endif //VPARSER_YAMLADAPTER_H
