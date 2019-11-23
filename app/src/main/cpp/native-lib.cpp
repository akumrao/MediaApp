#include <jni.h>
#include <string>

#include "mediaserver/src/base/include/base/process.h"

//#include "process.h"
using namespace base;

#include <android/log.h>

#include <android/log.h>

static const char* kTAG = "hello-jniCallback";
#define LOGI(...) \
  ((void)__android_log_print(ANDROID_LOG_INFO, kTAG, __VA_ARGS__))
#define LOGW(...) \
  ((void)__android_log_print(ANDROID_LOG_WARN, kTAG, __VA_ARGS__))
#define LOGE(...) \
  ((void)__android_log_print(ANDROID_LOG_ERROR, kTAG, __VA_ARGS__))

extern "C" JNIEXPORT jstring JNICALL
Java_com_harman_vns_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C1++";


    Process proc{ "ping", "-W", "4" , "-c",  "5", "www.google.com" };
    proc.onstdout = [&](std::string line) {
       // std::cout << "process stdout: " << line << std::endl;
        //gotStdout = true;
        LOGE("Failed to AttachCurrentThread, ErrorCode = %s", line.c_str());
        proc.kill();
        //str << line;
    };
    proc.onexit = [&](int64_t status) {
       // std::cout << "process exit: " << status << std::endl;
        //gotExit = true;
        LOGE("Failed to AttachCurrentThread, ErrorCode = %d", status);
    };
    proc.spawn();
    uv_run(uv_default_loop(), UV_RUN_DEFAULT);


    LOGE("Failed to AttachCurrentThread, ErrorCode = %d", 100);

    return env->NewStringUTF(hello.c_str());
}
