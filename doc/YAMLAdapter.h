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
    /**
     * Parse a doc to DocElement
     * @param fileName: path of the YAML doc to be parsed
     * @return DocElement*: the root node of the parsed doc tree
     * NULL means error when parsing
     */
    static DocElement* parseDoc(const char *fileName);
    /**
     * Translate a node tree to YAML doc
     * @param fileName: the path to save the doc
     * @param root: tree root
     * @return result
     * Non-zero result means error when saving
     */
    static int saveFile(const char *fileName, DocElement *root);

private:
    static DocScalarElement* parseScalar(yaml_parser_t& parser, yaml_event_t& event, int level);
    static DocObjectElement* parseObject(yaml_parser_t& parser, int level);
    static DocSequenceElement* parseSequence(yaml_parser_t& parser, int level);
    static DocElement* parseDocument(yaml_parser_t& parser, int level);

    static void addFormatErrorFromParser(yaml_parser_t &parser);

    // ---- Parse & Save Separator

    static int emitObject(yaml_emitter_t *emitter, yaml_event_t *event, DocElement *ele);
};


#endif //VPARSER_YAMLADAPTER_H
