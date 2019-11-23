#include <jni.h>
#include <string>


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

    LOGE("Failed to AttachCurrentThread, ErrorCode = %d", 100);

    return env->NewStringUTF(hello.c_str());
}
