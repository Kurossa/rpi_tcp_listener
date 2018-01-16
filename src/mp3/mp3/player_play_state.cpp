/*
 * player_play_state.cpp
 *
 *  Created on: Dec 24, 2017
 *      Author: mariusz
 */

#include "player_play_state.h"
#include "player_pause_state.h"
#include "player_stop_state.h"
#include "mp3_player.h"

using namespace mp3server;

namespace
{

State PlayModeToState(PlayMode play_mode)
{
    switch(play_mode)
    {
    case PlayMode::IN_LOOP:
        return State::PLAY_IN_LOOP;
    default:
        return State::PLAY_ONCE;
    }
}

}

PlayerPlayState::PlayerPlayState(Mp3Player& mp3_player, std::string& file_name, PlayMode play_mode):
    PlayerState(mp3_player),
    play_mode_m(play_mode),
    play_file_name_m(file_name)
{
    mp3_player.RunPlayThread(file_name, play_mode);
}

Status PlayerPlayState::Play(std::string file_name, PlayMode play_mode)
{
    printf("Play state, stop prev play and play new file: %s\n", file_name.c_str());
    play_mode_m = play_mode;
    play_file_name_m = file_name;
    mp3_player_m.StopPlayThread();
    mp3_player_m.ClosePlayer();
    mp3_player_m.OpenPlayer(file_name);
    mp3_player_m.RunPlayThread(file_name, play_mode);
    return Status::SUCCESS;
}

Status PlayerPlayState::Pause()
{
    printf("Play state, changing to Pause state\n");
    mp3_player_m.SetState(new PlayerPauseState(mp3_player_m, play_file_name_m, play_mode_m));
    return Status::SUCCESS;
}

Status PlayerPlayState::Resume()
{
    printf("Play state, already playing, nothing to resume\n");
    return Status::SUCCESS;
}

Status PlayerPlayState::Stop()
{
    printf("Play state, changing to Stop state\n");
    mp3_player_m.SetState(new PlayerStopState(mp3_player_m));
    return Status::SUCCESS;
}

Status PlayerPlayState::SetVolume(uint16_t volume)
{
    printf("Play state, Set volume: %d\n", volume);
    if (volume <=100) {
        mp3_player_m.volume_m = volume;
        float volume_float = volume / 100.0;
        mpg123_volume(mp3_player_m.mh_m, volume_float);
        return Status::SUCCESS;
    }
    return Status::FAILED;
}

State PlayerPlayState::GetState()
{
    return PlayModeToState(play_mode_m);
}

std::string PlayerPlayState::GetStateStr()
{
    return std::string(STATE_STR[PlayModeToState(play_mode_m)]);
}

