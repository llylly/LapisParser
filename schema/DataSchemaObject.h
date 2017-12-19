//
// Created by lly on 12/07/2017.
//

#ifndef VPARSER_DATASCHEMAOBJECT_H
#define VPARSER_DATASCHEMAOBJECT_H

#include <cstdlib>
#include <ctime>
#include "BaseObject.h"
#include "../doc/DocObjectElement.h"
#include "../doc/DocScalarElement.h"
#include "../doc/DocSequenceElement.h"
#include "ExternalDocObject.h"
#include "data_schema/XMLObject.h"
#include "../error/Error.h"
#include "../error/FieldMissError.h"
#include "../error/FieldIllegalError.h"
#include "../error/FieldInvalidError.h"
#include "../error/IllegalEnumObjectError.h"
#include "../data/DataObjectAdapter.h"
#include "../data/ObjectDataObject.h"
#include "../data/SequenceDataObject.h"
#include "../data/NumberDataObject.h"

class DataSchemaObject: public BaseObject {
    /**
     * Class for all schema related fields.
     * They are: Schema Object, Items Object and some parts of Parameter Object
     * This is only the base object for them.
     * Subclasses are: StringSchema, NumberSchema, IntegerSchema,
     *      BooleanSchema, ArraySchema, FileSchema, ObjectSchema
     */

public:
    static const int TYPE_BASE;
    static const int TYPE_STRING;
    static const int TYPE_NUMBER;
    static const int TYPE_INTEGER;
    static const int TYPE_BOOLEAN;
    static const int TYPE_ARRAY;
    static const int TYPE_FILE;
    static const int TYPE_OBJECT;
    static const int TYPE_CUSTOMIZED;

    bool valid;

    string title;
    string description;
    int type;
    bool allowEmptyValue;
    BaseDataObject *_default;
    DocElement *rawEnum;
    double emptyProbability;
    ExternalDocObject *externalDocs;
    BaseDataObject *example;
    XMLObject *xml;

    vector<BaseDataObject*> _enum;

    virtual ~DataSchemaObject();

    /**
     * Use schema info to transform legal data to Data Object
     * @param ele DocElement node to transform
     * @return
     *  for legal value: transformed Data Object
     *  for illegal value: return NULL
     */
    virtual BaseDataObject *transform(DocElement *ele) = 0;

    /**
     * Build up the Schema Object from DocObjectElement
     * It implements detailed checks for each specified type of schema
     * @param filePath: file path where the DocElement from, used for building error messages
     * @param obj: the meta DocObjectElement
     * @param schemaType: the situation of the parse processed.
     *  Values should be from DataSchemaObjectFactory
     * @return
     *  true: built succeed!
     *  false: built failed, because of error in the script
     */
    virtual bool init(string filePath, DocObjectElement *obj, int schemaType) = 0;

public:
    DataSchemaObject();

    /**
     * Transform Schema Object to corresponding Data Object storing schema info
     * @return A ObjectDataObject
     */
    virtual BaseDataObject* toDataObject() override;

    /**
     * Use Schema info to check whether a data instance satisfies schema requirement
     * @param obj: data instance to check
     * @return true - satisfy the schema; false - not satisfy the schema
     * VIRTUAL: implemented by subclass
     */
    virtual bool check(BaseDataObject *obj);

    /**
     * Use random policy to generate an instance of the datatype
     * For base class, when has enum elements constraint, randomly pick one from enum elements.
     *  Otherwise, return NULL
     * @return
     *   for valid schema object: generated data instance
     *   for invalid schema object: NULL
     */
    virtual BaseDataObject *generate();

    /**
     * Find the required field schema object
     * @param fieldVec: the field name vector: {fieldVec[0]}.{fieldVec[1]}. ... . {fieldVec[len-1]}
     * @param startIndex: start index for current search
     * @return legal: pointer of this; illegal: null
     */
    virtual DataSchemaObject *findField(const vector<string> &fieldVec, int startIndex) = 0;

    /**
     * Generate a random real number in [0,1]
     * @return the random real
     */
    static double randomReal();

    friend class ArraySchema;
    friend class ObjectSchema;
};

struct DataSchemaObjectFactory {
    static const int PARAMETER_SCHEMA;
    static const int ITEM_SCHEMA;
    static const int NORMAL_SCHEMA;

    /**
     * Generate SchemaObject from DocObjectElement
     * @param filePath: file path where the DocElement from, used for building error messages
     * @param obj the meta DocObjectElement
     * @param schemaType: the situation of the parse processed.
     *  Values should be from DataSchemaObjectFactory
     * @param inProperty: whether the parsed schema is in 'properties' field,
     *  Used for init XMLObject
     * @return
     *  If succeed, return generated DataSchemaObject
     *  If failed, return NULL
     */
    static DataSchemaObject *create(string filePath, DocObjectElement *obj, int schemaType, bool inProperty = false);
};

#endif //VPARSER_DATASCHEMAOBJECT_H
