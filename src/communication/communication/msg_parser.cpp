/*
 * app_msg_parser.cpp
 *
 *  Created on: Apr 20, 2016
 *      Author: Kurossa
 */

#include "msg_parser.h"
#include <string.h>
#include <stdlib.h>

using namespace comm;

namespace
{

const char msg_commands[TCP_CMD_NUM][MSG_COMAND_SIZE] = { "TCP_SET_TIME",
                                                          "TCP_PLAY",
                                                          "TCP_STOP",
                                                          "TCP_SET_CFG",
                                                          "TCP_GET_CFG",
                                                          "TCP_SET_VOLUME",
                                                          "TCP_RESET",
                                                          "TCP_STATUS" };

const size_t msg_commands_nodes[TCP_CMD_NUM] = { 3,     //"TCP_SET_TIME"
                                                 4,     //"TCP_PLAY"
                                                 2,     //"TCP_STOP"
                                                 4,     //"TCP_SET_CFG"
                                                 2,     //"TCP_GET_CFG"
                                                 3,     //"TCP_SET_VOLUME"
                                                 2,     //"TCP_RESET"
                                                 2};    //"TCP_STATUS"

const char msg_command_end[] = "END";

} // namespace

ParserStatus MsgParser::ParseMsg(const char* msg, const size_t msg_length) {
    parsed_masage_m.clear();
    std::string node_str;

    for (size_t msg_read_cnt = 0; msg_read_cnt < msg_length; ++msg_read_cnt)
    {
        if (parsed_masage_m.size() >= MSG_MAX_NODES) {
            return ParserStatus::TOO_MANY_NODES;
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

    if (TCP_CMD_NUM == VerifyAndGetMsgCommand())
    {
        return ParserStatus::WRONG_MESSAGE;
    }

    return ParserStatus::OK;
}

std::string MsgParser::GetMsgCommandStr() const
{
    if (parsed_masage_m.size() > 1)
    {
        return parsed_masage_m.at(0);
    }
    return "";
}

std::string MsgParser::GetMsgStrAt(size_t index) const
{
    if (index >= parsed_masage_m.size())
    {
        return "";
    }
    return parsed_masage_m.at(index);
}

std::string MsgParser::GetMsgAtributeNo(size_t index) const
{
    if (parsed_masage_m.size() > (index + 2))
    {
        return parsed_masage_m.at(index + 1);
    }
    return "";
}

MsgCmd MsgParser::VerifyAndGetMsgCommand(void)
{
    if (parsed_masage_m.size()) {
        if(0 == strcmp(parsed_masage_m.at(parsed_masage_m.size() - 1).c_str(), msg_command_end)) {
            for (int command = 0; command < TCP_CMD_NUM; ++command) {
                if (0 == strcmp(parsed_masage_m.at(0).c_str(), msg_commands[command])) {
                    if (msg_commands_nodes[command] != parsed_masage_m.size()) {
                        return command_m = TCP_CMD_NUM;
                    }
                    return command_m = static_cast<MsgCmd>(command);
                }
            }
        }
    }

    return command_m = TCP_CMD_NUM;
}


ParserStatus MsgParser::GetConfigStream(  const char* msg
                                        , const size_t msg_length
                                        , const size_t offset
                                        , size_t& read_cfg_size)
{
    read_cfg_size = atoi(parsed_masage_m.at(1).c_str());
    std::string config_stram;

    if ((read_cfg_size + offset) >=  msg_length) {
        parsed_masage_m.push_back("");
        return ParserStatus::WRONG_CFG_SIZE;
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
    return ParserStatus::OK;
}


