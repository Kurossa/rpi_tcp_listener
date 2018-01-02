/*
 * mp3_player.h
 *
 *  Created on: Dec 24, 2017
 *      Author: Kurossa
 */

#ifndef MP3_PLAYER_H_
#define MP3_PLAYER_H_

#include "mp3_player_interface.h"
#include "player_stop_state.h"
#include "player_play_state.h"

#include <utilities/message_queue.h>
#include <app_config/app_constants.h>

#include <string>
#include <vector>
#include <memory>
#include <thread>
#include <ao/ao.h>
#include <mpg123.h>

namespace mp3server
{

class Mp3Player
{

public:
    Mp3Player():
        mh_m(0),
        buffer_mpg_m(0),
        buffer_mpg_size_m(0),
        buffer_mpg_done_m(0),
        err_m(0),
        ao_driver_id_m(0),
        ao_dev_m(0),
        volume_m(100),
        state_m(new PlayerStopState(*this)),
        play_thread_stop_m(false) {}
    ~Mp3Player() { Stop(); }

    void SetState(PlayerState* new_state);
    Status Play(std::string file_name);
    Status Pause();
    Status Stop();
    Status SetVolume(uint16_t volume);

    void RunPlayThread(std::string& file_name);
    void StopPlayThread();

private:
    void DoPlay(std::string file_name);
    void InitPlayer(void);
    void ResetPlayer(void);

    // mpg123 and ao variables
    mpg123_handle *mh_m;
    unsigned char *buffer_mpg_m;
    size_t buffer_mpg_size_m;
    size_t buffer_mpg_done_m;
    int err_m;
    int ao_driver_id_m;
    ao_device *ao_dev_m;

    // Volume value
    uint32_t volume_m;

    // Player state
    std::mutex state_mutex_m;
    std::unique_ptr<PlayerState> state_m;

    // Player play thread
    std::mutex play_thread_mutex_m;
    bool play_thread_stop_m;
    std::thread play_thread_m;
};

} // namespace mp3server

#endif // MP3_PLAYER_H_
