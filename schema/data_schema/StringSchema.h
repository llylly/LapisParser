//
// Created by lly on 17/07/2017.
//

#ifndef VPARSER_STRINGSCHEMA_H
#define VPARSER_STRINGSCHEMA_H

#include <cstdlib>
#include <ctime>
#include "../DataSchemaObject.h"
#include "../../error/IllegalRegexError.h"
#include "../../error/IllegalIntervalError.h"

class StringSchema: public DataSchemaObject {
public:

    static const int NONETYPE;
    static const int DATE;
    static const int DATETIME;
    static const int PASSWORD;

    int formatType;
    int maxLength;
    int minLength;
    bool hasPattern;
    string pattern;

    StringSchema();
    BaseDataObject *toDataObject() override;
    bool check(BaseDataObject *obj) override;
    BaseDataObject *transform(DocElement *ele) override;
    BaseDataObject *generate() override;
    bool init(string filePath, DocObjectElement *obj, int schemaType) override;

    DataSchemaObject *findField(const vector<string> &fieldVec, int index) override;

private:
    /**
     * Check whether exists string that satisfies all these constraints
     * @return bool
     */
    bool compatibleCheck();

    /**
     * Whether the str is a legal regex expression
     * @param str: string to check
     * @return true: legal; false: illegal
     */
    static bool isLegalRegex(string str);

    /**
     * Whether the str meets the regex pattern
     * @param str: string to check
     * @param pattern: pattern
     * @return meet or not
     */
    static bool regexCheck(string str, string pattern);

    /**
     * Whether the str meets "DATE" format requirement
     * @param str: string to check
     * @return meet or not
     */
    static bool dateCheck(string str);

    /**
     * Whether the str meets "DATETIME" format requirement
     * @param str: string to check
     * @return meet or not
     */
    static bool dateTimeCheck(string str);

    /**
     * Generate a date
     * @param pattern: extra regex pattern constraint, "" means no constraint
     * @param minLength: min length
     * @param maxLength: max length
     * @return the string
     */
    static string generateDate(string pattern, int minLength, int maxLength);

    /**
     * Generate a datetime
     * @param pattern: extra regex pattern constraint, "" means no constraint
     * @param minLength: min length
     * @param maxLength: max length
     * @return the string
     */
    static string generateDateTime(string pattern, int minLength, int maxLength);

    /**
     * Generate a string satisfying the regex pattern given
     * @param pattern: the constraint regex expression pattern
     * @param minLength: min length
     * @param maxLength: max length
     * @return the string
     */
    static string generateFromPattern(string pattern, int minLength, int maxLength);
};


#endif //VPARSER_STRINGSCHEMA_H
