/*
 * app_communication.h
 *
 *  Created on: Apr 25, 2016
 *      Author: Kurossa
 */

#include "msg_parser.h"
#include <config_manager/config_manager.h>
#include <mp3/mp3_player.h>
#include <utilities/logger.h>
#include <utilities/time.h>
#include <string>
#include <functional>

#ifndef COMMUNICATION_COMMUNICATION_H_
#define COMMUNICATION_COMMUNICATION_H_

namespace comm
{

class Communication {
public:
    Communication(const config::ConfigManager& config_manager
                  , utils::Logger& logger
                  , std::function<void()> reboot_callback = nullptr) :
            config_manager_m(config_manager),
            logger_m(logger),
            command_m(0),
            reboot_callback_m(reboot_callback) {}
    ~Communication(void) {}
    void handleCommand(const char* cmdMsg,char* replyMsg);

private:
    void handleSetTime(char* replyMsg);
    void handlePlay(char* replyMsg);
    void handleStop(char* replyMsg);
    void handleSetCfg(char* replyMsg);
    void handleGetCfg(char* replyMsg);
    void handleVolume(char* replyMsg);
    void handleReset(char* replyMsg);
    void handleStatus(char* replyMsg);

    const config::ConfigManager config_manager_m;
    MsgParser msg_parser_m;
    mp3player::Mp3Player mp3_player_m;
    utils::Logger& logger_m;
    int command_m;
    std::string time_str_m;
    std::function<void()> reboot_callback_m;
};

} // namespace comm


#endif // COMMUNICATION_COMMUNICATION_H_
