/*
 * app_communication.h
 *
 *  Created on: Apr 25, 2016
 *      Author: Kurossa
 */

#include "msg_parser.h"
#include <config_manager/config_manager.h>
#include <mp3/mp3_player.h>
#include <utilities/time.h>
#include <string>

#ifndef COMMUNICATION_COMMUNICATION_H_
#define COMMUNICATION_COMMUNICATION_H_

namespace comm
{

class Communication {
public:
    Communication(const config::ConfigManager& config_manager) :
            config_manager_m(config_manager),
            command_m(0) {}
    ~Communication(void) {}
    void handleCommand(char* cmdMsg,char* replyMsg);
    void handleSetTime(char* replyMsg);
    void handlePlay(char* replyMsg);
    void handleStop(char* replyMsg);
    void handleSetCfg(char* replyMsg);
    void handleGetCfg(char* replyMsg);
    void handleVolume(char* replyMsg);
    void handleReset(char* replyMsg);
    void handleStatus(char* replyMsg);
private:
    const config::ConfigManager config_manager_m;
    MsgParser msg_parser_m;
    mp3player::Mp3Player mp3_player_m;

    int command_m;
    std::string time_str_m;

};

} // namespace comm


#endif // COMMUNICATION_COMMUNICATION_H_
