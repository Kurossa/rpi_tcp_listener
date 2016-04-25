/*
 * app_time.hpp
 *
 *  Created on: Apr 25, 2016
 *      Author: mariusz
 */

#ifndef APP_TIME_APP_TIME_HPP_
#define APP_TIME_APP_TIME_HPP_

#include <string>

using namespace std;

typedef enum eTimeStatus {
    TIME_NOK,
    TIME_SET_OK,
    TIME_GET_OK,
    TIME_NUM
} eTimeStatus_t;

/* Date/Time format: HH:MM:SS_DD_MM_YYYY i.e. 00:00:00_01-01-2016 */
class cTime {
public:
    cTime(void) {}
    eTimeStatus_t getTime(char* timeString);
    eTimeStatus_t setTime(const char* timeString);

private:
};


#endif /* APP_TIME_APP_TIME_HPP_ */
