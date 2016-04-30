/*
 * app_msg_parser.cpp
 *
 *  Created on: Apr 20, 2016
 *      Author: mariusz
 */
#include <app_msg_parser.hpp>
#include <app_logger.hpp>
#include <string.h>
#include <stdlib.h>


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

    /* Special parser for files */
    parseSetCfgMsg(msg, msgLength);

    //printf("Found %d node(s)\n", msgNodesNum_m);
}

void cMsgParser::parseSetCfgMsg(char* msg, int msgLength) {
    if (0 == strcmp(msgNodesStr_m[0], "TCP_SET_CFG")) {
        size_t msgRead = 0;
        size_t conf_size = atoi(msgNodesStr_m[1]);
        msgNodesNum_m = 0;
        int msgNodeIndex = 0;
        for (int i = 0; i < msgLength; ++i) {
            if (msgNodesNum_m >= MSG_MAX_NODES) {
                break;
            }
            if (conf_size > MSG_NODE_SIZE) {
                break;
            }

            if (msgNodesNum_m == 2) {
                if ((msgLength - msgRead) < conf_size) {
                    break;
                }
                if (0 == conf_size) {
                    msgNodesStr_m[msgNodesNum_m][msgNodeIndex] = '\0';
                    ++msgNodesNum_m;
                    msgNodeIndex = 0;
                } else {
                    if ('\r' == *(msg + i)) {
                        msgNodesStr_m[msgNodesNum_m][msgNodeIndex] = '\n';
                    } else {
                        msgNodesStr_m[msgNodesNum_m][msgNodeIndex] = *(msg + i);
                    }
                    ++msgNodeIndex;
                    --conf_size;
                }
            } else if (*(msg + i) == '\n') {
                msgNodesStr_m[msgNodesNum_m][msgNodeIndex] = '\0';
                ++msgNodesNum_m;
                msgNodeIndex = 0;
            } else {
                msgNodesStr_m[msgNodesNum_m][msgNodeIndex] = *(msg + i);
                ++msgNodeIndex;
            }
            ++msgRead;
        }
    }
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
    return command;
}


