/*
 * player_state_interface.h
 *
 *  Created on: Dec 24, 2017
 *      Author: Kurossa
 */

#ifndef PLAYER_STATE_INTERFACE_H_
#define PLAYER_STATE_INTERFACE_H_

#include "mp3_player_interface.h"
#include <string>

namespace mp3server
{

class Mp3Player;

class PlayerState
{
public:
    PlayerState(Mp3Player& mp3_player):
        mp3_player_m(mp3_player) {}
    virtual ~PlayerState() {}

    virtual Status Play(std::string file_name) = 0;
    virtual Status Pause() = 0;
    virtual Status Stop() = 0;
    virtual Status SetVolume(uint16_t volume) = 0;
    Mp3Player& GetPlayer() { return mp3_player_m; }

    virtual void OnEnter() {}
    virtual void OnLeave() {}
private:
    Mp3Player& mp3_player_m;
};

} // namespace mp3server

#endif // PLAYER_STATE_INTERFACE_H_
