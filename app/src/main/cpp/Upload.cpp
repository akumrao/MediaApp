//
// Created by root on 28/12/19.
//

#include "Upload.h"
#include "base/filesystem.h"
#include "base/platform.h"

extern TickContext g_ctx;
void sendJavaMsg(JNIEnv *env, jobject instance,  jmethodID func,const char* msg);
/*****************************************************************************************/

Upload::Upload(std::string url) : _url(url) {

    if (!client) {
        if (_url.scheme() == "http" || _url.scheme() == "ws") {
            client = new HttpClient(nullptr, _url, HTTP_RESPONSE);
            //client->shouldSendHeader(false);
        } else {
            LTrace("Only Http Upload is supported")
        }

    }

}

void Upload::stop(bool flag) {

    LTrace(" Upload::stop")

    if (client) {

        form->stop(true);
        form->join();
        //app.stopAsync();
        int  r = uv_async_send(&async);
        assert(r == 0);
    }

    LTrace(" Upload::stop over")

}

Upload::~Upload() {
    LTrace("~Upload()")
    join();
}

static void async_cb_upload(uv_async_t* handle) {

    LTrace(" Upload::async_cb_upload")

    Upload *p = ( Upload *) handle->data;
    uv_close((uv_handle_t*)&p->async, nullptr);

    p->client->Close();
    p->app.stop();

    p->join();
    p->app.uvDestroy();

    LTrace(" Upload::async_cb_upload over")


}

void Upload::run() {

    LTrace("Upload OnRun");


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



    client->_request.setMethod("PUT");
    // client->_request.add( "Expect", "100-continue");
    client->_request.add("Accept", "*/*");
    client->_request.setKeepAlive(true);

    //for multipart
    /*
    client->_request.setChunkedTransferEncoding(false);
    auto form = FormWriter::create(client, FormWriter::ENCODING_MULTIPART_FORM);
    form->addPart("metadata", new StringPart(metadata, "application/json; charset=UTF-8"));
    form->addPart("file", new FilePart("/var/tmp/a.txt", "text/plain"));
     */

    // for chunked
    client->_request.setChunkedTransferEncoding(true);
    //auto form = FormWriter::create(client, FormWriter::TEXT_PLAIN);
    //form->addPart("file", new FilePart("/var/tmp/a.txt", "text/plain"));
    form = FormWriter::create(client, FormWriter::APPLICATION_ZIP);
    //form->addPart("file", new FilePartPart("./test.zip", FormWriter::APPLICATION_ZIP));
    form->addPart("file", new StringPart( std::string( FILE_CHUNK_SIZE*1024, '*'), FormWriter::APPLICATION_ZIP));

    form->header();

    client->fnConnect = [&](HttpBase * con) {
        LTrace("fnConnect")
        form->start();
    };

    client->fnClose = [&](HttpBase * con) {
        LTrace("fnClose")
    };


    client->fnFormClose = [&](ClientConnecton * con) {
         int  r = uv_async_send(&async);
    };




    client->fnComplete = [&](const Response & response) {
        std::cout << "client->fnComplete" << std::endl << std::flush;
        //form->condWait.signal();
    };

    client->fnUpdateProgess = [&](const std::string str) {
        LTrace(str)

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
        sendJavaMsg(env, pctx->mainActivityObj, timerId, str.c_str()  );
    };

    client->fnPayload = [&](HttpBase * conn, const char* data, size_t sz) {

    };


    client->send();

    async.data = this;
    int r = uv_async_init(app.uvGetLoop(), &async, async_cb_upload);
    assert(r == 0);
    app.run();


    if (form)
        delete form;

    sendJavaMsg(env, pctx->mainActivityObj, timerId, "{done:done}"  );

    // expects(fs::exists(path));
    //expects(crypto::checksum("MD5", path) == "44d667c142d7cda120332623eab69f40");
    // fs::unlink(path);


    //  stop();

    //    exit = true;


    delete client;
    client = nullptr;

    LTrace("Upload Over");
}
