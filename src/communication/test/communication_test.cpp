#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <communication/communication.h>

using namespace comm;
//TODO: add fixture
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

//TODO: Add communication handle tests

TEST_F (CommunicationFixture, RebootCallback) {
    bool reboot_called = false;
    comm::Communication communication(config_manager_m, logger_m, [&reboot_called]() { reboot_called = true; });
    char command[1024] = "TCP_RESET\nEND\n";
    char reply[1024] = {0};
    communication.handleCommand(command, reply);
    //communication.handleReset(test);
    printf("%s", reply);
    logger_m.Log(utils::LogLevel::WARNING, "%s", reply);
    logger_m.Log(utils::LogLevel::ERROR, "dupa %s", reboot_called ? "reboot called" : "reboot not called");
    EXPECT_TRUE(reboot_called);
}
