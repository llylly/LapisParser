//
// Created by lly on 06/10/2017.
//

#include "RuntimeError.h"

std::vector<RuntimeError*> RuntimeError::errorPool;

RuntimeError::RuntimeError(): type(RUNTIME_BASE), timestamp(-1), msg("") {
}

BaseDataObject *RuntimeError::toDataObject() {
    ObjectDataObject *obj = new ObjectDataObject();
    (*obj)["errNo"] = new IntegerDataObject((int)this->type);
    (*obj)["timestamp"] = new IntegerDataObject(this->timestamp);
    (*obj)["msg"] = new StringDataObject(this->msg);
    return obj;
}

void RuntimeError::addError(RuntimeError *_err) {
    RuntimeError::errorPool.push_back(_err);
}

bool RuntimeError::hasError() {
    return (!RuntimeError::errorPool.empty());
}

void RuntimeError::printError(std::ostream &os) {
    for (std::vector<RuntimeError*>::iterator ite = RuntimeError::errorPool.begin();
         ite != RuntimeError::errorPool.end();
         ++ite) {
        RuntimeError *cur = *ite;
        os << "[Time: " << cur->timestamp << " errNo: " << (int)cur->type << "] "
           << cur->msg << std::endl;
    }
}

std::vector<RuntimeError*> *RuntimeError::getErrors() {
    return &errorPool;
}
