//
// Created by lly on 08/10/2017.
//

#include "RequesterReport.h"

RequesterReport::RequesterReport() {
    this->request.clear();
    this->url = "";
    this->responseType = "";
    this->response = NULL;
    this->startTime = 0LL;
    this->endTime = 0LL;
    this->requestMilliTime = 0LL;
    this->schema = NULL;
    this->err = NULL;
}

RequesterReport::~RequesterReport() {
    if (this->request.size() > 0) {
        for (map<string, BaseDataObject*>::iterator ite = this->request.begin();
                ite != this->request.end();
                ++ite)
            if (ite->second)
                delete ite->second;
    }
    if (this->response != NULL)
        delete this->response;
}

BaseDataObject *RequesterReport::toDataObject() {
    ObjectDataObject *res = new ObjectDataObject();

    ObjectDataObject *requestObj = new ObjectDataObject();
    for (map<string, BaseDataObject*>::iterator ite = this->request.begin();
            ite != this->request.end();
            ++ite)
        if (ite->second) {
            (*requestObj)[ite->first] = DataObjectAdapter::deepCopy(ite->second);
        }
    (*res)["request"] = requestObj;

    if (this->url != "")
        (*res)["url"] = new StringDataObject(this->url);

    if (this->responseType != "")
        (*res)["responseType"] = new StringDataObject(this->responseType);

    if (this->response != NULL) {
        (*res)["response"] = DataObjectAdapter::deepCopy(this->response);
    }

    if (this->startTime != 0LL)
        (*res)["startTime"] = new IntegerDataObject(this->startTime);
    if (this->endTime != 0LL)
        (*res)["endTime"] = new IntegerDataObject(this->endTime);
    if (this->requestMilliTime != 0LL)
        (*res)["requestMilliTime"] = new IntegerDataObject(this->requestMilliTime);

    if (this->err != NULL) {
        (*res)["err"] = this->err->toDataObject();
    }

    return res;
}

ostream &RequesterReport::printTo(ostream &os) {
    os << "---Requester Report Begin---" << endl;

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

    os << "---Requester Report End---" << endl;

    os.flush();
    return os;
}
