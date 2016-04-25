/*
 * app_communication.cpp
 *
 *  Created on: Apr 25, 2016
 *      Author: mariusz
 */
#include <app_communication.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <app_time.hpp>

using namespace std;

void cCommunication::handleCommand(char* cmdMsg, char* replyMsg) {
//    printf("Received: %s", cmdMsg);
    msgParser_m.parseMsg(cmdMsg, strlen(cmdMsg));
//    for (int k = 0; k < msgParser_m.getMsgNodesNum(); ++k) {
//        printf("%s\n", msgParser_m.getMsgNNodeStr(k));
//    }

    char time_str[32];
    cTime time;
    time.getTime(time_str);

    int command = msgParser_m.getMsgCommand();
    switch (command) {
    case TCP_SET_TIME:
        time.setTime(msgParser_m.getMsgNNodeStr(1));
        time.getTime(time_str);
        sprintf(replyMsg,"COMMAND_%d_RECEIVED\n%s\nEND\n",command ,time_str);
        break;
    case TCP_PLAY:
        mp3Cmd_m.action = MP3_PLAY;
        mp3Cmd_m.parameter = MP3_PARAM_IN_LOOP;
        mp3Cmd_m.file = msgParser_m.getMsgNNodeStr(1);
        mqSend_m->push(mp3Cmd_m);
        mqReply_m->wait(mp3Reply_m, -1);
        sprintf(replyMsg,"COMMAND_%d_RECEIVED\n%s\n%s\nEND\n",command ,time_str, MPS_STATUS[mp3Reply_m.status]);
        break;
    case TCP_STOP:
        mp3Cmd_m.action = MP3_STOP;
        mp3Cmd_m.parameter = MP3_PARAM_NUM;
        mp3Cmd_m.file = NULL;
        mqSend_m->push(mp3Cmd_m);
        mqReply_m->wait(mp3Reply_m, -1);
        sprintf(replyMsg,"COMMAND_%d_RECEIVED\n%s\n%s\nEND\n",command ,time_str, MPS_STATUS[mp3Reply_m.status]);
        break;
    case TCP_SET_CFG:
        sprintf(replyMsg,"COMMAND_%d_RECEIVED\n%s\nEND\n",command ,time_str);
        break;
    case TCP_GET_CFG:
        sprintf(replyMsg,"COMMAND_%d_RECEIVED\n%s\nEND\n",command ,time_str);
        break;
    case TCP_SET_VOLUME:
        mp3Cmd_m.action = MP3_VOLUME;
        mp3Cmd_m.parameter = atoi(msgParser_m.getMsgNNodeStr(1));
        mp3Cmd_m.file = NULL;
        mqSend_m->push(mp3Cmd_m);
        mqReply_m->wait(mp3Reply_m, -1);
        sprintf(replyMsg,"COMMAND_%d_RECEIVED\n%s\n%s\nEND\n",command ,time_str, MPS_STATUS[mp3Reply_m.status]);
        break;
    case TCP_RESET:
#warning //TODO: uncomment system reboot in final version
        //system("reboot");
        sprintf(replyMsg,"COMMAND_%d_RECEIVED\n%s\nEND\n",command ,time_str);
        break;
    case TCP_STATUS:
        mp3Cmd_m.action = MP3_STATUS;
        mp3Cmd_m.parameter = MP3_PARAM_NUM;
        mp3Cmd_m.file = NULL;
        mqSend_m->push(mp3Cmd_m);
        mqReply_m->wait(mp3Reply_m, -1);
        sprintf(replyMsg,"COMMAND_%d_RECEIVED\n%s\n%s\nEND\n",command ,time_str, MPS_STATUS[mp3Reply_m.status]);
        break;
    case TCP_CMD_NUM:
    default:
        sprintf(replyMsg,"UNKNOWN_COMMAND\n%s\nEND\n",time_str);
        break;
    }
}

