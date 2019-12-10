//
// Created by root on 10/12/19.
//

#ifndef MEDIAAPP_PINGTHREAD_H
#define MEDIAAPP_PINGTHREAD_H
#include "base/process.h"
#include "base/logger.h"
#include "Common.h"

#include <string>


using namespace base;

class PingThread : public Thread {
public:

    PingThread(std::string host);
   // PingThread(){};
    // virtual ~Thread2(void);

    void run() ;

    void stop(bool flag = true);


    Process proc;

};

#endif //MEDIAAPP_PINGTHREAD_H
