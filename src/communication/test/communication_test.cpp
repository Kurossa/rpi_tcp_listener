#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <communication/communication.h>
#include <cstring>
#include <fstream>

using namespace comm;

namespace {

int LinesCount(std::string& s)
{
    return std::count(s.begin(), s.end(), '\n');
}

std::string ReadLine(std::string& s, int line_num) {
    std::istringstream f(s);
    std::string line;
    int line_count = 0;
    while (std::getline(f, line)) {
        ++line_count;
        if (line_count == line_num) {
            return line;
        }
    }
    return "";
}

}

class CommunicationFixture : public ::testing::Test
{
public:
    CommunicationFixture()
        : logger_m(false, true)
        , config_manager_m(logger_m) {}
    virtual void SetUp() {}
    virtual void TearDown() {}

protected:
    utils::Logger logger_m;
    config::ConfigManager config_manager_m;
};

TEST_F (CommunicationFixture, EmptyCommand) {
    // Empty Command:
    // ""

    comm::Communication communication(config_manager_m, logger_m);
    char command[1024] = "";
    char reply[1024] = {0};
    communication.handleCommand(command, reply);

    std::string reply_str(reply);
    int lines_count = LinesCount(reply_str);
    EXPECT_EQ(3, lines_count);
    EXPECT_STREQ("UNKNOWN_COMMAND" ,ReadLine(reply_str, 1).c_str());
    EXPECT_STREQ("END" ,ReadLine(reply_str, 3).c_str());
}

TEST_F (CommunicationFixture, WrongCommand) {
    // Wrong Command:
    // TCP_STATUS\n

    comm::Communication communication(config_manager_m, logger_m);
    char command[1024] = "TCP_STATUS\n";
    char reply[1024] = {0};
    communication.handleCommand(command, reply);

    std::string reply_str(reply);
    int lines_count = LinesCount(reply_str);
    EXPECT_EQ(3, lines_count);
    EXPECT_STREQ("UNKNOWN_COMMAND" ,ReadLine(reply_str, 1).c_str());
    EXPECT_STREQ("END" ,ReadLine(reply_str, 3).c_str());
}

TEST_F (CommunicationFixture, WrongCommandStructure) {
    // Wrong Command:
    // TCP_STATUS\n
    // WWRONG_ATTR\n
    // END\n

    comm::Communication communication(config_manager_m, logger_m);
    char command[1024] = "TCP_STATUS\nWWRONG_ATTR\nEND\n";
    char reply[1024] = {0};
    communication.handleCommand(command, reply);

    std::string reply_str(reply);
    int lines_count = LinesCount(reply_str);
    EXPECT_EQ(3, lines_count);
    EXPECT_STREQ("UNKNOWN_COMMAND" ,ReadLine(reply_str, 1).c_str());
    EXPECT_STREQ("END" ,ReadLine(reply_str, 3).c_str());
}

TEST_F (CommunicationFixture, NoNewLineAtEndOfCommand) {
    // Wrong Command:
    // TCP_STATUS\n
    // END

    comm::Communication communication(config_manager_m, logger_m);
    char command[1024] = "TCP_STATUS\nEND";
    char reply[1024] = {0};
    communication.handleCommand(command, reply);

    std::string reply_str(reply);
    int lines_count = LinesCount(reply_str);
    EXPECT_EQ(3, lines_count);
    EXPECT_STREQ("UNKNOWN_COMMAND" ,ReadLine(reply_str, 1).c_str());
    EXPECT_STREQ("END" ,ReadLine(reply_str, 3).c_str());
}

TEST_F (CommunicationFixture, MissingNewLineInCommand) {
    // Wrong Command:
    // TCP_STATUS
    // END\n

    comm::Communication communication(config_manager_m, logger_m);
    char command[1024] = "TCP_STATUSEND\n";
    char reply[1024] = {0};
    communication.handleCommand(command, reply);

    std::string reply_str(reply);
    int lines_count = LinesCount(reply_str);
    EXPECT_EQ(3, lines_count);
    EXPECT_STREQ("UNKNOWN_COMMAND" ,ReadLine(reply_str, 1).c_str());
    EXPECT_STREQ("END" ,ReadLine(reply_str, 3).c_str());
}

TEST_F (CommunicationFixture, StatusCommand) {
    // Command:
    // TCP_STATUS\n
    // END\n

    comm::Communication communication(config_manager_m, logger_m);
    char command[1024] = "TCP_STATUS\nEND\n";
    char reply[1024] = {0};
    communication.handleCommand(command, reply);

    std::string reply_str(reply);
    int lines_count = LinesCount(reply_str);
    EXPECT_EQ(5, lines_count);
    EXPECT_STREQ("COMMAND_7_RECEIVED" ,ReadLine(reply_str, 1).c_str());
    EXPECT_STREQ("ERROR_CODE:0" ,ReadLine(reply_str, 4).c_str());
    EXPECT_STREQ("END" ,ReadLine(reply_str, 5).c_str());
}

TEST_F (CommunicationFixture, PlayOncePlayOncePlayWrongFileCommand) {
    // Command:
    // TCP_PLAY\n
    // NR_PLIKU\n
    // ONCE\n
    // END\n

    config_manager_m.ParseConfigFile("audio_app_cfg.xml");
    std::ifstream  src("123.mp3", std::ios::binary);
    std::ofstream  dst("/tmp/123.mp3",   std::ios::binary);
    dst << src.rdbuf();

    comm::Communication communication(config_manager_m, logger_m);

    char reply[1024] = {0};
    communication.handleCommand("TCP_PLAY\n1\nONCE\nEND\n", reply);

    std::string reply_str(reply);
    int lines_count = LinesCount(reply_str);
    EXPECT_EQ(5, lines_count);
    EXPECT_STREQ("COMMAND_1_RECEIVED" ,ReadLine(reply_str, 1).c_str());
    EXPECT_STREQ("AUDIO_PLAY_ONCE" ,ReadLine(reply_str, 3).c_str());
    EXPECT_STREQ("ERROR_CODE:0" ,ReadLine(reply_str, 4).c_str());
    EXPECT_STREQ("END" ,ReadLine(reply_str, 5).c_str());

    communication.handleCommand("TCP_PLAY\n1\nONCE\nEND\n", reply);

    printf("%s", reply);
    reply_str = std::string(reply);
    lines_count = LinesCount(reply_str);
    EXPECT_EQ(5, lines_count);
    EXPECT_STREQ("COMMAND_1_RECEIVED" ,ReadLine(reply_str, 1).c_str());
    EXPECT_STREQ("AUDIO_PLAY_ONCE" ,ReadLine(reply_str, 3).c_str());
    EXPECT_STREQ("ERROR_CODE:0" ,ReadLine(reply_str, 4).c_str());
    EXPECT_STREQ("END" ,ReadLine(reply_str, 5).c_str());

    rename("/tmp/123.mp3", "/tmp/234.mp3");
    communication.handleCommand("TCP_PLAY\n1\nONCE\nEND\n", reply);

    printf("%s", reply);
    reply_str = std::string(reply);
    lines_count = LinesCount(reply_str);
    EXPECT_EQ(5, lines_count);
    EXPECT_STREQ("COMMAND_1_RECEIVED" ,ReadLine(reply_str, 1).c_str());
    EXPECT_STREQ("AUDIO_IDLE" ,ReadLine(reply_str, 3).c_str());
    EXPECT_STREQ("ERROR_CODE:3" ,ReadLine(reply_str, 4).c_str());
    EXPECT_STREQ("END" ,ReadLine(reply_str, 5).c_str());
    rename("/tmp/234.mp3", "/tmp/123.mp3");
}

TEST_F (CommunicationFixture, PlayInLoopCommand) {
    // Command:
    // TCP_PLAY\n
    // NR_PLIKU\n
    // ONCE\n
    // END\n

    config_manager_m.ParseConfigFile("audio_app_cfg.xml");
    std::ifstream  src("123.mp3", std::ios::binary);
    std::ofstream  dst("/tmp/123.mp3",   std::ios::binary);
    dst << src.rdbuf();

    comm::Communication communication(config_manager_m, logger_m);

    char reply[1024] = {0};
    communication.handleCommand("TCP_PLAY\n1\nIN_LOOP\nEND\n", reply);

    std::string reply_str(reply);
    int lines_count = LinesCount(reply_str);
    EXPECT_EQ(5, lines_count);
    EXPECT_STREQ("COMMAND_1_RECEIVED" ,ReadLine(reply_str, 1).c_str());
    EXPECT_STREQ("AUDIO_PLAY_IN_LOOP" ,ReadLine(reply_str, 3).c_str());
    EXPECT_STREQ("ERROR_CODE:0" ,ReadLine(reply_str, 4).c_str());
    EXPECT_STREQ("END" ,ReadLine(reply_str, 5).c_str());
}

TEST_F (CommunicationFixture, PlayWrongModeCommand) {
    // Command:
    // TCP_PLAY\n
    // NR_PLIKU\n
    // WRONG_MODE\n
    // END\n

    config_manager_m.ParseConfigFile("audio_app_cfg.xml");
    std::ifstream  src("123.mp3", std::ios::binary);
    std::ofstream  dst("/tmp/123.mp3",   std::ios::binary);
    dst << src.rdbuf();

    comm::Communication communication(config_manager_m, logger_m);

    char reply[1024] = {0};
    communication.handleCommand("TCP_PLAY\n1\nWRONG_MODE\nEND\n", reply);

    std::string reply_str(reply);
    int lines_count = LinesCount(reply_str);
    EXPECT_EQ(5, lines_count);
    EXPECT_STREQ("COMMAND_1_RECEIVED" ,ReadLine(reply_str, 1).c_str());
    EXPECT_STREQ("AUDIO_IDLE" ,ReadLine(reply_str, 3).c_str());
    EXPECT_STREQ("ERROR_CODE:2" ,ReadLine(reply_str, 4).c_str());
    EXPECT_STREQ("END" ,ReadLine(reply_str, 5).c_str());
}




TEST_F (CommunicationFixture, RebootCallback) {
    // Command:
    // TCP_RESET\n
    // END\n

    bool reboot_called = false;
    comm::Communication communication(config_manager_m, logger_m, [&reboot_called]() { reboot_called = true; });
    char command[1024] = "TCP_RESET\nEND\n";
    char reply[1024] = {0};
    communication.handleCommand(command, reply);

    std::string reply_str(reply);
    int lines_count = LinesCount(reply_str);
    EXPECT_EQ(4, lines_count);
    EXPECT_STREQ("COMMAND_6_RECEIVED" ,ReadLine(reply_str, 1).c_str());
    EXPECT_STREQ("ERROR_CODE:0" ,ReadLine(reply_str, 3).c_str());
    EXPECT_STREQ("END" ,ReadLine(reply_str, 4).c_str());
    EXPECT_TRUE(reboot_called);
}

//TODO: Add communication all handle tests

