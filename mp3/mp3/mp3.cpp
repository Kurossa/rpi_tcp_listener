/*
 * app_mp3.cpp
 *
 *  Created on: Apr 12, 2016
 *      Author: mariusz
 */
#include "mp3.h"
#include <utilities/logger.h>
#include <unistd.h>
#include <stdio.h>

#define MAX_INFINITE_LOOPS (100)
#define MP3_SAMPLE_SIZE (16384)
#define BITS (8)

using namespace std;

int cMp3Server::startThread(void) {
    int status = 0;
    threadRunning_m = 1;
    status = pthread_create(&threadId_m, NULL, &doAction, this);
    return status;
}

int cMp3Server::stopThread(void) {
    int status = 0;
    threadRunning_m = 0;
    status = pthread_join(threadId_m, NULL);
    return status;
}

void* cMp3Server::action(void) {
    sMp3Message msg;
    while (threadRunning_m) {
        if (MQ_MSG_POP == mqSend_m->wait(msg, 1)) {
            handleMessage(msg);
        }
    }
    return NULL;
}

void cMp3Server::handleMessage(sMp3Message& msg) {

    switch (msg.action) {
    case MP3_PLAY:
        if (MP3_PARAM_ONCE == msg.parameter
            || MP3_PARAM_IN_LOOP == msg.parameter) {
            playMp3(msg);
        } else {
            errorCode_m = ERROR_CODE_PLAY_MODE_ERROR;
            mp3Status_m = MP3_STATUS_IDLE;
            sendReplay();
        }
        break;
    case MP3_STOP:
        errorCode_m = ERROR_CODE_OK;
        mp3Status_m = MP3_STATUS_IDLE;
        sendReplay();
        break;
    case MP3_VOLUME:
        setVolume(msg.parameter);
        sendReplay();
        break;
    case MP3_STATUS:
        errorCode_m = ERROR_CODE_OK;
        sendReplay();
        break;
    case MP3_PAUSE:
        errorCode_m = ERROR_CODE_OK;
        sendReplay();
        /* Not supported yet. */
    case MP3_CONTINUE:
        errorCode_m = ERROR_CODE_OK;
        sendReplay();
        /* Not supported yet. */
    default:
        errorCode_m = ERROR_CODE_OK;
        sendReplay();
        break;
    }
}
void cMp3Server::sendReplay(void) {
    sMp3Reply reply;
    reply.mp3Status = mp3Status_m;
    reply.errorCode = errorCode_m;
    mqReply_m->push(reply);
}

void cMp3Server::playMp3(sMp3Message& msg) {
    bool new_message = 0;
    sMp3Message msg_poke;

    /* init mp3 file */
    ao_sample_format format;
    int channels, encoding;
    long rate;

    init();
    errorCode_m = ERROR_CODE_NOK;
    if (msg.file_num > 0 && msg.file_num <= soundsInRam_m.size() && MPG123_OK == mpg123_open(mh_m, soundsInRam_m.at(msg.file_num -1).c_str())) {
        errorCode_m = ERROR_CODE_OK;
        sendReplay();
        if (MP3_PARAM_ONCE == msg.parameter) {
            mp3Status_m = MP3_STATUS_PLAY_ONCE;
        }
        else if (MP3_PARAM_IN_LOOP == msg.parameter) {
            mp3Status_m = MP3_STATUS_PLAY_IN_LOOP;
        }
        deinit();
        uint32_t max_loops = 0;
        do {
            init();
            // open the file and get the decoding format
            mpg123_open(mh_m, soundsInRam_m.at(msg.file_num -1).c_str());
            mpg123_getformat(mh_m, &rate, &channels, &encoding);

            // set the output format and open the output device
            format.bits = mpg123_encsize(encoding) * BITS;
            format.rate = rate;
            format.channels = channels;
            format.byte_format = AO_FMT_NATIVE;
            format.matrix = 0;
            ao_dev_m = ao_open_live(ao_driver_id_m, &format, NULL);
            //printf("Playing: %s, in mode %d\n", soundsInRam_m.at(msg.file_num -1).c_str(), msg.parameter);
            int status = 0;
            setVolume(volume_m);
            while (threadRunning_m && (status = mpg123_read(mh_m, buffer_mpg_m, buffer_mpg_size_m, &buffer_mpg_done_m)) == MPG123_OK) {
                /* MP3 DECODE CODE HERE */
                ao_play(ao_dev_m, (char *) buffer_mpg_m, buffer_mpg_done_m);
                /* MP3 DECODE CODE HERE */
                if (MQ_MSG_PENDING == mqSend_m->poke(msg_poke)) {
                    switch (msg_poke.action) {
                    case MP3_VOLUME:
                        if (MQ_MSG_POP == mqSend_m->pop(msg_poke)) {
                            setVolume(msg_poke.parameter);
                        } else {
                            logPrintf(ERROR_LOG, "cMp3Server: Message was pending, but not received.\n");
                        }
                        break;
                    case MP3_PLAY:
                        if (MQ_MSG_POP != mqSend_m->pop(msg_poke)) {
                            logPrintf(ERROR_LOG, "cMp3Server: Message was pending, but not received.\n");
                        }
                        break;
                    case MP3_STATUS:
                        if (MQ_MSG_POP != mqSend_m->pop(msg_poke)) {
                            logPrintf(ERROR_LOG, "cMp3Server: Message was pending, but not received.\n");
                        }
                        break;
                    default:
                        new_message = 1;
                        break;
                    }
                    if (new_message) {
                        break;
                    }
                    sendReplay();
                }
            }
            ++max_loops;
            deinit();
        } while (threadRunning_m && !new_message && max_loops < MAX_INFINITE_LOOPS && MP3_PARAM_IN_LOOP == msg.parameter);
    } else {
        errorCode_m = ERROR_CODE_NO_FILE_NUM;
        mp3Status_m = MP3_STATUS_IDLE;
        sendReplay();
        deinit();
    }
    mp3Status_m = MP3_STATUS_IDLE;
}

void cMp3Server::setVolume(uint32_t volume) {
    errorCode_m = ERROR_CODE_WRONG_VOLUME;
    if (volume >= 0 && volume <=100) {
        volume_m = volume;
        errorCode_m = ERROR_CODE_OK;
        float vol = volume / 100.0;
        mpg123_volume(mh_m, vol);
    }
}

void cMp3Server::init(void) {
    /* Initialization */
    ao_initialize();
    ao_driver_id_m = ao_default_driver_id();
    mpg123_init();
    mh_m = mpg123_new(NULL, &err_m);
    buffer_mpg_size_m = mpg123_outblock(mh_m);
    buffer_mpg_size_m = MP3_SAMPLE_SIZE;
    buffer_mpg_m = (unsigned char*) malloc(buffer_mpg_size_m * sizeof(unsigned char));
}

void cMp3Server::deinit(void) {
    /* clean up */
    free(buffer_mpg_m);
    ao_close(ao_dev_m);
    mpg123_close(mh_m);
    mpg123_delete(mh_m);
    mpg123_exit();
    ao_shutdown();
    buffer_mpg_m = 0;
    ao_dev_m = 0;
    mh_m = 0;

    buffer_mpg_size_m = 0;
    buffer_mpg_done_m = 0;
    err_m = 0;
    ao_driver_id_m = 0;
}
