/*
 * app_logger.hpp
 *
 *  Created on: Feb 2, 2016
 *      Author: mariusz
 */

#ifndef UTILITIES_LOGGER_HPP_
#define UTILITIES_LOGGER_HPP_

#include <cstdio>
#include <cstdint>

namespace utils
{

const uint16_t MAX_LOG_LINE_NUM     = 10000; //Max line number of a single file
const uint16_t MAX_LOG_FILE_NUM     = 99;
const uint16_t MAX_LOG_FILES        = 5;
const uint16_t LOG_BUF_LEN          = 256;
const uint16_t LOG_MSG_LEN          = 128;
const uint16_t LOG_FILENAME_LEN     = 64;
const uint16_t LOG_DATE_LEN         = 32;
const uint16_t LOG_MODULE_LEN       = 16;
const uint16_t LOG_MAX_LEVEL_NAME   = 8;

enum LogLevel {
    INFO,
    WARNING,
    ERROR,
    SCREEN,
    MAX_LEVEL
};

//extern FILE *pLogFile_g; /* Log file handler */
int logInit();
void logPrintf(int level, const char* format, ...);
void logClose();
void logSetVerboseMode(bool value);
void logSetLogToFile(bool value);

} //namespace utils

#endif // UTILITIES_LOGGER_HPP_
