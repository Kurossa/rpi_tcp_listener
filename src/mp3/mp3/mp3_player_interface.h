/*
 * mp3_player_interface.h
 *
 *  Created on: Dec 24, 2017
 *      Author: Kurossa
 */

#ifndef MP3_MP3_PLAYER_INTERFACE_H_
#define MP3_MP3_PLAYER_INTERFACE_H_

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

enum PlayMode {
    ONCE = 0,
    IN_LOOP,
    PLAY_MODE_MAX
};

enum State {
    STOPED = 0,
    PLAY_ONCE,
    PLAY_IN_LOOP,
    PAUSED,
    STATE_MAX
};

enum Status {
    SUCCESS = 0,
    FILE_NOT_FOUND,
    FAILED,
    STATUS_MAX
};

const char STATE_STR[STATE_MAX][64] = { "AUDIO_IDLE",
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

#endif // MP3_MP3_PLAYER_INTERFACE_H_
