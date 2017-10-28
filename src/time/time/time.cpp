/*
 * app_time.cpp1900
 *
 *  Created on: Apr 25, 2016
 *      Author: mariusz
 */
#include "time.h"
#include <utilities/logger.h>
#include <stdlib.h>
#include <sys/time.h>

eTimeStatus_t cTime::getTime(char* timeString) {
    eTimeStatus_t status = TIME_NOK;
    time_t mytime = time(0);
    struct tm* tm_ptr = localtime(&mytime);

    if (tm_ptr) {
        sprintf(timeString,"%02d:%02d:%02d_%02d.%02d.%04d",tm_ptr->tm_hour, tm_ptr->tm_min, tm_ptr->tm_sec, tm_ptr->tm_mday, tm_ptr->tm_mon+1, 1900+tm_ptr->tm_year);
                           //012  345  67 8 90 1 23 4 5678
        status = TIME_GET_OK;
    }
    return status;
}
eTimeStatus_t cTime::setTime(const char* timeString) {
    eTimeStatus_t status = TIME_NOK;
    time_t mytime = time(0);
    struct tm* tm_ptr = localtime(&mytime);
    string date_str(timeString);

    if (tm_ptr) {
        tm_ptr->tm_hour = atoi(date_str.substr(0, 2).c_str());
        tm_ptr->tm_min = atoi(date_str.substr(3, 2).c_str());
        tm_ptr->tm_sec = atoi(date_str.substr(6, 2).c_str());
        tm_ptr->tm_mday = atoi(date_str.substr(9, 2).c_str());
        tm_ptr->tm_mon = atoi(date_str.substr(12, 2).c_str()) - 1;
        tm_ptr->tm_year = atoi(date_str.substr(15, 4).c_str())-1900;

        const struct timeval tv = { mktime(tm_ptr), 0 };
        int time_status = settimeofday(&tv, 0);
        logPrintf(SCREEN_LOG, "Set time: %02d:%02d:%02d_%02d.%02d.%04d, status = %d\n",tm_ptr->tm_hour, tm_ptr->tm_min, tm_ptr->tm_sec, tm_ptr->tm_mday, tm_ptr->tm_mon+1, tm_ptr->tm_year + 1900, time_status);

        status = TIME_SET_OK;
    }
    return status;
}

