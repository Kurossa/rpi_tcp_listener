/*
 * app_mp3.hpp
 *
 *  Created on: Apr 12, 2016
 *      Author: Kurossa
 */

#ifndef MP3_MP3_H_
#define MP3_MP3_H_

#include <utilities/thread_wrapper.h>
#include <utilities/message_queue.h>
#include <app_config/app_constants.h>
#include <pthread.h>
#include <string>
#include <vector>
#include <ao/ao.h>
#include <mpg123.h>

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

const char MP3_STATUS_STR[MP3_STATUS_NUM][50] = { "AUDIO_IDLE",
                                              "AUDIO_PLAY_ONCE",
                                              "AUDIO_PLAY_IN_LOOP",
                                              "AUDIO_FAILED" };

struct sMp3Message
{
    uint16_t action;
    uint16_t parameter;
    uint16_t file_num;
    size_t file_size;
};

struct sMp3Reply
{
    eMp3Status_t mp3Status;
    eErrorCode_t errorCode;
};

typedef utils::MessageQueue<sMp3Message> mqSend_t;
typedef utils::MessageQueue<sMp3Reply> mqReply_t;

class cMp3Server : public cPthreadWrapper{
public:
    cMp3Server(mqSend_t* mqSend, mqReply_t* mqReply, std::vector<std::string> soundsInRam) :
        mqSend_m(mqSend),
        mqReply_m(mqReply),
        threadRunning_m(0),
        mp3Status_m(MP3_STATUS_IDLE),
        errorCode_m(ERROR_CODE_OK),
        soundsInRam_m(soundsInRam),
        mh_m(0),
        buffer_mpg_m(0),
        buffer_mpg_size_m(0),
        buffer_mpg_done_m(0),
        err_m(0),
        ao_driver_id_m(0),
        ao_dev_m(0),
        volume_m(100) {}
    ~cMp3Server() {}

    int startThread(void);
    int stopThread(void);
    void* action(void);

private:
    void init(void);
    void deinit(void);
    void handleMessage(sMp3Message& msg);
    void sendReplay(void);
    void playMp3(sMp3Message& msg);
    void setVolume(uint32_t volume);

    mqSend_t* mqSend_m;
    mqReply_t* mqReply_m;
    int threadRunning_m;
    eMp3Status_t mp3Status_m;
    eErrorCode_t errorCode_m;

    std::vector<std::string> soundsInRam_m;

    /* mp3 */
    mpg123_handle *mh_m;
    unsigned char *buffer_mpg_m;
    size_t buffer_mpg_size_m;
    size_t buffer_mpg_done_m;
    int err_m;
    int ao_driver_id_m;
    ao_device *ao_dev_m;

    uint32_t volume_m;
};


#endif /* MP3_MP3_H_ */
