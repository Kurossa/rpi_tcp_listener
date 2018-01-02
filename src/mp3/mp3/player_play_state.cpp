/*
 * player_play_state.cpp
 *
 *  Created on: Dec 24, 2017
 *      Author: mariusz
 */

#include "player_play_state.h"
#include "player_stop_state.h"
#include "mp3_player.h"

using namespace mp3server;

Status PlayerPlayState::Play(std::string file_name)
{
    printf("Play state, stop prev play and play new file: %s\n", file_name.c_str());
    mp3_player_m.StopPlayThread();
    mp3_player_m.RunPlayThread(file_name);
    return Status::PLAY_ONCE;
}

Status PlayerPlayState::Pause()
{
    printf("Play state, pause\n");
    return Status::PLAY_ONCE;
}

Status PlayerPlayState::Stop()
{
    printf("Play state, changing to Stop state\n");
    mp3_player_m.StopPlayThread();
    mp3_player_m.SetState(new PlayerStopState(mp3_player_m));
    return Status::PLAY_ONCE;
}

Status PlayerPlayState::SetVolume(uint16_t volume)
{
    printf("Stop state, Set volume: %d", volume);
    if (volume <=100) {
        mp3_player_m.volume_m = volume;
        float volume_float = volume / 100.0;
        mpg123_volume(mp3_player_m.mh_m, volume_float);
        return Status::PLAY_ONCE;
    }
    return Status::FAILED;
}
