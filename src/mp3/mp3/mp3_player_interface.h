/*
 * mp3_player_interface.h
 *
 *  Created on: Dec 24, 2017
 *      Author: Kurossa
 */

#ifndef MP3_PLAYER_INTERFACE_H_
#define MP3_PLAYER_INTERFACE_H_

#include <stdint.h>
#include <unistd.h>
#include <app_config/app_constants.h>

namespace mp3server
{

enum Action {
    PLAY = 0,
    STOP,
    VOLUME,
    STATUS,
    PAUSE,
    CONTINUE,
    ACTION_MAX
};

enum Play {
    ONCE = 0,
    IN_LOOP,
    PLAY_MAX
};

enum Status {
    IDLE = 0,
    PLAY_ONCE,
    PLAY_IN_LOOP,
    PLAY_PAUSED,
    FAILED,
    STATUS_MAX
};

const char STATUS_STR[STATUS_MAX][64] = { "AUDIO_IDLE",
                                          "AUDIO_PLAY_ONCE",
                                          "AUDIO_PLAY_IN_LOOP",
                                          "AUDIO_PLAY_PAUSED"
                                          "AUDIO_FAILED" };

struct Message
{
    Action action;
    uint16_t parameter;
    uint16_t file_num;
    size_t file_size;
};

struct Reply
{
    Status status;
    eErrorCode_t error_code;
};

} // namespace mp3server

#endif // MP3_PLAYER_INTERFACE_H_
