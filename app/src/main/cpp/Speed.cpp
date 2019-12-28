//
// Created by root on 28/12/19.
//

#include "Speed.h"
#include "base/platform.h"
#include <iostream>
#include <map>
#include <iomanip>
#include "SpeedTest.h"
#include "TestConfigTemplate.h"
#include <csignal>

extern TickContext g_ctx;
void sendJavaMsg(JNIEnv *env, jobject instance,  jmethodID func,const char* msg);
/*****************************************************************************************/

Speed::Speed() {

    signal(SIGPIPE, SIG_IGN);
}

Speed::~Speed() {
    LTrace("~Speed()")
    join();
}



void Speed::run() {

    LTrace("Speed OnRun");

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



   // client->fnLoad = [&](const std::string str) {
     //   LTrace(str)
      //  sendJavaMsg(env, pctx->mainActivityObj, timerId, str.c_str()  );
    //};


    SpeedTest sp = SpeedTest(SPEED_TEST_MIN_SERVER_VERSION);
    //IPInfo info;
    ServerInfo serverInfo;
    ServerInfo serverQualityInfo;


    auto serverList = sp.serverList();

    STrace << serverList.size() << " Servers online" << std::endl;

    serverInfo = sp.bestServer(10, [&](bool success) {

        std::cout << (success ? '.' : '*') << std::flush;

        return stopped();
    });

    if (stopped())
        return;


    STrace << "Server: " << serverInfo.name
           << " " << serverInfo.host
           << " by " << serverInfo.sponsor
           << " (" << serverInfo.distance << " km from you): "
           << sp.latency() << " ms" << std::endl;


    STrace << "Ping: " << sp.latency() << " ms." << std::endl;

    STrace << "Latency:" << sp.latency() << " ms." << std::endl;

    long jitter = 0;
    if (sp.jitter(serverInfo, jitter)) {
        STrace << "Jitter:" << jitter << " ms." << std::endl;
    }


    TestConfig uploadConfig;
    TestConfig downloadConfig;
    downloadConfig = narrowConfigDownload;
    uploadConfig = narrowConfigUpload;

    /*
       double preSpeed = 0;
       if (!sp.downloadSpeed(serverInfo, preflightConfigDownload, preSpeed, [&programOptions](bool success){

               std::cout << (success ? '.' : '*') << std::flush;
                std::cout << "Testing download speed (" << downloadConfig.concurrency << ") "  << std::flush;
       })){
           std::cerr << "Pre-flight check failed." << std::endl;
           return;
       }
       testConfigSelector(preSpeed, uploadConfig, downloadConfig);
     */

    if (stopped())
        return;

    double downloadSpeed = 0;
    if (sp.downloadSpeed(serverInfo, downloadConfig, downloadSpeed, [&](bool success, double matrix) {

        //std::cout << (success ? '.' : '*') << std::flush;
        if (success)
            STrace << std::fixed << std::setprecision(2) << uploadConfig.concurrency * matrix / 1000000 << " Mbit/s" << std::endl << std::flush;
        else {
            STrace << "download failed" << std::endl << std::flush;
            return false;
        }

        return stopped();
    })) {

        STrace <<  "Download: " << std::fixed << std::setprecision(2) << downloadSpeed << " Mbit/s" << std::endl;


    } else {
        STrace << "Download test failed." << std::endl;

        return;
    }

    if (stopped())
        return;


    double uploadSpeed = 0;
    if (sp.uploadSpeed(serverInfo, uploadConfig, uploadSpeed, [&](bool success, double matrix) {

        // std::cout << (success ? '.' : '*') << std::flush;
        if (success)
            STrace << "Upload:" << std::fixed << std::setprecision(2) << uploadConfig.concurrency * matrix / 1000000 << " Mbit/s" << std::endl << std::flush;
        else {
            std::cout << "upload test failed" << std::endl << std::flush;
            return false;
        }
        return stopped();
    })) {

        STrace << "Upload: " << std::fixed << std::setprecision(2) << uploadSpeed << " Mbit/s" << std::endl;


    } else {
        STrace << "Upload test failed." << std::endl;
        return;
    }


    sendJavaMsg(env, pctx->mainActivityObj, timerId, "{Speed_Test_done}"  );


    LTrace("Speed Over");
}
