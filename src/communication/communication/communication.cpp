/*
 * app_communication.cpp
 *
 *  Created on: Apr 25, 2016
 *      Author: Kurossa
 */

#include "communication.h"
#include <cstdio>
#include <cstring>

using namespace utils;
using namespace comm;

void Communication::handleCommand(char* cmdMsg, char* replyMsg) {
    //printf("Received: %s", cmdMsg);
    msg_parser_m.ParseMsg(cmdMsg, strlen(cmdMsg));
    utils::GetTime(time_str_m);
    command_m = msg_parser_m.GetMsgCommand();
    logger_m.Log(LogLevel::SCREEN, "Received %d command\n", command_m);

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
        sprintf(replyMsg,"UNKNOWN_COMMAND\n%s\nEND\n",time_str_m.c_str());
        break;
    }
}

void Communication::handleSetTime(char* replyMsg) {
    ErrorCode error_code = ERROR_CODE_SET_TIME_ERROR;
    if (utils::TIME_SET_OK == utils::SetTime(msg_parser_m.GetMsgStrAt(1))) {
        error_code = ERROR_CODE_OK;
    }
    utils::GetTime(time_str_m);
    sprintf(replyMsg, "COMMAND_%d_RECEIVED\n%s\nERROR_CODE:%d\nEND\n", command_m, time_str_m.c_str(), error_code);
}

void Communication::handlePlay(char* replyMsg) {
    int file_num = atoi(msg_parser_m.GetMsgStrAt(1).c_str());
    mp3player::PlayMode play_mode = mp3player::PlayMode::ONCE;

    if(0==strcmp(msg_parser_m.GetMsgStrAt(2).c_str(),"ONCE")){
        play_mode = mp3player::PlayMode::ONCE;
    } else if (0==strcmp(msg_parser_m.GetMsgStrAt(2).c_str(),"IN_LOOP")) {
        play_mode = mp3player::PlayMode::IN_LOOP;
    } else {
        play_mode = mp3player::PlayMode::PLAY_MODE_MAX;
    }

    mp3_player_m.Play(config_manager_m.GetConfig().sound_files.at(file_num).c_str(), play_mode);
    sprintf(replyMsg, "COMMAND_%d_RECEIVED\n%s\n%s\nERROR_CODE:%d\nEND\n", command_m, time_str_m.c_str(), mp3_player_m.GetStateStr().c_str(), ERROR_CODE_OK);
}

void Communication::handleStop(char* replyMsg) {
    mp3_player_m.Stop();
    sprintf(replyMsg, "COMMAND_%d_RECEIVED\n%s\n%s\nERROR_CODE:%d\nEND\n", command_m, time_str_m.c_str(), mp3_player_m.GetStateStr().c_str(), ERROR_CODE_OK);
}

void Communication::handleSetCfg(char* replyMsg) {
    ErrorCode error_code = ERROR_CODE_OK;
    FILE* dest = fopen(config_file_g, "wb");
    fwrite(msg_parser_m.GetMsgStrAt(2).c_str(), 1, strlen(msg_parser_m.GetMsgStrAt(2).c_str()), dest);
    fclose(dest);
    //printf("Config: %s \n",msgParser_m.getMsgNNodeStr(2));
    sprintf(replyMsg, "COMMAND_%d_RECEIVED\n%s\nERROR_CODE:%d\nEND\n", command_m, time_str_m.c_str(), error_code);
}

void Communication::handleGetCfg(char* replyMsg) {
    ErrorCode error_code = ERROR_CODE_GET_CFG_ERROR;
    std::string stream;
    if (config::Status::OK == config_manager_m.GetConfigFromFile(stream)) {
        error_code = ERROR_CODE_OK;
    }
    sprintf(replyMsg, "COMMAND_%d_RECEIVED\n%s\n%lu\n%s\nERROR_CODE:%d\nEND\n", command_m, time_str_m.c_str(), (long unsigned int)stream.size(), stream.c_str(), error_code);
}

void Communication::handleVolume(char* replyMsg) {

    int volume = atoi(msg_parser_m.GetMsgStrAt(1).c_str());
    mp3_player_m.SetVolume(volume);
    sprintf(replyMsg, "COMMAND_%d_RECEIVED\n%s\n%s\nERROR_CODE:%d\nEND\n", command_m, time_str_m.c_str(), mp3_player_m.GetStateStr().c_str(), ERROR_CODE_OK);
}

void Communication::handleReset(char* replyMsg) {
    logger_m.Log(LogLevel::SCREEN, "system reboot!\n");
    mp3_player_m.Stop();
    sprintf(replyMsg, "COMMAND_%d_RECEIVED\n%s\nERROR_CODE:%d\nEND\n", command_m, time_str_m.c_str(), ERROR_CODE_OK);
    reboot_callback_m();
}

void Communication::handleStatus(char* replyMsg) {
    sprintf(replyMsg, "COMMAND_%d_RECEIVED\n%s\n%s\nERROR_CODE:%d\nEND\n", command_m, time_str_m.c_str(), mp3_player_m.GetStateStr().c_str(), ERROR_CODE_OK);
}

