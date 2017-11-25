#include "gtest/gtest.h"

#include <time/time.h>
#include <utilities/logger.h>

#include <cstdlib>

namespace
{

// Date/Time format: HH:MM:SS_DD.MM.YYYY i.e. 00:00:00_01.01.2016
bool IsCorrectTimeFormat(const std::string& time)
{
    if (   time.length() == 19
        && std::isdigit(static_cast<unsigned char>(*time.substr(0, 1).c_str()))
        && std::isdigit(static_cast<unsigned char>(*time.substr(1, 1).c_str()))
        && ":" == time.substr(2, 1)
        && std::isdigit(static_cast<unsigned char>(*time.substr(3, 1).c_str()))
        && std::isdigit(static_cast<unsigned char>(*time.substr(4, 1).c_str()))
        && ":" == time.substr(5, 1)
        && std::isdigit(static_cast<unsigned char>(*time.substr(6, 1).c_str()))
        && std::isdigit(static_cast<unsigned char>(*time.substr(7, 1).c_str()))
        && "_" == time.substr(8, 1)
        && std::isdigit(static_cast<unsigned char>(*time.substr(9, 1).c_str()))
        && std::isdigit(static_cast<unsigned char>(*time.substr(10, 1).c_str()))
        && "." == time.substr(11, 1)
        && std::isdigit(static_cast<unsigned char>(*time.substr(12, 1).c_str()))
        && std::isdigit(static_cast<unsigned char>(*time.substr(13, 1).c_str()))
        && "." == time.substr(14, 1)
        && std::isdigit(static_cast<unsigned char>(*time.substr(15, 1).c_str()))
        && std::isdigit(static_cast<unsigned char>(*time.substr(16, 1).c_str()))
        && std::isdigit(static_cast<unsigned char>(*time.substr(17, 1).c_str()))
        && std::isdigit(static_cast<unsigned char>(*time.substr(18, 1).c_str())) ) {
        return true;
    }
    return false;
}

}

TEST (TimeTest, GetTimeTest) {
    std::string time;
    EXPECT_EQ(LinuxTime::TIME_GET_OK, LinuxTime::GetTime(time));
    // Date/Time format: HH:MM:SS_DD.MM.YYYY i.e. 00:00:00_01.01.2016
    EXPECT_TRUE(IsCorrectTimeFormat(time));
}

TEST (TimeTest, SetTimeWrongStringTest)
{
    // Date/Time format: HH:MM:SS_DD.MM.YYYY i.e. 00:00:00_01.01.2016
    EXPECT_EQ(LinuxTime::TIME_WRONG_FORMAT, LinuxTime::SetTime(std::string("11.22:33_11.22.3333")));
    EXPECT_EQ(LinuxTime::TIME_WRONG_FORMAT, LinuxTime::SetTime(std::string("11:22.33_11.22.3333")));
    EXPECT_EQ(LinuxTime::TIME_WRONG_FORMAT, LinuxTime::SetTime(std::string("11:22:33.11-22-3333")));
    EXPECT_EQ(LinuxTime::TIME_WRONG_FORMAT, LinuxTime::SetTime(std::string("1:22:33_11.22.3333")));
    EXPECT_EQ(LinuxTime::TIME_WRONG_FORMAT, LinuxTime::SetTime(std::string("111:22:33_11.22.3333")));
    EXPECT_EQ(LinuxTime::TIME_WRONG_FORMAT, LinuxTime::SetTime(std::string("11:2:33_11.22.3333")));
    EXPECT_EQ(LinuxTime::TIME_WRONG_FORMAT, LinuxTime::SetTime(std::string("11:222:33_11.22.3333")));
    EXPECT_EQ(LinuxTime::TIME_WRONG_FORMAT, LinuxTime::SetTime(std::string("11:22:3_11.22.3333")));
    EXPECT_EQ(LinuxTime::TIME_WRONG_FORMAT, LinuxTime::SetTime(std::string("11:22:333_11.22.3333")));
    EXPECT_EQ(LinuxTime::TIME_WRONG_FORMAT, LinuxTime::SetTime(std::string("11:22:33_1.22.3333")));
    EXPECT_EQ(LinuxTime::TIME_WRONG_FORMAT, LinuxTime::SetTime(std::string("11:22:33_111.22.3333")));
    EXPECT_EQ(LinuxTime::TIME_WRONG_FORMAT, LinuxTime::SetTime(std::string("11:22:33_11.2.3333")));
    EXPECT_EQ(LinuxTime::TIME_WRONG_FORMAT, LinuxTime::SetTime(std::string("11:22:33_11.222.3333")));
    EXPECT_EQ(LinuxTime::TIME_WRONG_FORMAT, LinuxTime::SetTime(std::string("11:22:33_11.22.333")));
    EXPECT_EQ(LinuxTime::TIME_WRONG_FORMAT, LinuxTime::SetTime(std::string("11:22:33_11.22.33333")));
    EXPECT_EQ(LinuxTime::TIME_WRONG_FORMAT, LinuxTime::SetTime(std::string("aa:22:33_11.22.3333")));
    EXPECT_EQ(LinuxTime::TIME_WRONG_FORMAT, LinuxTime::SetTime(std::string("11:aa:33_11.22.3333")));
    EXPECT_EQ(LinuxTime::TIME_WRONG_FORMAT, LinuxTime::SetTime(std::string("11:22:aa_11.22.3333")));
    EXPECT_EQ(LinuxTime::TIME_WRONG_FORMAT, LinuxTime::SetTime(std::string("11:22:33_aa.22.3333")));
    EXPECT_EQ(LinuxTime::TIME_WRONG_FORMAT, LinuxTime::SetTime(std::string("11:22:33_11.aa.3333")));
    EXPECT_EQ(LinuxTime::TIME_WRONG_FORMAT, LinuxTime::SetTime(std::string("11:22:33_11.22.aaaa")));
}

TEST (TimeTest, GetSetTimeTest)
{
    std::string time;
    EXPECT_EQ(LinuxTime::TIME_GET_OK, LinuxTime::GetTime(time));
    EXPECT_EQ(LinuxTime::TIME_SET_OK, LinuxTime::SetTime(time));
}

