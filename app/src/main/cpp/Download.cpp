//
// Created by root on 10/12/19.
//


#include "Download.h"

#include "base/filesystem.h"
#include "base/platform.h"

extern TickContext g_ctx;

void sendJavaMsg(JNIEnv *env, jobject instance,  jmethodID func,const char* msg);

Download::Download(std::string url):_url(url)  {

    if(!client)
    {

        if (_url.scheme() == "http" || _url.scheme() == "ws") {
            client = new HttpClient(nullptr, _url, HTTP_RESPONSE);
        }
        else
        {
            LTrace("Only Http download is supported")
        }

    }

}


void Download::stop(bool flag ) {

    LTrace(" Download::stop")

    int  r = uv_async_send(&async);
    assert(r == 0);

}

Download::~Download()
{
    LTrace("~Download()")

    join();

}

static void async_cb_download(uv_async_t* handle) {

    LTrace("async_cb_download");
    Download *p = ( Download *) handle->data;
   // uv_close((uv_handle_t*)&p->async, nullptr);

    p->client->Close();
    p->app.stop();
    p->join();
    p->app.uvDestroy();
    LTrace("async_cb_download over");


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
    fs::addnode(path, "test.zip");


    //Client *conn = new Client("http://zlib.net/index.html");
    // client->start();
    client->fnComplete = [&](const Response & response) {
        std::cout << "client->fnComplete" << std::endl << std::flush;
        uv_close((uv_handle_t*)&async, nullptr);
        client->Close();
        //            app.stop()
    };

    client->fnUpdateProgess = [&](const std::string str) {
        LTrace(str)
        sendJavaMsg(env, pctx->mainActivityObj, timerId, str.c_str()  );
    };

    client->fnConnect = [&](HttpBase * conn) {
       ClientConnecton* con = (ClientConnecton*) conn;
       con->OutgoingProgress.start();
    };

    client->fnPayload = [&](HttpBase * conn, const char* data, size_t sz) {
        ClientConnecton* con = (ClientConnecton*) conn;
        con->OutgoingProgress.update(sz, con);
    };


    client->fnClose = [&](HttpBase * con, std::string str) {

        str= "{done:"+ str + "}" ;
        LTrace("fnClose " + str);
        uv_close((uv_handle_t*)&async, nullptr);
        sendJavaMsg(env, pctx->mainActivityObj, timerId, str.c_str() );
    };

    client->_request.setMethod("GET");
    client->_request.setKeepAlive(false);
    client->setReadStream(new std::ofstream(path, std::ios_base::out | std::ios_base::binary));
    client->send();

    async.data = this;
    int r = uv_async_init(app.uvGetLoop(), &async, async_cb_download);
    assert(r == 0);
    app.run();
    LTrace("run Over");


    LTrace("Download Over");

    sendJavaMsg(env, pctx->mainActivityObj, timerId, "{done:Download-Completed}"  );


   // exit = true;

    delete client;
    client = nullptr;

    LTrace("Download Over");
}
