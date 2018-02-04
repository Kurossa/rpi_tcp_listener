#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <communication/msg_parser.h>

TEST (MsgParserTest, StatusCommand) {
    // Command:
    // TCP_STATUS\n
    // END\n
    MsgParser msg_parser;
    std::string status_cmd = "TCP_STATUS\nEND\n";
    msg_parser.ParseMsg(status_cmd.c_str(), status_cmd.length());
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
    std::string command = "TCP_SET_CFG\n";
    std::ostringstream oss;
    oss << config.length() << std::endl;
    command += oss.str();
    command += config;
    command += "END\n";

    msg_parser.ParseMsg(command.c_str(), command.length());

    EXPECT_EQ(4, static_cast<int>(msg_parser.GetMsgVectorSize()));
    EXPECT_STREQ(config.c_str(), msg_parser.GetMsgStrAt(2).c_str());
}
