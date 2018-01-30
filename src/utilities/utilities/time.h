/*
 * app_time.hpp
 *
 *  Created on: Apr 25, 2016
 *      Author: mariusz
 */

#ifndef UTILITIES_TIME_H_
#define UTILITIES_TIME_H_

#include <string>

namespace utils
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

} //namespace LinuxTime

#endif // UTILITIES_TIME_H_
