/*
 * app_communication.hpp
 *
 *  Created on: Apr 25, 2016
 *      Author: mariusz
 */
#include <app_mp3.hpp>
#include <app_msg_parser.hpp>
#include <app_config_manager.hpp>
#include <app_time.hpp>

#ifndef APP_COMMUNICATION_APP_COMMUNICATION_HPP_
#define APP_COMMUNICATION_APP_COMMUNICATION_HPP_

class cCommunication {
public:
    cCommunication(mqSend_t* mqSend, mqReply_t* mqReply, cConfigManager* conifgManager) :
            mqSend_m(mqSend),
            mqReply_m(mqReply),
            conifgManager_m(conifgManager),
            command_m(0) {
    }
    ~cCommunication(void) {}
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
    mqSend_t* mqSend_m;
    mqReply_t* mqReply_m;
    cConfigManager* conifgManager_m;
    sMp3Message mp3Cmd_m;
    sMp3Reply mp3Reply_m;
    cMsgParser msgParser_m;
    cTime time_m;

    int command_m;
    char time_str_m[32];

};

#endif /* APP_COMMUNICATION_APP_COMMUNICATION_HPP_ */
