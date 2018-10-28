/*
 * time.cpp
 *
 *  Created on: Oct 27, 2017
 *      Author: Kurossa
 */

#include <sys/time.h>

// Mock function that is only available using root privilages
int settimeofday (const struct timeval *__tv,
             const struct timezone *__tz)
{
    (void)__tv;
    (void)__tz;
    return 0;
}
