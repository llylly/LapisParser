//
// Created by lly on 08/10/2017.
//

#include "SingleAPIReport.h"

SingleAPIReport::SingleAPIReport(): RequesterReport() {
    this->useAliMiddleware = false;
}

SingleAPIReport::~SingleAPIReport() { }

BaseDataObject *SingleAPIReport::toDataObject() {
    BaseDataObject *ret = RequesterReport::toDataObject();
    ObjectDataObject *objRet = (ObjectDataObject*)ret;
    (*objRet)["useAliMiddleware"] = new BooleanDataObject(this->useAliMiddleware);
    return objRet;
}

ostream &SingleAPIReport::printTo(ostream &os) {
    os << "---Single API Request Report Begin---" << endl;

    if (this->request.size() > 0) {
        os << "Request:" << endl;
        for (map<string, BaseDataObject*>::iterator ite = this->request.begin();
             ite != this->request.end();
             ++ite)
            if (ite->second != NULL) {
                os << "   " << ite->first << ":" << endl;
                DocElement *ele = DataObjectAdapter::toDocElement(ite->second);
                ele->printTo(os);
                delete ele;
            }
    }
    os << endl;

    if (this->url != "")
        os << "URL: " << this->url << endl;

    if (this->responseType != "")
        os << "ResponseType: " << this->responseType << endl;

    if (this->response != NULL) {
        os << "Response:" << endl;
        DocElement *ele = DataObjectAdapter::toDocElement(this->response);
        ele->printTo(os);
        delete ele;
        os << endl;
    }

    if (this->startTime != 0LL)
        os << "StartTime: " << this->startTime << endl;

    if (this->endTime != 0LL)
        os << "EndTime: " << this->endTime << endl;

    if (this->requestMilliTime != 0LL)
        os << "RequestMilliTime: " << this->requestMilliTime << endl;

    if (this->err != 0LL) {
        os << "Err: " << endl;
        err->printError(os);
        os << endl;
    }

    if (this->useAliMiddleware)
        os << "UseAliMiddleware: true" << endl;
    else
        os << "UseAliMiddleware: false" << endl;

    os << "---Single API Request Report End---" << endl;

    os.flush();
    return os;
}