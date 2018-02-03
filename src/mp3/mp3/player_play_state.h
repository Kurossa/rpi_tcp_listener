/*
 * player_play_state.h
 *
 *  Created on: Dec 24, 2017
 *      Author: Kurossa
 */

#ifndef MP3_PLAYER_PLAY_STATE_H_
#define MP3_PLAYER_PLAY_STATE_H_

#include "player_state_interface.h"

namespace mp3player
{

class PlayerPlayState : public PlayerState
{
public:
    PlayerPlayState(Mp3Player& mp3_player, std::string& file_name, PlayMode play_mode);
    ~PlayerPlayState() {}
    Status Play(std::string file_name, PlayMode play_mode) override final;
    Status Pause() override final;
    Status Resume() override final;
    Status Stop() override final;
    Status SetVolume(uint16_t volume) override final;
    State GetState() override final;
    std::string GetStateStr() override final;
private:
    Status ConvertPlayModeToStatus(PlayMode play_mode);

    PlayMode play_mode_m;
    std::string play_file_name_m;
};

} // namespace mp3server

#endif // MP3_PLAYER_PLAY_STATE_H_
