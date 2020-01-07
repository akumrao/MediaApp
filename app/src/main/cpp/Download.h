
#ifndef MEDIAAPP_Download_H
#define MEDIAAPP_Download_H

#include "base/process.h"
#include "base/logger.h"
#include "base/thread.h"
#include "Common.h"

#include <string>
#include "net/netInterface.h"
#include "http/client.h"
#include "http/HttpClient.h"
#include "base/application.h"

using namespace base;
using namespace net;

class Download : public Thread {
public:

    Download(std::string url);

    ~Download();


    void run();

    void stop(bool flag = true);

    URL _url;
    ClientConnecton *client{nullptr};

    Application app;
    uv_async_t async;

};


#endif //MEDIAAPP_Download_H
