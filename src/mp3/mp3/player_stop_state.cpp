/*
 * player_stop_state.cpp
 *
 *  Created on: Dec 24, 2017
 *      Author: Kurossa
 */

#include "player_stop_state.h"
#include "player_play_state.h"
#include "mp3_player.h"

using namespace mp3server;

Status PlayerStopState::Play(std::string file_name)
{
    printf("Stop state, play file: %s\n", file_name.c_str());
    GetPlayer().RunPlayThread(file_name);
    GetPlayer().SetState(new PlayerPlayState(GetPlayer()));
    return Status::IDLE;
}

Status PlayerStopState::Pause()
{
    printf("Stop state, not in Play state, can't pause\n");
    return Status::IDLE;
}

Status PlayerStopState::Stop()
{
    printf("Stop state, already stopped\n");

    return Status::IDLE;
}

Status PlayerStopState::SetVolume(uint16_t volume)
{
    printf("Stop state, Set volume: %d", volume);
    return Status::IDLE;
}
