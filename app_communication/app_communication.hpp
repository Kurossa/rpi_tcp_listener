/*
 * app_communication.hpp
 *
 *  Created on: Apr 25, 2016
 *      Author: mariusz
 */
#include <app_mp3.hpp>
#include <app_msg_parser.hpp>

#ifndef APP_COMMUNICATION_APP_COMMUNICATION_HPP_
#define APP_COMMUNICATION_APP_COMMUNICATION_HPP_

class cCommunication {
public:
    cCommunication(mqSend_t* mqSend, mqReply_t* mqReply) : mqSend_m(mqSend), mqReply_m(mqReply) {}
    ~cCommunication(void) {}
    void handleCommand(char* cmdMsg,char* replyMsg);
private:
    mqSend_t* mqSend_m;
    mqReply_t* mqReply_m;
    sMp3Message mp3Cmd_m;
    sMp3Reply mp3Reply_m;
    cMsgParser msgParser_m;

};

#endif /* APP_COMMUNICATION_APP_COMMUNICATION_HPP_ */
