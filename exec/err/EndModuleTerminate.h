//
// Created by lly on 08/10/2017.
//

#ifndef VPARSER_ENDMODULETERMINATE_H
#define VPARSER_ENDMODULETERMINATE_H

#include "RuntimeError.h"
#include <string>

using namespace std;

class EndModuleTerminate: public RuntimeError {
public:
    EndModuleTerminate(string moduleName) {
        this->type = END_MODULE_TERMINATE;
        this->timestamp = time(0);
        this->msg = "Reach the end module \"" + moduleName + "\".";
    }
};


#endif //VPARSER_ENDMODULETERMINATE_H
