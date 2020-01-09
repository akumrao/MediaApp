//
// Created by root on 10/12/19.
//


#include "PingThread.h"
#include <string>

extern TickContext g_ctx;




void   sendJavaMsg(JNIEnv *env, jobject instance,  jmethodID func,const char* msg);



PingThread::PingThread(std::string host)  {

    proc.args = {"ping", "-W",  "4", "-c", "60",  host};
}

void PingThread::stop(bool flag ) {
    LTrace("Ping Stop")
    proc.kill(SIGINT);
    // sleep(1);
    exit = flag;
    join();
}


void PingThread::run() {

    LTrace("PingThread OnRun");

    /////////////////////////////////////
    TickContext *pctx = (TickContext*) &g_ctx;
    JavaVM *javaVM = pctx->javaVM;
    JNIEnv *env;
    jint res = javaVM->GetEnv((void**)&env, JNI_VERSION_1_6);
    if (res != JNI_OK) {
        res = javaVM->AttachCurrentThread( &env, NULL);
        if (JNI_OK != res) {
            LOGE("Failed to AttachCurrentThread, ErrorCode = %d", res);
            return ;
        }
    }

    jmethodID statusId = env->GetMethodID( pctx->jniHelperClz,
                                           "updateStatus",
                                           "(Ljava/lang/String;)V");
    sendJavaMsg(env, pctx->jniHelperObj, statusId,
                "TickerThread status: initializing...");

    // get mainActivity updateTimer function
    jmethodID timerId = env->GetMethodID( pctx->mainActivityClz,
                                          "updateTimer", "(Ljava/lang/String;)V");
    //jmethodID timerId = (*env)->GetMethodID(env, pctx->mainActivityClz,
    //                                        "updateTimer1", "()V");

    struct timeval beginTime, curTime, usedTime, leftTime;
    const struct timeval kOneSecond = {
            (__kernel_time_t)1,
            (__kernel_suseconds_t) 0
    };

    sendJavaMsg(env, pctx->jniHelperObj, statusId,
                "TickerThread status: start ticking ...");




    ////////////////////////////////////

    proc.onstdout = [&](std::string line) {

        std::stringstream X(line);
        std::string T;

        while (std::getline(X, T, '\n')    ) {

            {
                LTrace( T );

                //gettimeofday(&beginTime, NULL);

                //(*env)->CallVoidMethod(env, pctx->mainActivityObj, timerId);

                if(T.length())
                sendJavaMsg(env, pctx->mainActivityObj, timerId, T.c_str()  );


            }
        }


    };
    proc.onexit = [&](int64_t status) {
        LTrace("PingThread onexit");
        sendJavaMsg(env, pctx->jniHelperObj, statusId,
                    "TickerThread status: ticking stopped");
        javaVM->DetachCurrentThread();
    };
    proc.spawn();
    uv_run(uv_default_loop(), UV_RUN_DEFAULT);


    LTrace("PingThread Ping over");
}