
#ifndef MEDIAAPP_Download_H
#define MEDIAAPP_Download_H

#include "base/process.h"
#include "base/logger.h"
#include "base/thread.h"
#include "Common.h"

#include <string>
#include "net/netInterface.h"
#include "http/client.h"

using namespace base;
using namespace net;

class Download : public Thread {
public:

    Download(std::string url);

    ~Download();

    // Download(){};
    // virtual ~Thread2(void);

    void run();

    void stop(bool flag = true);


    std::string url;

    Client *conn={nullptr};

};

#endif //MEDIAAPP_Download_H
