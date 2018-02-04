/*
 * app_msg_parser.cpp
 *
 *  Created on: Apr 20, 2016
 *      Author: Kurossa
 */
#include "msg_parser.h"
#include <utilities/logger.h>
#include <string.h>
#include <stdlib.h>


void MsgParser::ParseMsg(const char* msg, const size_t msg_length) {
    parsed_masage_m.clear();
    std::string node_str;

    for (size_t msg_read_cnt = 0; msg_read_cnt < msg_length; ++msg_read_cnt) {
        if (parsed_masage_m.size() >= MSG_MAX_NODES) {
            break;
        }

        // If message is TCP_SET_CFG, get config from message.
        if (   2 == parsed_masage_m.size()
            && 0 == strcmp(parsed_masage_m.at(0).c_str(), msg_commands[TCP_SET_CFG]))
        {
            size_t conf_size = 0;
            GetConfigStream(msg, msg_length, msg_read_cnt, conf_size);
            // if error return status
            // return status;
            msg_read_cnt += conf_size;
            node_str.clear();
        }

        if (*(msg+msg_read_cnt) == '\n') {
            parsed_masage_m.push_back(node_str);
            node_str.clear();
        } else {
            node_str += *(msg+msg_read_cnt);
        }

    }
}

void MsgParser::GetConfigStream(  const char* msg
                                , const size_t msg_length
                                , const size_t offset
                                , size_t& read_cfg_size)
{
    read_cfg_size = atoi(parsed_masage_m.at(1).c_str());
    std::string config_stram;

    if ((read_cfg_size + offset) >=  msg_length) {
        parsed_masage_m.push_back("");
        //return error;
    }

    for (size_t msg_read_cnt = 0; msg_read_cnt < read_cfg_size; ++msg_read_cnt) {
        char c = *(msg + offset + msg_read_cnt);
        if ('\r' == c) {
            config_stram += '\n';
        } else {
            config_stram += c;
        }
    }

    parsed_masage_m.push_back(config_stram);
    //return error;
}

std::string MsgParser::GetMsgStrAt(size_t index) const
{
    if (index >= parsed_masage_m.size())
    {
        return "";
    }
    return parsed_masage_m.at(index);
}

MsgCmd MsgParser::GetMsgCommand(void) {
    MsgCmd command = TCP_CMD_NUM;
    if(0 == strcmp(parsed_masage_m.at(parsed_masage_m.size() - 1).c_str(), msg_command_end)) {
        for (int k = 0; k < TCP_CMD_NUM; ++k) {
            if (0 == strcmp(parsed_masage_m.at(0).c_str(), msg_commands[k])) {
                return command = (MsgCmd)k;
            }
        }
    }
    if (msg_commands_nodes[command] != parsed_masage_m.size()) {
        command = TCP_CMD_NUM;
    }
    return command;
}


