/*
 * mp3_player.h
 *
 *  Created on: Dec 24, 2017
 *      Author: Kurossa
 */

#ifndef MP3_PLAYER_H_
#define MP3_PLAYER_H_

#include "mp3_player_interface.h"
#include "player_state_interface.h"

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
    friend class PlayerPlayState;
    friend class PlayerPauseState;
    friend class PlayerStopState;

public:
    Mp3Player();
    ~Mp3Player();

    Status Play(std::string file_name, PlayMode play_mode);
    Status Pause();
    Status Resume();
    Status Stop();
    Status SetVolume(uint16_t volume);
    State GetPlayMode();
    std::string GetPlayModeStr();

private:
    void SetState(PlayerState* new_state);

    bool OpenPlayer(std::string& file_name);
    void ClosePlayer(void);

    void RunPlayThread(std::string& file_name, PlayMode play_mode);
    void StopPlayThread();

    void DoPlay(std::string file_name, PlayMode play_mode);
    void DoStop();

    // mpg123 variables
    mpg123_handle *mh_m;
    unsigned char *buffer_mpg_m;
    size_t buffer_mpg_size_m;
    size_t buffer_mpg_done_m;

    // ao_driver variables
    int ao_driver_id_m;
    ao_device *ao_dev_m;

    // Volume value
    uint32_t volume_m;

    // Player state
    std::mutex state_mutex_m;
    std::unique_ptr<PlayerState> state_m;

    // Player play thread
    std::mutex stop_thread_mutex_m;
    bool stop_thread_m;
    std::mutex thread_mutex_m;
    std::thread play_thread_m;
    bool thread_is_running_m;

    std::mutex cv_mutex_m;
    bool cv_ready_m;
    bool stop_auto_stop_thread_m;
    std::condition_variable cv_m;
    std::thread::id thread_id_to_stop_m;

    std::thread auto_stop_thread_m;
};

} // namespace mp3server

#endif // MP3_PLAYER_H_
