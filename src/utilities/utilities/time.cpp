/*
 * time.cpp
 *
 *  Created on: Apr 25, 2016
 *      Author: Kurossa
 */

#include "time.h"
#include <stdlib.h>
#include <sys/time.h>
#include <cctype>

using namespace utils;

namespace
{

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

using namespace utils;

eTimeStatus_t utils::GetTime(std::string& time_string) {
    time_t mytime = time(0);
    struct tm* tm_ptr = localtime(&mytime);

    if (tm_ptr) {
        char time[32] = {0};
        sprintf(time,"%02d:%02d:%02d_%02d.%02d.%04d",tm_ptr->tm_hour, tm_ptr->tm_min, tm_ptr->tm_sec, tm_ptr->tm_mday, tm_ptr->tm_mon+1, 1900+tm_ptr->tm_year);
                      //012  345 67 8 90 1 23 4 5678
        time_string = std::string(time);
        return TIME_GET_OK;
    }
    return TIME_NOK;
}

eTimeStatus_t utils::SetTime(const std::string& time_string) {
    time_t mytime = time(0);
    struct tm* tm_ptr = localtime(&mytime);
    /* Date/Time format: HH:MM:SS_DD_MM_YYYY i.e. 00:00:00_01-01-2016 */
    //std::regex re_pattern("[0-9]{2}:[0-9]{2}:[0-9]{2}_[0-9]{2}\\.[0-9]{2}\\.[0-9]{4}");
    if (!IsCorrectTimeFormat(time_string)) {
        return TIME_WRONG_FORMAT;
    }

    if (tm_ptr) {
        tm_ptr->tm_hour = atoi(time_string.substr(0, 2).c_str());
        tm_ptr->tm_min = atoi(time_string.substr(3, 2).c_str());
        tm_ptr->tm_sec = atoi(time_string.substr(6, 2).c_str());
        tm_ptr->tm_mday = atoi(time_string.substr(9, 2).c_str());
        tm_ptr->tm_mon = atoi(time_string.substr(12, 2).c_str()) - 1;
        tm_ptr->tm_year = atoi(time_string.substr(15, 4).c_str())-1900;

        const struct timeval tv = { mktime(tm_ptr), 0 };
        int success = settimeofday(&tv, 0);
        if (0 == success )
        {
            return TIME_SET_OK;
        }
    }
    return TIME_NOK;
}

