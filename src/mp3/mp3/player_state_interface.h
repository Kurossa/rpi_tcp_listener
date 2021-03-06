/*
 * player_state_interface.h
 *
 *  Created on: Dec 24, 2017
 *      Author: Kurossa
 */

#ifndef MP3_PLAYER_STATE_INTERFACE_H_
#define MP3_PLAYER_STATE_INTERFACE_H_

#include "mp3_player_interface.h"
#include <string>

namespace mp3player
{

class Mp3Player;

class PlayerState
{
public:
    PlayerState(Mp3Player& mp3_player):
        mp3_player_m(mp3_player) {}
    virtual ~PlayerState() {}

    virtual Status Play(std::string file_name, PlayMode play_mode) = 0;
    virtual Status Pause() = 0;
    virtual Status Resume() = 0;
    virtual Status Stop() = 0;
    virtual Status SetVolume(uint16_t volume) = 0;
    virtual State GetState() = 0;
    virtual std::string GetStateStr() = 0;

    virtual Status OnEnter() { return Status::SUCCESS; } // Method used on entering new state during switch
    virtual Status OnLeave() { return Status::SUCCESS; } // Method used on leaving new state during switch

protected:
    Mp3Player& mp3_player_m;
};

} // namespace mp3server

#endif // MP3_PLAYER_STATE_INTERFACE_H_
