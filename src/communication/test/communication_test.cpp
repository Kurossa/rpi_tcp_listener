#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <communication/communication.h>
#include <cstring>

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

