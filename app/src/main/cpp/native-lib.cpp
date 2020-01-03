

#include "PingThread.h"
#include "Download.h"
#include "Upload.h"
#include "Speed.h"

TickContext g_ctx;


#include <android/log.h>

extern "C" JNIEXPORT jstring JNICALL
Java_com_harman_vns_MainActivity_stringFromJNI(  JNIEnv *env, jobject /* this */) {

    std::string hello = "Ping www.yahoo.com";
    LOGE("Failed to AttachCurrentThread, ErrorCode = %d", 100);
    return env->NewStringUTF(hello.c_str());
}



extern "C" JNIEXPORT void JNICALL
Java_com_harman_vns_ui_PingFragment_LocationJNI( JNIEnv *env, jobject jobj, jdouble j1, jdouble j2) {

    //Logger::instance().add(new RemoteChannel("debug", Level::Trace, "10.99.234.126"));

    Logger::instance().add(new RemoteChannel("debug", Level::Remote, "10.99.89.206"));

    double j1d = static_cast<double>(j1);
    double j2d = static_cast<double>(j2);

    LTrace("location " ,j1d, " , ", j2d );

}




/* This is a trivial JNI example where we use a native method
 * to return a new VM String. See the corresponding Java source
 * file located astatic const char* kTAG = "hello-jniCallback";
#define LOGI(...) \
  ((void)__android_log_print(ANDROID_LOG_INFO, kTAG, __VA_ARGS__))
#define LOGW(...) \
  ((void)__android_log_print(ANDROID_LOG_WARN, kTAG, __VA_ARGS__))
#define LOGE(...) \
  ((void)__android_log_print(ANDROID_LOG_ERROR, kTAG, __VA_ARGS__))t:
 *
 *   hello-jniCallback/app/src/main/java/com/example/hellojnicallback/MainActivity.java
 */
/*
extern "C"  JNIEXPORT jstring JNICALL
Java_com_harman_vns_MainActivity_stringFromJNI( JNIEnv* env, jobject thiz )
{
#if defined(__arm__)
    #if defined(__ARM_ARCH_7A__)
    #if defined(__ARM_NEON__)
      #if defined(__ARM_PCS_VFP)
        #define ABI "armeabi-v7a/NEON (hard-float)"
      #else
        #define ABI "armeabi-v7a/NEON"
      #endif
    #else
      #if defined(__ARM_PCS_VFP)
        #define ABI "armeabi-v7a (hard-float)"
      #else
        #define ABI "armeabi-v7a"
      #endif
    #endif
  #else
   #define ABI "armeabi"
  #endif
#elif defined(__i386__)
#define ABI "x86"
#elif defined(__x86_64__)
    #define ABI "x86_64"
#elif defined(__mips64)
#define ABI "mips64"
#elif defined(__mips__)
#define ABI "mips"
#elif defined(__aarch64__)
#define ABI "arm64-v8a"
#else
#define ABI "unknown"
#endif
    return env->NewStringUTF( "Hello from JNI !  Compiled with ABI " ABI ".");
}
*/
/*
 *  A helper function to show how to call
 *     java static functions JniHelper::getBuildVersion()
 *     java non-static function JniHelper::getRuntimeMemorySize()
 *  The trivial implementation for these functions are inside file
 *     JniHelper.java
 */
void queryRuntimeInfo(JNIEnv *env, jobject instance) {
    // Find out which OS we are running on. It does not matter for this app
    // just to demo how to call static functions.
    // Our java JniHelper class id and instance are initialized when this
    // shared lib got loaded, we just directly use them
    //    static function does not need instance, so we just need to feed
    //    class and method id to JNI
  /*  jmethodID versionFunc = env->GetStaticMethodID(
            g_ctx.jniHelperClz,
            "getBuildVersion", "()Ljava/lang/String;");
    if (!versionFunc) {
        LOGE("Failed to retrieve getBuildVersion() methodID @ line %d",
             __LINE__);
        return;
    }


    jstring buildVersion = (jstring)env->CallStaticObjectMethod( g_ctx.jniHelperClz, versionFunc);
    const char *version = env->GetStringUTFChars( buildVersion, NULL);
    if (!version) {
        LOGE("Unable to get version string @ line %d", __LINE__);
        return;
    }
    LOGI("Android Version - %s", version);
    env->ReleaseStringUTFChars( buildVersion, version);

    // we are called from JNI_OnLoad, so got to release LocalRef to avoid leaking
    env->DeleteLocalRef(buildVersion);

    // Query available memory size from a non-static public function
    // we need use an instance of JniHelper class to call JNI
    jmethodID memFunc = env->GetMethodID( g_ctx.jniHelperClz,
                                          "getRuntimeMemorySize", "()J");
    if (!memFunc) {
        LOGE("Failed to retrieve getRuntimeMemorySize() methodID @ line %d",
             __LINE__);
        return;
    }
    jlong result = env->CallLongMethod( instance, memFunc);
    LOGI("Runtime free memory size: %" PRId64, result);
    (void)result;  // silence the compiler warning
    */
}

/*
 * processing one time initialization:
 *     Cache the javaVM into our context
 *     Find class ID for JniHelper
 *     Create an instance of JniHelper
 *     Make global reference since we are using them from a native thread
 * Note:
 *     All resources allocated here are never released by application
 *     we rely on system to free all global refs when it goes away;
 *     the pairing function JNI_OnUnload() never gets called at all.
 */
extern "C" JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {

    Logger::instance().add(new RemoteChannel("debug", Level::Remote, "10.99.89.206"));

    //Logger::instance().add(new ConsoleChannel("debug", Level::Trace ));


    LTrace("OnLoad");

    JNIEnv* env;
    memset(&g_ctx, 0, sizeof(g_ctx));

    g_ctx.javaVM = vm;
    if (vm->GetEnv( (void**)&env, JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR; // JNI version not supported.
    }

    jclass  clz = env->FindClass("com/harman/vns/JniHandler");
    g_ctx.jniHelperClz = (jclass)env->NewGlobalRef( clz);

    jmethodID  jniHelperCtor = env->GetMethodID( g_ctx.jniHelperClz, "<init>", "()V");



    jobject    handler = env->NewObject( g_ctx.jniHelperClz,
                                         jniHelperCtor);
    g_ctx.jniHelperObj = env->NewGlobalRef( handler);
    queryRuntimeInfo(env, g_ctx.jniHelperObj);

    g_ctx.done = 0;
    g_ctx.mainActivityObj = NULL;
    return  JNI_VERSION_1_6;
}

/*
 * A helper function to wrap java JniHelper::updateStatus(String msg)
 * JNI allow us to call this function via an instance even it is
 * private function.
 */
void   sendJavaMsg(JNIEnv *env, jobject instance,
                   jmethodID func,const char* msg) {

    jstring javaMsg = env->NewStringUTF( msg);
    env->CallVoidMethod( instance, func, javaMsg);
    env->DeleteLocalRef( javaMsg);
}

/*
 * Main working thread function. From a pthread,
 *     calling back to MainActivity::updateTimer() to display ticks on UI
 *     calling back to JniHelper::updateStatus(String msg) for msg
 */
/*
void*  UpdateTicks(void* context) {
    TickContext *pctx = (TickContext*) context;
    JavaVM *javaVM = pctx->javaVM;
    JNIEnv *env;
    jint res = javaVM->GetEnv((void**)&env, JNI_VERSION_1_6);
    if (res != JNI_OK) {
        res = javaVM->AttachCurrentThread( &env, NULL);
        if (JNI_OK != res) {
            LOGE("Failed to AttachCurrentThread, ErrorCode = %d", res);
            return NULL;
        }
    }

    jmethodID statusId = env->GetMethodID( pctx->jniHelperClz,
                                           "updateStatus",
                                           "(Ljava/lang/String;)V");
    sendJavaMsg(env, pctx->jniHelperObj, statusId,
                "TickerThread status: initializing...");

    // get mainActivity updateTimer function
    jmethodID timerId = env->GetMethodID( pctx->mainActivityClz,
                                          "updateTimer1", "(Ljava/lang/String;)V");
    //jmethodID timerId = (*env)->GetMethodID(env, pctx->mainActivityClz,
    //                                        "updateTimer1", "()V");

    struct timeval beginTime, curTime, usedTime, leftTime;
    const struct timeval kOneSecond = {
            (__kernel_time_t)1,
            (__kernel_suseconds_t) 0
    };

    sendJavaMsg(env, pctx->jniHelperObj, statusId,
                "TickerThread status: start ticking ...");
    while(1) {
        gettimeofday(&beginTime, NULL);
        pthread_mutex_lock(&pctx->lock);
        int done = pctx->done;
        if (pctx->done) {
            pctx->done = 0;
        }
        pthread_mutex_unlock(&pctx->lock);
        if (done) {
            break;
        }
        //(*env)->CallVoidMethod(env, pctx->mainActivityObj, timerId);

        sendJavaMsg(env, pctx->mainActivityObj, timerId,
                    "TickerThread status: initializing...");


        gettimeofday(&curTime, NULL);
        timersub(&curTime, &beginTime, &usedTime);
        timersub(&kOneSecond, &usedTime, &leftTime);
        struct timespec sleepTime;
        sleepTime.tv_sec = leftTime.tv_sec;
        sleepTime.tv_nsec = leftTime.tv_usec * 1000;

        if (sleepTime.tv_sec <= 1) {
            nanosleep(&sleepTime, NULL);
        } else {
            sendJavaMsg(env, pctx->jniHelperObj, statusId,
                        "TickerThread error: processing too long!");
        }
    }

    sendJavaMsg(env, pctx->jniHelperObj, statusId,
                "TickerThread status: ticking stopped");
    javaVM->DetachCurrentThread();
    return context;
}
*/
/*
 * Interface to Java side to start ticks, caller is from onResume()
 */



Thread *thread = nullptr;


void   start(JNIEnv *env, jobject instance, jstring jcmd , jstring jurl) {

    const char *c_cmd = env->GetStringUTFChars(jcmd, NULL);
    std::string cmd = c_cmd;

    const char *c_url = env->GetStringUTFChars(jurl, NULL);
    std::string url = c_url;


    LTrace("StartTicks ", cmd )

    LTrace("StartTicks ", url )

  //  static int inc = 0;

    if (!thread)
    {
        if( cmd == std::string("Download" ))
            thread = new Download(url);
        else if( cmd == std::string("Upload" ))
            thread = new Upload(url);
        else if( cmd == std::string("Speed" ))
            thread = new Speed();
        else {
           // if(++inc % 2 )
                thread = new PingThread(url);
          //  else
            //    thread = new Speed();  //  //thread = new Speed();
        }
    }
    else
        return ;



    jclass clz = env->GetObjectClass(instance);
    g_ctx.mainActivityClz = (jclass)env->NewGlobalRef( clz);
    g_ctx.mainActivityObj = env->NewGlobalRef(instance);

    thread->start();
}


void  stop(JNIEnv *env, jobject instance) {
    LTrace("Stop Ticks");

    if( thread) {
        thread->stop();

        // sleep(1);
        delete thread;
        thread = nullptr;

        LTrace("pingThread delete over");

        // release object we allocated from StartTicks() function
        env->DeleteGlobalRef(g_ctx.mainActivityClz);
        env->DeleteGlobalRef(g_ctx.mainActivityObj);
        g_ctx.mainActivityObj = NULL;
        g_ctx.mainActivityClz = NULL;

        LTrace("Stop Ticks over");
    }
}

extern "C" JNIEXPORT void JNICALL
Java_com_harman_vns_ui_DownloadFragment_startTicks(JNIEnv *env, jobject instance ,jstring jcmd , jstring jurl ) {
    start(env,instance, jcmd,jurl );
}


extern "C" JNIEXPORT void JNICALL
Java_com_harman_vns_ui_PingFragment_startTicks(JNIEnv *env, jobject instance ,jstring jcmd , jstring jurl ) {
    start(env,instance, jcmd,jurl );
}
/*
 * Interface to Java side to stop ticks:
 *    we need to hold and make sure our native thread has finished before return
 *    for a clean shutdown. The caller is from onPause
 */
extern "C" JNIEXPORT void JNICALL
Java_com_harman_vns_ui_DownloadFragment_StopTicks(JNIEnv *env, jobject instance) {

    stop(env,instance );
}

/*
 * Interface to Java side to stop ticks:
 *    we need to hold and make sure our native thread has finished before return
 *    for a clean shutdown. The caller is from onPause
 */
extern "C" JNIEXPORT void JNICALL
Java_com_harman_vns_ui_PingFragment_StopTicks(JNIEnv *env, jobject instance) {

    stop(env,instance);
}
