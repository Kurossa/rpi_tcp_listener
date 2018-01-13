/*
 * player_stop_state.cpp
 *
 *  Created on: Dec 24, 2017
 *      Author: Kurossa
 */

#include "player_stop_state.h"
#include "player_play_state.h"
#include "player_pause_state.h"
#include "mp3_player.h"

using namespace mp3server;

PlayerStopState::PlayerStopState(Mp3Player& mp3_player) : PlayerState(mp3_player)
{
    mp3_player.StopPlayThread();
    mp3_player.ClosePlayer();
}

Status PlayerStopState::Play(std::string file_name, PlayMode play_mode)
{
    printf("Stop state, play file: %s\n", file_name.c_str());
    mp3_player_m.OpenPlayer(file_name);
    mp3_player_m.SetState(new PlayerPlayState(mp3_player_m, file_name, play_mode));
    //TODO: Return proper code after changing the state.
    return GetStatusOk();
}

Status PlayerStopState::Pause()
{
    printf("Stop state, nothing to pause\n");
    return GetStatusOk();
}

Status PlayerStopState::Resume()
{
    printf("Stop state, nothing to resume\n");
    return GetStatusOk();
}

Status PlayerStopState::Stop()
{
    printf("Stop state, already stopped\n");

    return GetStatusOk();
}

Status PlayerStopState::SetVolume(uint16_t volume)
{
    printf("Stop state, Set volume: %d\n", volume);
    if (volume <=100) {
        mp3_player_m.volume_m = volume;
        return GetStatusOk();
    }
    return Status::FAILED;
}
