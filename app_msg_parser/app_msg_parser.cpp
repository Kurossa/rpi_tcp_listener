/*
 * app_msg_parser.cpp
 *
 *  Created on: Apr 20, 2016
 *      Author: mariusz
 */
#include <app_msg_parser.hpp>
#include <app_logger.hpp>
#include <string.h>


void cMsgParser::parseMsg(char* msg, int msgLength) {
    msgNodesNum_m = 0;
    int msgNodeIndex = 0;
    for (int i = 0; i < msgLength; ++i) {
        if (msgNodesNum_m >= MSG_MAX_NODES) {
            break;
        }
        if (*(msg+i) == '\n') {
            msgNodesStr_m[msgNodesNum_m][msgNodeIndex] = '\0';
            ++msgNodesNum_m;
            msgNodeIndex = 0;
        } else {
            msgNodesStr_m[msgNodesNum_m][msgNodeIndex]  = *(msg+i);
            ++msgNodeIndex;
        }
    }
    printf("Found %d node(s)\n", msgNodesNum_m);
}

eMsgCmd_t cMsgParser::getMsgCommand(void) {
    eMsgCmd_t command = TCP_CMD_NUM;
    if(0 == strcmp(msgNodesStr_m[msgNodesNum_m-1],MSG_COMMAND_END)) {
        for (int k = 0; k < TCP_CMD_NUM; ++k) {
            if (0 == strcmp(msgNodesStr_m[0],MSG_COMMAND[k])) {
                command = (eMsgCmd_t)k;
            }
        }
    }
    if (MSG_COMMAND_NODES[command] != msgNodesNum_m) {
        command = TCP_CMD_NUM;
    }
    printf ("Command: %d found.\n",command);
    return command;
}


