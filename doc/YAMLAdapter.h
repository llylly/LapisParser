//
// Created by lly on 23/04/2017.
//

#ifndef VPARSER_YAMLADAPTER_H
#define VPARSER_YAMLADAPTER_H

#include <cstdio>
#include "../parser/libyaml/yaml.h"
#include "../error/Error.h"
#include "DocElement.h"
#include "DocScalarElement.h"
#include "DocObjectElement.h"
#include "DocSequenceElement.h"


class YAMLAdapter {
public:
    static DocElement* parseDoc(FILE *fin);

private:
    static DocScalarElement* parseScalar(yaml_parser_t& parser, yaml_event_t& event, int level);
    static DocObjectElement* parseObject(yaml_parser_t& parser, int level);
    static DocSequenceElement* parseSequence(yaml_parser_t& parser, int level);
    static DocElement* parseDocument(yaml_parser_t& parser, int level);

    static void addFormatErrorFromParser(yaml_parser_t &parser);
};


#endif //VPARSER_YAMLADAPTER_H
