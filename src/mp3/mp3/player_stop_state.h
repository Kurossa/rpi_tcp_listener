/*
 * player_stop_state.h
 *
 *  Created on: Dec 24, 2017
 *      Author: Kurossa
 */

#ifndef MP3_PLAYER_STOP_STATE_H_
#define MP3_PLAYER_STOP_STATE_H_

#include "player_state_interface.h"

namespace mp3server
{

class PlayerStopState : public PlayerState
{
public:
    PlayerStopState(Mp3Player& mp3_player);
    ~PlayerStopState() {}
    Status Play(std::string file_name, PlayMode play_mode) override final;
    Status Pause() override final;
    Status Resume() override final;
    Status Stop() override final;
    Status SetVolume(uint16_t volume) override final;
    State GetState() override final { return State::STOPED; }
    std::string GetStateStr() override final { return std::string(STATE_STR[State::STOPED]); }
private:

};

} // namespace mp3server

#endif // MP3_PLAYER_STOP_STATE_H_
