/*
 * player_stop_state.h
 *
 *  Created on: Dec 24, 2017
 *      Author: Kurossa
 */

#ifndef PLAYER_STOP_STATE_H_
#define PLAYER_STOP_STATE_H_

#include "player_state_interface.h"

namespace mp3server
{

class PlayerStopState : public PlayerState
{
public:
    PlayerStopState(Mp3Player& mp3_player) : PlayerState(mp3_player) {}
    ~PlayerStopState() {}
    Status Play(std::string file_name) override final;
    Status Pause() override final;
    Status Stop() override final;
    Status SetVolume(uint16_t volume) override final;
private:

};

} // namespace mp3server

#endif // PLAYER_STOP_STATE_H_
