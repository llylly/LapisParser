//
// Created by lly on 08/05/2017.
//

#ifndef VPARSER_REFEXPAND_H
#define VPARSER_REFEXPAND_H

#include <vector>
#include "../doc/DocElement.h"

class RefExpand {
public:
    void work();

    void visitor(DocElement *ele);

    DocElement *finder(string path);

private:
    vector<string> pathStack;
    vector<DocElement*> eleStack;
};


#endif //VPARSER_REFEXPAND_H
