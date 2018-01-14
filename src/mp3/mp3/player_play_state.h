/*
 * player_play_state.h
 *
 *  Created on: Dec 24, 2017
 *      Author: Kurossa
 */

#ifndef PLAYER_PLAY_STATE_H_
#define PLAYER_PLAY_STATE_H_

#include "player_state_interface.h"

namespace mp3server
{

class PlayerPlayState : public PlayerState
{
public:
    //play_mode set to other than IN_LOOP will be converted to ONCE
    PlayerPlayState(Mp3Player& mp3_player, std::string& file_name, PlayMode play_mode);
    ~PlayerPlayState() {}
    Status Play(std::string file_name, PlayMode play_mode) override final;
    Status Pause() override final;
    Status Resume() override final;
    Status Stop() override final;
    Status SetVolume(uint16_t volume) override final;
    PlayMode GetPlayMode() override final { return play_mode_m; }
    std::string GetPlayModeStr() override final { return std::string(PLAY_MODE_STR[play_mode_m]); }

private:
    Status ConvertPlayModeToStatus(PlayMode play_mode);

    PlayMode play_mode_m;
    std::string play_file_name_m;
};

} // namespace mp3server

#endif // PLAYER_PLAY_STATE_H_
