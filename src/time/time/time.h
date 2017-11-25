/*
 * app_time.hpp
 *
 *  Created on: Apr 25, 2016
 *      Author: mariusz
 */

#ifndef APP_TIME_APP_TIME_HPP_
#define APP_TIME_APP_TIME_HPP_

#include <string>

namespace LinuxTime
{

typedef enum eTimeStatus {
    TIME_NOK,
    TIME_WRONG_FORMAT,
    TIME_SET_OK,
    TIME_GET_OK,
    TIME_NUM
} eTimeStatus_t;

// Date/Time format: HH:MM:SS_DD.MM.YYYY i.e. 00:00:00_01.01.2016

eTimeStatus_t GetTime(std::string& time_string);
eTimeStatus_t SetTime(const std::string& time_string);

}

#endif /* APP_TIME_APP_TIME_HPP_ */
