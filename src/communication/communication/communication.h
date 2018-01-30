/*
 * app_communication.hpp
 *
 *  Created on: Apr 25, 2016
 *      Author: Kurossa
 */
#include "msg_parser.h"
#include <mp3/mp3.h>
#include <config_manager/config_manager.h>
#include <utilities/time.h>

#include <string>

#ifndef COMMUNICATION_COMMUNICATION_H_
#define COMMUNICATION_COMMUNICATION_H_

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

    int command_m;
    std::string time_str_m;

};

#endif // COMMUNICATION_COMMUNICATION_H_
