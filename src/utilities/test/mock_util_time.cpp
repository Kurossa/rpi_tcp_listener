/*
 * mock_util_time.cpp
 *
 *  Created on: Nov 15, 2018
 *      Author: Kurossa
 */

#include <sys/time.h>

// Mock function that is only available using root privilages
int settimeofday (const struct timeval *__tv, const struct timezone *__tz) __THROW
{
    (void)__tv;
    (void)__tz;
    return 0;
}
