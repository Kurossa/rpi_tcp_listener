/*
 * app_msg_parser.h
 *
 *  Created on: Apr 20, 2016
 *      Author: Kurossa
 */

#include <cstddef>
#include <string>
#include <vector>

#ifndef COMMUNICATION_MSG_PARSER_H_
#define COMMUNICATION_MSG_PARSER_H_

/*
 * Example message exchange.
 * Command:
 *
 *
 * TCP_SET_TIME\n
 * 12:50:00_13.06.2011\n
 * END\n
 *
 * Reply:
 *
 * COMMAND_0_RECIEVED\n
 * 12:45:50_13.06.2011\n
 * ERROR_CODE:0\n
 */

#define MSG_COMAND_SIZE (128)
#define MSG_NODE_SIZE   (2048)
#define MSG_MAX_NODES   (5)

enum MsgCmd {
    TCP_SET_TIME,       // 0
    TCP_PLAY,           // 1
    TCP_STOP,           // 2
    TCP_SET_CFG,        // 3
    TCP_GET_CFG,        // 4
    TCP_SET_VOLUME,     // 5
    TCP_RESET,          // 6
    TCP_STATUS,         // 7
    TCP_CMD_NUM         // 8
};

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

enum MsgParserStatus {
    MSG_PARS_OK,
    MSG_PARS_NOK,
    MSG_PARSER_NUM
};

class MsgParser {
public:
    MsgParser() {}
    ~MsgParser() {}
    void ParseMsg(const char* msg, const size_t msg_length);
    std::vector<std::string> GetMsgVector() const { return parsed_masage_m; }
    size_t GetMsgVectorSize() const { return parsed_masage_m.size(); }
    std::string GetMsgStrAt(size_t index) const;

    MsgCmd GetMsgCommand(void);

private:
    void GetConfigStream(  const char* msg
                         , const size_t msg_length
                         , const size_t offset
                         , size_t& read_cfg_size);
    //void ParseSetCfgMsg(const char* msg, const int msgLength);
    std::vector<std::string> parsed_masage_m;
    //size_t msg_nodes_num_m;
    char msg_nodes_str_m[MSG_MAX_NODES][MSG_NODE_SIZE];
};


#endif // COMMUNICATION_MSG_PARSER_H_
