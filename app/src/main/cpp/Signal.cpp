//
// Created by root on 10/12/19.
//


#include "Signal.h"

#include "base/filesystem.h"
#include "base/platform.h"



#define SERVER_HOST "192.168.0.4"
#define SERVER_PORT 3000 //443

extern TickContext g_ctx;

void sendJavaMsg(JNIEnv *env, jobject instance,  jmethodID func,const char* msg);

Signal::Signal(std::string url)/*:_url(url)*/  {
    /*
    if(!client)
    {

        if (_url.scheme() == "http" || _url.scheme() == "ws") {
            client = new HttpClient(nullptr, _url, HTTP_RESPONSE);
        }
        else
        {
            LTrace("Only Http Signal is supported")
        }

    }*/

}


void Signal::stop(bool flag ) {

    LTrace(" Signal::stop")

    int  r = uv_async_send(&async);
    assert(r == 0);

}

Signal::~Signal()
{
    LTrace("~Signal()")

    join();

}

static void async_cb_Signal(uv_async_t* handle) {

    LTrace("async_cb_Signal");
    Signal *p = ( Signal *) handle->data;
   // uv_close((uv_handle_t*)&p->async, nullptr);

    p->client->close(0,"exit");
    p->app.stop();
    p->join();
    p->app.uvDestroy();
    LTrace("async_cb_Signal over");


}

void Signal::run() {

    LTrace("Signal OnRun");

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


    client = new SocketioClient(SERVER_HOST , SERVER_PORT);
    client->connect();
    base::sockio::Socket* socket = client->io();

    socket->on("connection", [&](string const& name, json const& data, bool isAck, json & ack_resp)
    {

        socket->on("ipaddr", Socket::event_listener_aux([&](string const& name, json const& data, bool isAck,json &ack_resp){
            LTrace(cnfg::stringify(data));

            LTrace("Server IP address is: " , data);
            // updateRoomURL(ipaddr);
        }));

        socket->on("created", Socket::event_listener_aux([&](string const& name, json const& data, bool isAck,json &ack_resp){

            LTrace(cnfg::stringify(data));
            LTrace("Created room", data[0], "- my client ID is", data[1]);
            isInitiator = true;
            //grabWebCamVideo();
        }));


        socket->on("bye", Socket::event_listener_aux([&](string const& name, json const& data, bool isAck,json &ack_resp){
            LTrace(cnfg::stringify(data));
            //  LTrace("Peer leaving room {" "room" }.`);
            // sendBtn.disabled = true;
            //snapAndSendBtn.disabled = true;
            // If peer did not create the room, re-enter to be creator.
            //if (!isInitiator) {
            // window.location.reload();
            //}
        }));

        socket->on("joined", Socket::event_listener_aux([&](string const& name, json const& data, bool isAck,json &ack_resp){
            LTrace(cnfg::stringify(data));
            //console.log('This peer has joined room', room, 'with client ID', clientId);
            // isInitiator = false;
            // createPeerConnection(isInitiator, configuration);
            // grabWebCamVideo();
        }));

        socket->on("full", Socket::event_listener_aux([&](string const& name, json const& data, bool isAck,json &ack_resp){
            LTrace(cnfg::stringify(data));
            // alert('Room ' + room + ' is full. We will create a new room for you.');
            // window.location.hash = '';
            // window.location.reload();
        }));

        socket->on("ready", Socket::event_listener_aux([&](string const& name, json const& data, bool isAck,json &ack_resp){
            LTrace(cnfg::stringify(data));
            // console.log('Socket is ready');
            // createPeerConnection(isInitiator, configuration);
        }));

        socket->on("log", Socket::event_listener_aux([&](string const& name, json const& data, bool isAck,json &ack_resp){
            LTrace(cnfg::stringify(data));
            //console.log.apply(console, array);
        }));

        socket->on("message", Socket::event_listener_aux([&](string const& name, json const& data, bool isAck,json &ack_resp){
            LTrace(cnfg::stringify(data));
            // console.log('SocketioClient received message:', message);
            // signalingMessageCallback(message);
        }));



        // Leaving rooms and disconnecting from peers.
        socket->on("disconnect", Socket::event_listener_aux([&](string const& name, json const& data, bool isAck,json &ack_resp){
            LTrace(cnfg::stringify(data));
            //console.log(`Disconnected: ${reason}.`);
            // sendBtn.disabled = true;
            // snapAndSendBtn.disabled = true;
        }));


        // window.addEventListener('unload', function() {
        //  console.log(`Unloading window. Notifying peers in ${room}.`);
        // socket->emit('bye', room);
        // });




        socket->emit("create or join", room);
        socket->emit("ipaddr");
    });



        LTrace("client->cbConnected" )




    async.data = this;
    int r = uv_async_init(app.uvGetLoop(), &async, async_cb_Signal);
    assert(r == 0);
    app.run();
    LTrace("run Over");


    LTrace("Signal Over");

    sendJavaMsg(env, pctx->mainActivityObj, timerId, "{done:Signal-Completed}"  );


   // exit = true;

    delete client;
    client = nullptr;

    LTrace("Signal Over");
}
