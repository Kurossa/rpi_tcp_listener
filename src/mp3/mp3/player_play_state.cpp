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
    GetPlayer().StopPlayThread();
    GetPlayer().RunPlayThread(file_name);
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
    GetPlayer().StopPlayThread();
    GetPlayer().SetState(new PlayerStopState(GetPlayer()));
    return Status::PLAY_ONCE;
}

Status PlayerPlayState::SetVolume(uint16_t volume)
{
    printf("Stop state, Set volume: %d", volume);
    if (volume <=100) {
        GetPlayer.volume_m = volume;
        float volume_float = volume / 100.0;
        mpg123_volume(mh_m, volume_float);
        return Status::PLAY_ONCE;
    }
    return Status::FAILED;
}
