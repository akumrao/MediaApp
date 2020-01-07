//
// Created by root on 28/12/19.
//

#ifndef MEDIAAPP_UPLOAD_H
#define MEDIAAPP_UPLOAD_H

#include "base/process.h"
#include "base/logger.h"
#include "base/thread.h"
#include "Common.h"

#include <string>
#include "net/netInterface.h"
#include "http/client.h"
#include "http/HttpClient.h"
#include "base/application.h"
#include "http/form.h"

using namespace base;
using namespace net;

class Upload : public Thread {
public:

    Upload(std::string url);

    ~Upload();


    void run();
    void stop(bool flag = true);
    URL _url;
    ClientConnecton *client{nullptr};

    Application app;
    FormWriter *form;

    uv_async_t async;
    uv_async_t asyncUpdate;

};

#endif //MEDIAAPP_UPLOAD_H
