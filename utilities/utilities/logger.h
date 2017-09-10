/*
 * app_logger.hpp
 *
 *  Created on: Feb 2, 2016
 *      Author: mariusz
 */

#ifndef UTILITIES_APP_LOGGER_HPP_
#define UTILITIES_APP_LOGGER_HPP_

#include <stdio.h>

#define MAX_LOG_LINE_NUM    10000 /* Max line number of a single file */
#define MAX_LOG_FILE_NUM    99
#define MAX_LOG_FILES       5
#define LOG_BUF_LEN         256
#define LOG_MSG_LEN         128
#define LOG_FILENAME_LEN    64
#define LOG_DATE_LEN        32
#define LOG_MODULE_LEN      16
#define LOG_MAX_LEVEL_NAME  8
#define LOG_MAX_LEVEL       4

typedef enum elogLevel {
    INFO_LOG,
    WARNING_LOG,
    ERROR_LOG,
    SCREEN_LOG
} elogLevel_t;

extern FILE *pLogFile_g; /* Log file handler */
int logInit();
void logPrintf(int level, const char* format, ...);
void logClose();
void logSetVerboseMode(bool value);
void logSetLogToFile(bool value);

#endif /* UTILITIES_APP_LOGGER_HPP_ */
