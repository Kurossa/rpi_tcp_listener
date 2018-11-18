#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <utilities/zip.h>

#include <communication/communication.h>

#include <cstring>
#include <fstream>
#include <sys/time.h>

// Mock function that is only available using root privilages
int settimeofday (const struct timeval *__tv, const struct timezone *__tz) __THROW
{
    (void)__tv;
    (void)__tz;
    return 0;
}

using namespace comm;

namespace {

const char compressed_file_name[] = "compressed_123.mp3";

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
        , config_manager_m(logger_m) {
        utils::ZipCompress("123.mp3", compressed_file_name);
        config_manager_m.ParseConfigFile("audio_app_cfg.xml");
    }

    ~CommunicationFixture() {
        std::string file_to_remove = "rm /tmp/";
        file_to_remove += compressed_file_name;
        system(file_to_remove.c_str());
    }
    virtual void SetUp() {}
    virtual void TearDown() {}

protected:
    utils::Logger logger_m;
    config::ConfigManager config_manager_m;
};

// Wrong command structure
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

// TCP_STATUS
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

//TCP_PLAY
TEST_F (CommunicationFixture, PlayOncePlayOncePlayWrongFileCommand) {
    // Command:
    // TCP_PLAY\n
    // NR_PLIKU\n
    // ONCE\n
    // END\n

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

    reply_str = std::string(reply);
    lines_count = LinesCount(reply_str);
    EXPECT_EQ(5, lines_count);
    EXPECT_STREQ("COMMAND_1_RECEIVED" ,ReadLine(reply_str, 1).c_str());
    EXPECT_STREQ("AUDIO_PLAY_ONCE" ,ReadLine(reply_str, 3).c_str());
    EXPECT_STREQ("ERROR_CODE:0" ,ReadLine(reply_str, 4).c_str());
    EXPECT_STREQ("END" ,ReadLine(reply_str, 5).c_str());

    // Rename file to try open file that no longer exists
    rename("/tmp/compressed_123.mp3", "/tmp/tmp.mp3");
    communication.handleCommand("TCP_PLAY\n1\nONCE\nEND\n", reply);

    reply_str = std::string(reply);
    lines_count = LinesCount(reply_str);
    EXPECT_EQ(5, lines_count);
    EXPECT_STREQ("COMMAND_1_RECEIVED" ,ReadLine(reply_str, 1).c_str());
    EXPECT_STREQ("AUDIO_IDLE" ,ReadLine(reply_str, 3).c_str());
    EXPECT_STREQ("ERROR_CODE:3" ,ReadLine(reply_str, 4).c_str());
    EXPECT_STREQ("END" ,ReadLine(reply_str, 5).c_str());
    rename("/tmp/tmp.mp3", "/tmp/compressed_123.mp3");
}

TEST_F (CommunicationFixture, PlayInLoopCommand) {
    // Command:
    // TCP_PLAY\n
    // NR_PLIKU\n
    // ONCE\n
    // END\n

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

// TCP_STOP
TEST_F (CommunicationFixture, StopPlayStopCommand) {
    // Command:
    // TCP_STOP\n
    // END\n

    comm::Communication communication(config_manager_m, logger_m);

    char reply[1024] = {0};
    communication.handleCommand("TCP_STOP\nEND\n", reply);

    std::string reply_str(reply);
    int lines_count = LinesCount(reply_str);
    EXPECT_EQ(5, lines_count);
    EXPECT_STREQ("COMMAND_2_RECEIVED" ,ReadLine(reply_str, 1).c_str());
    EXPECT_STREQ("AUDIO_IDLE" ,ReadLine(reply_str, 3).c_str());
    EXPECT_STREQ("ERROR_CODE:0" ,ReadLine(reply_str, 4).c_str());
    EXPECT_STREQ("END" ,ReadLine(reply_str, 5).c_str());

    communication.handleCommand("TCP_PLAY\n1\nONCE\nEND\n", reply);

    reply_str = std::string(reply);
    lines_count = LinesCount(reply_str);
    EXPECT_EQ(5, lines_count);
    EXPECT_STREQ("COMMAND_1_RECEIVED" ,ReadLine(reply_str, 1).c_str());
    EXPECT_STREQ("AUDIO_PLAY_ONCE" ,ReadLine(reply_str, 3).c_str());
    EXPECT_STREQ("ERROR_CODE:0" ,ReadLine(reply_str, 4).c_str());
    EXPECT_STREQ("END" ,ReadLine(reply_str, 5).c_str());

    communication.handleCommand("TCP_STOP\nEND\n", reply);

    reply_str = std::string(reply);
    lines_count = LinesCount(reply_str);
    EXPECT_EQ(5, lines_count);
    EXPECT_STREQ("COMMAND_2_RECEIVED" ,ReadLine(reply_str, 1).c_str());
    EXPECT_STREQ("AUDIO_IDLE" ,ReadLine(reply_str, 3).c_str());
    EXPECT_STREQ("ERROR_CODE:0" ,ReadLine(reply_str, 4).c_str());
    EXPECT_STREQ("END" ,ReadLine(reply_str, 5).c_str());
}

//TCP_SET_TIME
TEST_F (CommunicationFixture, SetTimeCommand) {
    // Command:
    //TCP_SET_TIME\n
    //GG:MM:SS_DD.MM.RRRR\n
    //END\n

    comm::Communication communication(config_manager_m, logger_m);

    char reply[1024] = {0};
    communication.handleCommand("TCP_SET_TIME\n12:12:12_01.01.1971\nEND\n", reply);

    std::string reply_str(reply);
    int lines_count = LinesCount(reply_str);
    EXPECT_EQ(4, lines_count);
    EXPECT_STREQ("COMMAND_0_RECEIVED" ,ReadLine(reply_str, 1).c_str());
    EXPECT_STREQ("ERROR_CODE:0" ,ReadLine(reply_str, 3).c_str());
    EXPECT_STREQ("END" ,ReadLine(reply_str, 4).c_str());
}

// TCP_SET_CFG
TEST_F (CommunicationFixture, SetCfgCommand) {
    // Command:
    // TCP_SET_CFG\n
    // length_in_bytes\n
    // charactes_of_configuration\n
    // END\n

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

    comm::Communication communication(config_manager_m, logger_m);

    char reply[1024] = {0};
    communication.handleCommand(msg_cmd.c_str(), reply);

    std::string reply_str(reply);
    int lines_count = LinesCount(reply_str);
    EXPECT_EQ(4, lines_count);
    EXPECT_STREQ("COMMAND_3_RECEIVED" ,ReadLine(reply_str, 1).c_str());
    // Error code was not tested as on real system file can exists ath this will succes on others will fail writtung
    EXPECT_STREQ("END" ,ReadLine(reply_str, 4).c_str());
}

// TCP_GET_CFG
TEST_F (CommunicationFixture, GetCfgCommand) {
    // Command:
    //TCP_GET_CFG\n
    //END\n

    comm::Communication communication(config_manager_m, logger_m);

    char reply[1024] = {0};
    communication.handleCommand("TCP_GET_CFG\nEND\n", reply);

    std::string reply_str(reply);
    int lines_count = LinesCount(reply_str);
    EXPECT_EQ(6, lines_count);
    EXPECT_STREQ("COMMAND_4_RECEIVED" ,ReadLine(reply_str, 1).c_str());
    // Do not test following lines as it vary depending on sytem it runs tests
    // 2nd line: Time
    // 3rd line: size
    // 4th line: config content
    EXPECT_STREQ("END" ,ReadLine(reply_str, 6).c_str());
}

// TCP_SET_VOLUME
TEST_F (CommunicationFixture, SetVolumeCommand) {
    // Command:
    //TCP_SET_VOLUME\n
    //20\n
    //END\n

    comm::Communication communication(config_manager_m, logger_m);

    char reply[1024] = {0};
    communication.handleCommand("TCP_SET_VOLUME\n20\nEND\n", reply);

    std::string reply_str(reply);
    int lines_count = LinesCount(reply_str);
    EXPECT_EQ(5, lines_count);
    EXPECT_STREQ("COMMAND_5_RECEIVED" ,ReadLine(reply_str, 1).c_str());
    EXPECT_STREQ("AUDIO_IDLE" ,ReadLine(reply_str, 3).c_str());
    EXPECT_STREQ("ERROR_CODE:0" ,ReadLine(reply_str, 4).c_str());
    EXPECT_STREQ("END" ,ReadLine(reply_str, 5).c_str());
}

// TCP_RESET
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

