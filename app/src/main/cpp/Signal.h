
#ifndef MEDIAAPP_Signal_H
#define MEDIAAPP_Signal_H

#include "socketio/client.h"
#include "base/logger.h"
#include "base/thread.h"
#include "Common.h"

#include <string>
#include "base/application.h"

using namespace base;
using namespace net;
using namespace sockio;

class Signal : public Thread {
public:

    Signal(std::string url);

    ~Signal();


    void run();

    void stop(bool flag = true);

    Client *client{nullptr};
    URL _url;
    Application app;
    uv_async_t async;

};


#endif //MEDIAAPP_Signal_H
