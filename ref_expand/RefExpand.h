//
// Created by lly on 08/05/2017.
//

#ifndef VPARSER_REFEXPAND_H
#define VPARSER_REFEXPAND_H

#include <vector>
#include <set>
#include "../doc/DocElement.h"

/**
 * This phase replace all Reference Object with the entity
 * To prevent infinite recursion, I configured a maximum scan time
 * If there's still reference after too many scans, judge there's infinite recursion and exit
 */
class RefExpand {
public:
    bool work();


private:
    static int MAX_RECURSIVE_TIME;

    bool visit(string currentDoc, DocElement **ele);
    static DocElement *findByPath(string currentDoc, string path, int line, int col);

    vector<string> pathStack;
    vector<DocElement*> eleStack;
    string curFile;

    set<DocElement*> refObjRepo;
};


#endif //VPARSER_REFEXPAND_H
