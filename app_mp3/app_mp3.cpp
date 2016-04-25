/*
 * app_mp3.cpp
 *
 *  Created on: Apr 12, 2016
 *      Author: mariusz
 */
#include <unistd.h>
#include <stdio.h>
//#include <ao/ao.h>
//#include <mpg123.h>
#include <app_mp3.hpp>
#include <app_logger.hpp>

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
        sendReplay();
        if (MP3_PARAM_ONCE == msg.parameter) {
            status_m = MP3_STATUS_PLAY_ONCE;
            playMp3(msg);
        }
        else if (MP3_PARAM_IN_LOOP == msg.parameter) {
            status_m = MP3_STATUS_PLAY_IN_LOOP;
            playMp3(msg);
        } else {
            status_m = MP3_STATUS_IDLE;
        }
        break;
    case MP3_STOP:
        status_m = MP3_STATUS_IDLE;
        printf("Stop playing.\n");
        sendReplay();
        break;
    case MP3_VOLUME:
        setVolume(msg.parameter);
        sendReplay();
        break;
    case MP3_STATUS:
        sendReplay();
        break;
    case MP3_PAUSE:
        sendReplay();
        /* Not supported yet. */
    case MP3_CONTINUE:
        sendReplay();
        /* Not supported yet. */
    default:
        break;
    }
}
void cMp3Server::sendReplay(void) {
    sMp3Reply reply;
    reply.status = status_m;
    mqReply_m->push(reply);
}

void cMp3Server::playMp3(sMp3Message& msg) {
    bool new_message = 0;
    sMp3Message msg_poke;
    do {
        printf("Playing: %s, in mode %d\n",msg.file, msg.parameter);
        for(int n_samples = 0; n_samples < 50; ++n_samples) {
            /* MP3 DECODE CODE HERE */
            printf("Sample %02d \n", n_samples);
            usleep(50000);
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
    }
    while (MP3_PARAM_IN_LOOP == msg.parameter && !new_message);
    status_m = MP3_STATUS_IDLE;
}

void cMp3Server::setVolume(uint32_t volume) {
    printf("Volume changed %d.\n", volume);
#warning //TODO: Implement volume change;
}

/* mp3 */
/*    mpg123_handle *mh;
unsigned char *buffer_mpg;
size_t buffer_size;
size_t done;
int err;

int driver;
ao_device *dev;

ao_sample_format format;
int channels, encoding;
long rate;

if (argc < 2)
    exit(0);

//initializations
ao_initialize();
driver = ao_default_driver_id();
mpg123_init();
mh = mpg123_new(NULL, &err);
buffer_size = mpg123_outblock(mh);
buffer_size = 4096;
buffer_mpg = (unsigned char*) malloc(buffer_size * sizeof(unsigned char));

printf("mpg123 buffer size = %d\n", buffer_size);
// open the file and get the decoding format
mpg123_open(mh, argv[1]);
mpg123_getformat(mh, &rate, &channels, &encoding);

// set the output format and open the output device
format.bits = mpg123_encsize(encoding) * BITS;
format.rate = rate;
format.channels = channels;
format.byte_format = AO_FMT_NATIVE;
format.matrix = 0;
dev = ao_open_live(driver, &format, NULL);

// decode and play
int n_samp = 0;
int status = 0;
double base, really, rva_db, vol = 1;
vol = 1;
while ((status = mpg123_read(mh, buffer_mpg, buffer_size, &done)) == MPG123_OK) {
    ++n_samp;
    ao_play(dev, (char *) buffer_mpg, done);
    }
    mpg123_volume(mh, vol);
    //MPG123_EXPORT int mpg123_volume_change(mpg123_handle *mh, double change);

    mpg123_getvolume(mh, &base, &really, &rva_db);
    printf("next sample %d , status %d, base = %f, really = %f, rva_db = %f\n", n_samp, status, base, really, rva_db);
}
printf("next sample %d , status %d\n", n_samp, status);

// clean up
free(buffer_mpg);
ao_close(dev);
mpg123_close(mh);
mpg123_delete(mh);
mpg123_exit();
ao_shutdown();


return 0;*/
