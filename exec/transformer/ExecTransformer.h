//
// Created by lly on 07/10/2017.
//

#ifndef VPARSER_EXECTRANSFORMER_H
#define VPARSER_EXECTRANSFORMER_H

#include <string>
#include "../../schema/DataSchemaObject.h"
#include "../../schema/data_schema/ArraySchema.h"
#include "../../schema/data_schema/ObjectSchema.h"
#include "../../schema/data_schema/BooleanSchema.h"
#include "../../doc/YAMLAdapter.h"
#include "../../lib/libxml/libxml/parser.h"
#include "../../lib/libxml/libxml/xmlmemory.h"
#include "../../lib/libxml/libxml/xmlstring.h"

using namespace std;

class ExecTransformer {
public:
    static string serailize(BaseDataObject *obj, DataSchemaObject *schema, string paramName);
    static DocElement *deserialize(string s, DataSchemaObject *schema);

private:
    /**
     *
     * @param obj
     * @param schema
     * @param paramName
     * @param type: 0 - uncertain, 1 - JSON, 2 - XML
     * @return
     */
    static string baseSerialize(BaseDataObject *obj, DataSchemaObject *schema, string paramName, int type=0);

    /**
     *
     * @param s
     * @param schema
     * @param type: 0 - uncertain, 1 - JSON, 2 - XML
     * @return
     */
    static DocElement *baseDeserialize(string s, DataSchemaObject *schema, int type=0);

    static DocElement *xmlParse(xmlNodePtr ptr, DataSchemaObject *schema);
};


#endif //VPARSER_EXECTRANSFORMER_H
