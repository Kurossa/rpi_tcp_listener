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
    play_mode_m = play_mode;
    return GetStatusOk();
}

Status PlayerPlayState::Pause()
{
    printf("Play state, changing to Pause state\n");
    mp3_player_m.SetState(new PlayerPauseState(mp3_player_m, play_file_name_m, play_mode_m));
    //TODO: Return proper code after changing the state.
    return GetStatusOk();
}

Status PlayerPlayState::Resume()
{
    printf("Play state, already playing, nothing to resume\n");
    return GetStatusOk();
}

Status PlayerPlayState::Stop()
{
    printf("Play state, changing to Stop state\n");
    mp3_player_m.SetState(new PlayerStopState(mp3_player_m));
    //TODO: Return proper code after changing the state.
    return GetStatusOk();
}

Status PlayerPlayState::SetVolume(uint16_t volume)
{
    printf("Play state, Set volume: %d\n", volume);
    if (volume <=100) {
        mp3_player_m.volume_m = volume;
        float volume_float = volume / 100.0;
        mpg123_volume(mp3_player_m.mh_m, volume_float);
        return GetStatusOk();
    }
    return Status::FAILED;
}

Status PlayerPlayState::GetStatusOk()
{
    return ConvertPlayModeToStatus(play_mode_m);
}

Status PlayerPlayState::ConvertPlayModeToStatus(mp3server::PlayMode play_mode)
{
    switch (play_mode)
    {
    case PlayMode::ONCE:
        return Status::PLAY_ONCE;
    case PlayMode::IN_LOOP:
        return Status::PLAY_IN_LOOP;
    default:
        return Status::FAILED;
    }
}
