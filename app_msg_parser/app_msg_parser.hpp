/*
 * app_msg_parser.hpp
 *
 *  Created on: Apr 20, 2016
 *      Author: mariusz
 */

#ifndef APP_MSG_PARSER_APP_MSG_PARSER_HPP_
#define APP_MSG_PARSER_APP_MSG_PARSER_HPP_
/*
 * Example message
TCP_SET_TIME\n
12:50:00_13.06.2011\n
END\n
 COMMAND_0_RECIEVED\n
 12:45:50_13.06.2011\n
 ERROR_CODE:0\n */
#define MSG_COMAND_SIZE (128)
#define MSG_NODE_SIZE   (2048)
#define MSG_MAX_NODES   (5)

typedef enum eMsgCmd {
    TCP_SET_TIME,       // 0
    TCP_PLAY,           // 1
    TCP_STOP,           // 2
    TCP_SET_CFG,        // 3
    TCP_GET_CFG,        // 4
    TCP_SET_VOLUME,     // 5
    TCP_RESET,          // 6
    TCP_STATUS,         // 7
    TCP_CMD_NUM         // 8
}eMsgCmd_t;

const char MSG_COMMAND[TCP_CMD_NUM][MSG_COMAND_SIZE] = { "TCP_SET_TIME",
                                                         "TCP_PLAY",
                                                         "TCP_STOP",
                                                         "TCP_SET_CFG",
                                                         "TCP_GET_CFG",
                                                         "TCP_SET_VOLUME",
                                                         "TCP_RESET",
                                                         "TCP_STATUS" };

const int MSG_COMMAND_NODES[TCP_CMD_NUM] = { 3,     //"TCP_SET_TIME"
                                             4,     //"TCP_PLAY"
                                             2,     //"TCP_STOP"
                                             4,     //"TCP_SET_CFG"
                                             2,     //"TCP_GET_CFG"
                                             3,     //"TCP_SET_VOLUME"
                                             2,     //"TCP_RESET"
                                             2};    //"TCP_STATUS"

const char MSG_COMMAND_END[] = "END";

typedef enum eMsgParserStatus {
    MSG_PARS_OK,
    MSG_PARS_NOK,
    MSG_PARSER_NUM
} eMsgParserStatus_t;

class cMsgParser {
public:
    cMsgParser() : msgNodesNum_m(0) {}
    ~cMsgParser() {}
    void parseMsg(char* msg, int msgLength);
    int getMsgNodesNum(void) { return msgNodesNum_m; }
    char* getMsgNNodeStr(int node) { return &msgNodesStr_m[node][0]; }

    eMsgCmd_t getMsgCommand(void);

private:
    void parseSetCfgMsg(char* msg, int msgLength);
    int msgNodesNum_m;
    char msgNodesStr_m[MSG_MAX_NODES][MSG_NODE_SIZE];
};


#endif /* APP_MSG_PARSER_APP_MSG_PARSER_HPP_ */
