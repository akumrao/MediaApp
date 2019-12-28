//
// Created by root on 28/12/19.
//

#ifndef MEDIAAPP_SPEED_H
#define MEDIAAPP_SPEED_H

#include "base/process.h"
#include "base/logger.h"
#include "base/thread.h"
#include "Common.h"

#include <string>

using namespace base;
using namespace net;

class Speed : public Thread {
public:
public:

    Speed();
    ~Speed();
    void run();

};

#endif //MEDIAAPP_SPEED_H
