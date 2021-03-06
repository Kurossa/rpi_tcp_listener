/*
 * player_play_state.cpp
 *
 *  Created on: Dec 24, 2017
 *      Author: mariusz
 */

#include "player_pause_state.h"
#include "player_play_state.h"
#include "player_stop_state.h"
#include "mp3_player.h"

using namespace mp3player;

PlayerPauseState::PlayerPauseState(Mp3Player& mp3_player, std::string& file_name, PlayMode play_mode):
    PlayerState(mp3_player),
    play_mode_m(play_mode),
    play_file_name_m(file_name)
{
    mp3_player.StopPlayThread();
}

Status PlayerPauseState::Play(std::string file_name, PlayMode play_mode)
{
    printf("Pause state, stop prev play and play new file: %s\n", file_name.c_str());
    mp3_player_m.ClosePlayer();
    mp3_player_m.OpenPlayer(file_name);
    {
        mp3_player_m.SetState(new PlayerStopState(mp3_player_m));
        return Status::FILE_NOT_FOUND;
    }
    mp3_player_m.SetState(new PlayerPlayState(mp3_player_m, file_name, play_mode));
    return Status::SUCCESS;
}

Status PlayerPauseState::Pause()
{
    printf("Pause state, already paused\n");
    return Status::SUCCESS;
}

Status PlayerPauseState::Resume()
{
    printf("Pause state, changing to Play state\n");
    // Test file if exists
    FILE* source = fopen(play_file_name_m.c_str(), "rb");
    if (source) {
        fclose(source);
    } else {
        // File not found or file corrupted
        mp3_player_m.SetState(new PlayerStopState(mp3_player_m));
        return Status::FILE_NOT_FOUND;
    }
    mp3_player_m.SetState(new PlayerPlayState(mp3_player_m, play_file_name_m, play_mode_m));
    return Status::SUCCESS;
}

Status PlayerPauseState::Stop()
{
    printf("Pause state, changing to Stop state\n");
    mp3_player_m.SetState(new PlayerStopState(mp3_player_m));
    return Status::SUCCESS;
}

Status PlayerPauseState::SetVolume(uint16_t volume)
{
    printf("Pause state, Set volume: %d\n", volume);
    if (volume <=100) {
        mp3_player_m.volume_m = volume;
        float volume_float = volume / 100.0;
        mpg123_volume(mp3_player_m.mh_m, volume_float);
        return Status::SUCCESS;
    }
    return Status::FAILED;
}
