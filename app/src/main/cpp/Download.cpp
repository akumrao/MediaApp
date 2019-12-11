//
// Created by root on 10/12/19.
//


#include "Download.h"

#include "base/filesystem.h"
#include "base/platform.h"

extern TickContext g_ctx;

void sendJavaMsg(JNIEnv *env, jobject instance,  jmethodID func,const char* msg);

Download::Download(std::string url)  {

    if(!client)
    {
        _url = "http://zlib.net/fossils/zlib-1.2.8.tar.gz";

        if (_url.scheme() == "http" || _url.scheme() == "ws") {
            client = new HttpClient(nullptr, _url, HTTP_RESPONSE, 6553688);
        }
        else
        {
            LTrace("Only Http download is supported")
        }

    }

}


void Download::stop(bool flag ) {

    LTrace(" Download::stop")


    if(client ) {
        app.stop();
        // client->Close();
    }

}

Download::~Download()
{
    LTrace("~Download()")
    if(client )
    {
        app.uvDestroy();
    }

    while( !stopped())
    {
        base::sleep(1000);
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

    // sendJavaMsg(env, pctx->jniHelperObj, statusId,
    //           "TickerThread status: start ticking ...");

    ////////////////////////////////////

    std::string path("./");
    fs::addnode(path, "zlib-1.2.8.tar.gz");


    //Client *conn = new Client("http://zlib.net/index.html");
    // client->start();


    client->fnLoad = [&](const std::string str) {
        std::cout << "final test " << str << std::endl << std::flush;
        sendJavaMsg(env, pctx->mainActivityObj, timerId, str.c_str()  );
    };

    client->fnComplete = [&](const Response & response) {
        std::cout << "client->fnComplete" <<  std::endl << std::flush;

        client->Close();
    };

    client->_request.setMethod("GET");
    client->_request.setKeepAlive(false);
    client->setReadStream(new std::ofstream(path, std::ios_base::out | std::ios_base::binary));
    client->send();



    app.run();

    sendJavaMsg(env, pctx->mainActivityObj, timerId, "{Download done}"  );

    //expects(fs::exists(path));
    //expects(crypto::checksum("MD5", path) == "44d667c142d7cda120332623eab69f40");
    // fs::unlink(path);

    // std::cout << "app.run() over " << std::endl << std::flush;

    //  stop();

    exit = true;

    delete client;
    client = nullptr;

    LTrace("Download Over");
}
/*
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

   // sendJavaMsg(env, pctx->jniHelperObj, statusId,
     //           "TickerThread status: start ticking ...");




    ////////////////////////////////////

    Application app;

    std::string path("./");
    fs::addnode(path, "zlib-1.2.8.tar.gz");


    //Client *conn = new Client("http://zlib.net/index.html");
    conn->start();
    conn->clientConn->fnComplete = [&](const Response & response) {

        LTrace("clientConn->fnComplete")

        conn->shutdown();
      //  sendJavaMsg(env, pctx->mainActivityObj, timerId, "{Download done}"  );
        app.stop();
        app.uvDestroy();

        LTrace("clientConn->fnComplete over")
    };

    conn->clientConn->fnLoad = [&](const std::string &str) {

        sendJavaMsg(env, pctx->mainActivityObj, timerId, str.c_str()  );

    //    jstring javaMsg = env->NewStringUTF( msg);
     //   env->CallVoidMethod( instance, func, javaMsg);
       // env->DeleteLocalRef( javaMsg);

    };
    conn->clientConn->_request.setMethod("GET");
    conn->clientConn->_request.setKeepAlive(false);
    conn->clientConn->setReadStream(new std::ofstream(path, std::ios_base::out | std::ios_base::binary));
    conn->clientConn->send();

    app.run();

    LTrace(" app.run() over");

    sendJavaMsg(env, pctx->mainActivityObj, timerId, "{Download done}"  );


    //expect(fs::exists(path));
    //expect(crypto::checksum("MD5", path) == "44d667c142d7cda120332623eab69f40");
    //fs::unlink(path);

    stop();

}*/