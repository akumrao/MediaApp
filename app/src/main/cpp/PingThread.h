//
// Created by root on 10/12/19.
//

#ifndef MEDIAAPP_PINGTHREAD_H
#define MEDIAAPP_PINGTHREAD_H
#include "base/process.h"
#include "base/logger.h"
#include <jni.h>

#include <inttypes.h>
#include <string>


static const char* kTAG = "hello-jniCallback";
#define LOGI(...) \
  ((void)__android_log_print(ANDROID_LOG_INFO, kTAG, __VA_ARGS__))
#define LOGW(...) \
  ((void)__android_log_print(ANDROID_LOG_WARN, kTAG, __VA_ARGS__))
#define LOGE(...) \
  ((void)__android_log_print(ANDROID_LOG_ERROR, kTAG, __VA_ARGS__))


// processing callback to handler class
typedef struct tick_context {
    JavaVM  *javaVM;
    jclass   jniHelperClz;
    jobject  jniHelperObj;
    jclass   mainActivityClz;
    jobject  mainActivityObj;
    pthread_mutex_t  lock;
    int      done;
} TickContext;


using namespace base;

class PingThread : public Thread {
public:

    PingThread(std::string host);
   // PingThread(){};
    // virtual ~Thread2(void);

    void run() ;

    void stop() {
        LTrace("Ping Stop")
        proc.kill(SIGINT);
        // sleep(1);
    }

    Process proc;

};

#endif //MEDIAAPP_PINGTHREAD_H
