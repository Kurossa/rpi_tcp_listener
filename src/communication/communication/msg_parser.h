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

namespace comm
{

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

enum ParserStatus {
    OK,
    TOO_MANY_NODES,
    WRONG_MESSAGE,
    WRONG_CFG_SIZE,
    MSG_PARSER_NUM
};

class MsgParser {
public:
    MsgParser() : command_m(TCP_CMD_NUM) {}
    ~MsgParser() {}

    ParserStatus             ParseMsg(const char* msg, const size_t msg_length);
    MsgCmd                   GetMsgCommand() const { return command_m; }
    std::string              GetMsgCommandStr() const;
    std::vector<std::string> GetMsgVector() const { return parsed_masage_m; }
    size_t                   GetMsgVectorSize() const { return parsed_masage_m.size(); }
    std::string              GetMsgStrAt(size_t index) const;
    std::string              GetMsgAtributeNo(size_t index) const;
    void                     Clear() { parsed_masage_m.clear(); }

private:
    MsgCmd VerifyAndGetMsgCommand(void);
    ParserStatus GetConfigStream(  const char* msg
                                 , const size_t msg_length
                                 , const size_t offset
                                 , size_t& read_cfg_size);

    std::vector<std::string> parsed_masage_m;
    MsgCmd command_m;
};

} // namespace comm

#endif // COMMUNICATION_MSG_PARSER_H_
