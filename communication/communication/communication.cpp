/*
 * app_communication.cpp
 *
 *  Created on: Apr 25, 2016
 *      Author: mariusz
 */
#include "communication.h"
#include <utilities/file_handler.h>
#include <utilities/logger.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

void cCommunication::handleCommand(char* cmdMsg, char* replyMsg) {
    //printf("Received: %s", cmdMsg);
    msgParser_m.parseMsg(cmdMsg, strlen(cmdMsg));
    time_m.getTime(time_str_m);
    command_m = msgParser_m.getMsgCommand();
    logPrintf(SCREEN_LOG, "Received %d command\n", command_m);

    switch (command_m) {
    case TCP_SET_TIME:
        handleSetTime(replyMsg);
        break;
    case TCP_PLAY:
        handlePlay(replyMsg);
        break;
    case TCP_STOP:
        handleStop(replyMsg);
        break;
    case TCP_SET_CFG:
        handleSetCfg(replyMsg);
        break;
    case TCP_GET_CFG:
        handleGetCfg(replyMsg);
        break;
    case TCP_SET_VOLUME:
        handleVolume(replyMsg);
        break;
    case TCP_RESET:
        handleReset(replyMsg);
        break;
    case TCP_STATUS:
        handleStatus(replyMsg);
        break;
    case TCP_CMD_NUM:
    default:
        sprintf(replyMsg,"UNKNOWN_COMMAND\n%s\nEND\n",time_str_m);
        break;
    }
}

void cCommunication::handleSetTime(char* replyMsg) {
    eErrorCode_t error_code = ERROR_CODE_SET_TIME_ERROR;
    if (TIME_SET_OK == time_m.setTime(msgParser_m.getMsgNNodeStr(1))) {
        error_code = ERROR_CODE_OK;
    }
    time_m.getTime(time_str_m);
    sprintf(replyMsg, "COMMAND_%d_RECEIVED\n%s\nERROR_CODE:%d\nEND\n", command_m, time_str_m, error_code);
}

void cCommunication::handlePlay(char* replyMsg) {
    mp3Cmd_m.action = MP3_PLAY;
    if(0==strcmp(msgParser_m.getMsgNNodeStr(2),"ONCE")){
        mp3Cmd_m.parameter = MP3_PARAM_ONCE;
    } else if (0==strcmp(msgParser_m.getMsgNNodeStr(2),"IN_LOOP")) {
        mp3Cmd_m.parameter = MP3_PARAM_IN_LOOP;
    } else {
        mp3Cmd_m.parameter = MP3_PARAM_NUM;
    }
    mp3Cmd_m.file_num = atoi(msgParser_m.getMsgNNodeStr(1));
    mqSend_m->push(mp3Cmd_m);
    mqReply_m->wait(mp3Reply_m, -1);
    sprintf(replyMsg, "COMMAND_%d_RECEIVED\n%s\n%s\nERROR_CODE:%d\nEND\n", command_m, time_str_m, MP3_STATUS_STR[mp3Reply_m.mp3Status], mp3Reply_m.errorCode);
}

void cCommunication::handleStop(char* replyMsg) {
    mp3Cmd_m.action = MP3_STOP;
    mp3Cmd_m.parameter = MP3_PARAM_NUM;
    mp3Cmd_m.file_num = 0;
    mqSend_m->push(mp3Cmd_m);
    mqReply_m->wait(mp3Reply_m, -1);
    sprintf(replyMsg, "COMMAND_%d_RECEIVED\n%s\n%s\nERROR_CODE:%d\nEND\n", command_m, time_str_m, MP3_STATUS_STR[mp3Reply_m.mp3Status], mp3Reply_m.errorCode);
}

void cCommunication::handleSetCfg(char* replyMsg) {
    eErrorCode_t error_code = ERROR_CODE_OK;
    FILE* dest = fopen(config_file, "wb");
        fwrite(msgParser_m.getMsgNNodeStr(2), 1, strlen(msgParser_m.getMsgNNodeStr(2)), dest);
    fclose(dest);
    //printf("Config: %s \n",msgParser_m.getMsgNNodeStr(2));
    sprintf(replyMsg, "COMMAND_%d_RECEIVED\n%s\nERROR_CODE:%d\nEND\n", command_m, time_str_m, error_code);
}

void cCommunication::handleGetCfg(char* replyMsg) {
    char configStream[MSG_NODE_SIZE];
    size_t configSize = MSG_NODE_SIZE;
    eErrorCode_t error_code = ERROR_CODE_GET_CFG_ERROR;
    if (CONFIG_MANAGER_OK == conifgManager_m->getConfigFile(configStream, configSize)) {
        error_code = ERROR_CODE_OK;
    }
    sprintf(replyMsg, "COMMAND_%d_RECEIVED\n%s\n%lu\n%s\nERROR_CODE:%d\nEND\n", command_m, time_str_m, configSize, configStream, error_code);
}

void cCommunication::handleVolume(char* replyMsg) {

    mp3Cmd_m.action = MP3_VOLUME;
    mp3Cmd_m.parameter = atoi(msgParser_m.getMsgNNodeStr(1));
    mp3Cmd_m.file_num = 0;
    mqSend_m->push(mp3Cmd_m);
    mqReply_m->wait(mp3Reply_m, -1);
    sprintf(replyMsg, "COMMAND_%d_RECEIVED\n%s\n%s\nERROR_CODE:%d\nEND\n", command_m, time_str_m, MP3_STATUS_STR[mp3Reply_m.mp3Status], mp3Reply_m.errorCode);
}

void cCommunication::handleReset(char* replyMsg) {
    logPrintf(SCREEN_LOG, "system reboot!\n");
    system("reboot");
    sprintf(replyMsg, "COMMAND_%d_RECEIVED\n%s\nERROR_CODE:%d\nEND\n", command_m, time_str_m, ERROR_CODE_OK);
}

void cCommunication::handleStatus(char* replyMsg) {
    mp3Cmd_m.action = MP3_STATUS;
    mp3Cmd_m.parameter = MP3_PARAM_NUM;
    mp3Cmd_m.file_num = 0;
    mqSend_m->push(mp3Cmd_m);
    mqReply_m->wait(mp3Reply_m, -1);
    sprintf(replyMsg, "COMMAND_%d_RECEIVED\n%s\n%s\nERROR_CODE:%d\nEND\n", command_m, time_str_m, MP3_STATUS_STR[mp3Reply_m.mp3Status], mp3Reply_m.errorCode);
}

