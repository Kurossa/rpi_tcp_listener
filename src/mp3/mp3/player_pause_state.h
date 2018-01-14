/*
 * player_play_state.h
 *
 *  Created on: Dec 24, 2017
 *      Author: Kurossa
 */

#ifndef PLAYER_PAUSE_STATE_H_
#define PLAYER_PAUSE_STATE_H_

#include "player_state_interface.h"

namespace mp3server
{

class PlayerPauseState : public PlayerState
{
public:
    PlayerPauseState(Mp3Player& mp3_player, std::string& file_name, PlayMode play_mode);
    ~PlayerPauseState() {}
    Status Play(std::string file_name, PlayMode play_mode) override final;
    Status Pause() override final;
    Status Resume() override final;
    Status Stop() override final;
    Status SetVolume(uint16_t volume) override final;
    PlayMode GetPlayMode() override final { return PlayMode::PAUSED; }
    std::string GetPlayModeStr() override final { return std::string(PLAY_MODE_STR[PlayMode::PAUSED]); }

private:
    PlayMode play_mode_m;
    std::string play_file_name_m;

};

} // namespace mp3server

#endif // PLAYER_PAUSE_STATE_H_
