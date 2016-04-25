/*
 * app_mp3.hpp
 *
 *  Created on: Apr 12, 2016
 *      Author: mariusz
 */

#ifndef APP_MP3_APP_MP3_HPP_
#define APP_MP3_APP_MP3_HPP_

#include <pthread.h>
#include <string>
#include <app_thread_wrapper.hpp>
#include <app_message_queue.hpp>

typedef enum eMp3Action {
    MP3_PLAY = 0,
    MP3_STOP,
    MP3_VOLUME,
    MP3_STATUS,
    MP3_PAUSE,
    MP3_CONTINUE,
    MP3_NUM
}eMp3Action_t;

typedef enum eMp3Param {
    MP3_PARAM_ONCE = 0,
    MP3_PARAM_IN_LOOP,
    MP3_PARAM_NUM
}eMp3Param_t;

typedef enum eMp3Status {
    MP3_STATUS_IDLE = 0,
    MP3_STATUS_PLAY_ONCE,
    MP3_STATUS_PLAY_IN_LOOP,
    MP3_STATUS_FAILED,
    MP3_STATUS_NUM
}eMp3Status_t;

const char MPS_STATUS[MP3_STATUS_NUM][50] = { "AUDIO_IDLE",
                                              "AUDIO_PLAY_ONCE",
                                              "AUDIO_PLAY_IN_LOOP",
                                              "AUDIO_FAILED" };

struct sMp3Message
{
    uint16_t action;
    uint16_t parameter;
    char* file;
    size_t file_size;

};

struct sMp3Reply
{
    eMp3Status_t status;
};

typedef cMessageQueue<sMp3Message> mqSend_t;
typedef cMessageQueue<sMp3Reply> mqReply_t;

class cMp3Server : public cPthreadWrapper{
public:
    cMp3Server(mqSend_t* mqSend, mqReply_t* mqReply) : mqSend_m(mqSend), mqReply_m(mqReply), threadRunning_m(0), status_m(MP3_STATUS_IDLE) {}
    ~cMp3Server() {}

    int startThread(void);
    int stopThread(void);
    void* action(void);
    eMp3Status_t getStatus(void) { return status_m; }

private:
    void handleMessage(sMp3Message& msg);
    void sendReplay(void);
    void playMp3(sMp3Message& msg);
    void setVolume(uint32_t volume);

    mqSend_t* mqSend_m;
    mqReply_t* mqReply_m;
    int threadRunning_m;
    eMp3Status_t status_m;
};


#endif /* APP_MP3_APP_MP3_HPP_ */
