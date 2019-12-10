//
// Created by root on 10/12/19.
//


#include "Download.h"
#include "base/application.h"
#include "base/filesystem.h"

extern TickContext g_ctx;

void sendJavaMsg(JNIEnv *env, jobject instance,  jmethodID func,const char* msg);

Download::Download(std::string url): url(url) {

    if(!conn)
    conn =  new Client("http://zlib.net/fossils/zlib-1.2.8.tar.gz");
}


void Download::stop(bool flag ) {

    LTrace("Ping Stop")

    conn->clientConn->Close();
    delete conn;
    conn = nullptr;

}


Download::~Download()
{
    if(conn )
    {
        conn->clientConn->Close();
        delete conn;
        conn = nullptr;
    }
}

void Download::run() {

    LTrace("Download OnRun");

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

    Application app;

    std::string path("./");
    fs::addnode(path, "zlib-1.2.8.tar.gz");


    //Client *conn = new Client("http://zlib.net/index.html");
    conn->start();
    conn->clientConn->fnComplete = [&](const Response & response) {
        std::cout << "Lerver response:";
    };
    conn->clientConn->_request.setMethod("GET");
    conn->clientConn->_request.setKeepAlive(false);
    conn->clientConn->setReadStream(new std::ofstream(path, std::ios_base::out | std::ios_base::binary));
    conn->clientConn->send();

    app.run();

    //expect(fs::exists(path));
    //expect(crypto::checksum("MD5", path) == "44d667c142d7cda120332623eab69f40");
    //fs::unlink(path);

}