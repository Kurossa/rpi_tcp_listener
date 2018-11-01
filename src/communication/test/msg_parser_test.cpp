#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <communication/msg_parser.h>

using namespace comm;

TEST (MsgParserTest, EmptyCommand) {
    // Empty Command:
    // ""
    MsgParser msg_parser;
    std::string msg_cmd = "";
    ParserStatus status = msg_parser.ParseMsg(msg_cmd.c_str(), msg_cmd.length());

    EXPECT_EQ(ParserStatus::WRONG_MESSAGE, status);
}

TEST (MsgParserTest, WrongCommand) {
    // Wrong Command:
    // TCP_STATUS\n
    MsgParser msg_parser;
    std::string msg_cmd = "TCP_STATUS\n";
    ParserStatus status = msg_parser.ParseMsg(msg_cmd.c_str(), msg_cmd.length());

    EXPECT_EQ(ParserStatus::WRONG_MESSAGE, status);
}

TEST (MsgParserTest, WrongCommandStructure) {
    // Wrong Command:
    // TCP_STATUS\n
    // WWRONG_ATTR\n
    // END\n
    MsgParser msg_parser;
    std::string msg_cmd = "TCP_STATUS\nWWRONG_ATTR\nEND\n";
    ParserStatus status = msg_parser.ParseMsg(msg_cmd.c_str(), msg_cmd.length());

    EXPECT_EQ(ParserStatus::WRONG_MESSAGE, status);
}

TEST (MsgParserTest, NoNewLineAtEndOfCommand) {
    // Wrong Command:
    // TCP_STATUS\n
    // END
    MsgParser msg_parser;
    std::string msg_cmd = "TCP_STATUS\nEND";
    ParserStatus status = msg_parser.ParseMsg(msg_cmd.c_str(), msg_cmd.length());

    EXPECT_EQ(ParserStatus::WRONG_MESSAGE, status);
}

TEST (MsgParserTest, MissingNewLineInCommand) {
    // Wrong Command:
    // TCP_STATUS
    // END\n
    MsgParser msg_parser;
    std::string msg_cmd = "TCP_STATUSEND\n";
    ParserStatus status = msg_parser.ParseMsg(msg_cmd.c_str(), msg_cmd.length());

    EXPECT_EQ(ParserStatus::WRONG_MESSAGE, status);
}

TEST (MsgParserTest, StatusCommand) {
    // Command:
    // TCP_STATUS\n
    // END\n
    MsgParser msg_parser;
    std::string msg_cmd = "TCP_STATUS\nEND\n";
    ParserStatus status = msg_parser.ParseMsg(msg_cmd.c_str(), msg_cmd.length());

    EXPECT_EQ(MsgCmd::TCP_STATUS, msg_parser.GetMsgCommand());
    EXPECT_EQ(ParserStatus::OK, status);
    EXPECT_EQ(static_cast<size_t>(2), msg_parser.GetMsgVectorSize());
    EXPECT_STREQ("TCP_STATUS", msg_parser.GetMsgCommandStr().c_str());
}

TEST (MsgParserTest, Play1stFileOnceCommand) {
    // Command:
    // TCP_PLAY\n
    // NR_PLIKU\n
    // ONCE\n
    // END\n
    MsgParser msg_parser;
    std::string msg_cmd = "TCP_PLAY\n1\nONCE\nEND\n";
    msg_parser.ParseMsg(msg_cmd.c_str(), msg_cmd.length());

    EXPECT_EQ(MsgCmd::TCP_PLAY, msg_parser.GetMsgCommand());
    EXPECT_EQ(static_cast<size_t>(4), msg_parser.GetMsgVectorSize());
    EXPECT_STREQ("TCP_PLAY", msg_parser.GetMsgCommandStr().c_str());
    EXPECT_STREQ("1", msg_parser.GetMsgAtributeNo(0).c_str());
    EXPECT_STREQ("ONCE", msg_parser.GetMsgAtributeNo(1).c_str());
}

TEST (MsgParserTest, StopCommand) {
    // Command:
    // TCP_STOP\n
    // END\n
    MsgParser msg_parser;
    std::string msg_cmd = "TCP_STOP\nEND\n";
    msg_parser.ParseMsg(msg_cmd.c_str(), msg_cmd.length());

    EXPECT_EQ(MsgCmd::TCP_STOP, msg_parser.GetMsgCommand());
    EXPECT_EQ(static_cast<size_t>(2), msg_parser.GetMsgVectorSize());
    EXPECT_STREQ("TCP_STOP", msg_parser.GetMsgCommandStr().c_str());
}

TEST (MsgParserTest, SetTimeCommand) {
    // Command:
    //TCP_SET_TIME\n
    //GG:MM:SS_DD.MM.RRRR\n
    //END\n
    MsgParser msg_parser;
    std::string msg_cmd = "TCP_SET_TIME\n12:12:12_01.01.1971\nEND\n";
    msg_parser.ParseMsg(msg_cmd.c_str(), msg_cmd.length());

    EXPECT_EQ(MsgCmd::TCP_SET_TIME, msg_parser.GetMsgCommand());
    EXPECT_EQ(static_cast<size_t>(3), msg_parser.GetMsgVectorSize());
    EXPECT_STREQ("TCP_SET_TIME", msg_parser.GetMsgCommandStr().c_str());
    EXPECT_STREQ("12:12:12_01.01.1971", msg_parser.GetMsgAtributeNo(0).c_str());
}

TEST (MsgParserTest, SetVolumeCommand) {
    // Command:
    //TCP_SET_VOLUME\n
    //20\n
    //END\n
    MsgParser msg_parser;
    std::string msg_cmd = "TCP_SET_VOLUME\n20\nEND\n";
    msg_parser.ParseMsg(msg_cmd.c_str(), msg_cmd.length());

    EXPECT_EQ(MsgCmd::TCP_SET_VOLUME, msg_parser.GetMsgCommand());
    EXPECT_EQ(static_cast<size_t>(3), msg_parser.GetMsgVectorSize());
    EXPECT_STREQ("TCP_SET_VOLUME", msg_parser.GetMsgCommandStr().c_str());
    EXPECT_STREQ("20", msg_parser.GetMsgAtributeNo(0).c_str());
}

TEST (MsgParserTest, SetCfgCommand) {
    // Command:
    // TCP_SET_CFG\n
    // length_in_bytes\n
    // charactes_of_configuration\n
    // END\n
    MsgParser msg_parser;
    std::string config = "<?xml version=\"1.0\"?>\n"
                         "<audio_app_cfg>\n"
                         "\t<network>\n"
                         "\t\t<ip>127.0.0.1</ip>\n"
                         "\t\t<mask>255.255.255.0</mask>\n"
                         "\t\t<gateway>192.168.0.1</gateway>\n"
                         "\t\t<port>9999</port>\n"
                         "\t</network>\n"
                         "\t<sounds>\n"
                         "\t\t<sound>file1</sound>\n"
                         "\t\t<sound>file2</sound>\n"
                         "\t\t<sound>file3</sound>\n"
                         "\t\t<sound>file4</sound>\n"
                         "\t</sounds>\n"
                         "</audio_app_cfg>\n";
    std::string msg_cmd = "TCP_SET_CFG\n";
    std::ostringstream oss;
    oss << config.length() << std::endl;
    msg_cmd += oss.str();
    msg_cmd += config;
    msg_cmd += "END\n";

    msg_parser.ParseMsg(msg_cmd.c_str(), msg_cmd.length());

    EXPECT_EQ(MsgCmd::TCP_SET_CFG, msg_parser.GetMsgCommand());
    EXPECT_EQ(static_cast<size_t>(4), msg_parser.GetMsgVectorSize());
    EXPECT_STREQ("TCP_SET_CFG", msg_parser.GetMsgCommandStr().c_str());
    oss.str("");
    oss << config.length();
    EXPECT_STREQ(oss.str().c_str(), msg_parser.GetMsgAtributeNo(0).c_str());
    EXPECT_STREQ(config.c_str(), msg_parser.GetMsgAtributeNo(1).c_str());
}

TEST (MsgParserTest, GetCfgCommand) {
    // Command:
    // TCP_GET_CFG\n
    // END\n
    MsgParser msg_parser;
    std::string msg_cmd = "TCP_GET_CFG\nEND\n";
    msg_parser.ParseMsg(msg_cmd.c_str(), msg_cmd.length());

    EXPECT_EQ(MsgCmd::TCP_GET_CFG, msg_parser.GetMsgCommand());
    EXPECT_EQ(static_cast<size_t>(2), msg_parser.GetMsgVectorSize());
    EXPECT_STREQ("TCP_GET_CFG", msg_parser.GetMsgCommandStr().c_str());
}

TEST (MsgParserTest, ResetCommand) {
    // Command:
    // TCP_RESET\n
    // END\n
    MsgParser msg_parser;
    std::string msg_cmd = "TCP_RESET\nEND\n";
    msg_parser.ParseMsg(msg_cmd.c_str(), msg_cmd.length());

    EXPECT_EQ(MsgCmd::TCP_RESET, msg_parser.GetMsgCommand());
    EXPECT_EQ(static_cast<size_t>(2), msg_parser.GetMsgVectorSize());
    EXPECT_STREQ("TCP_RESET", msg_parser.GetMsgCommandStr().c_str());
}
