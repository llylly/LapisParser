//
// Created by lly on 23/04/2017.
//

#ifndef VPARSER_XMLADAPTER_H
#define VPARSER_XMLADAPTER_H

#include "DocElement.h"
#include "../parser/libxml/libxml/parser.h"
#include "../parser/libxml/libxml/xmlmemory.h"
#include "../parser/libxml/libxml/xmlstring.h"
#include "../error/Error.h"
#include "../error/XMLFormatError.h"
#include "DocScalarElement.h"
#include "DocSequenceElement.h"
#include "DocObjectElement.h"

class XMLAdapter {
public:
    /**
     * Parse a doc to DocElement
     * @param fileName: path of the XML doc to be parsed
     * @return DocElement*: the root node of the parsed doc tree
     * NULL means error when parsing
     */
    static DocElement* parseDoc(const char *fileName);
    /**
     * Translate a node tree to XML doc
     * @param fileName: the path to save the doc
     * @param root: tree root
     * @return result
     * Non-zero result means error when saving
     */
    static int saveFile(const char *fileName, DocElement *root);

private:
    // Check the type label of the element and distribute it
    static DocElement* mainParse(xmlNodePtr ptr, int level);
    // -1 means any error of the array node, 0 means successful
    static int parseSequence(xmlNodePtr arrayPtr, DocSequenceElement *ele, int level);
    // -1 means any error of the object node, 0 means successful
    static int parseObject(xmlNodePtr objPtr, DocObjectElement *ele, int level);

    // ---- Parse & Save Separator

    static int saveObject(xmlNodePtr objPtr, DocElement *ele, string fieldName);

    // ---- Tools Function

    static bool needAlternate(string s);
};


#endif //VPARSER_XMLADAPTER_H
