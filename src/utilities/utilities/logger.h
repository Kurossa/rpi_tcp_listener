/*
 * logger.h
 *
 *  Created on: Feb 2, 2016
 *      Author: Kurossa
 */

#ifndef UTILITIES_LOGGER_H_
#define UTILITIES_LOGGER_H_

#include <cstdio>
#include <cstdint>
#include <mutex>

//TODO: create real logger class
//TODO: add logger tests
namespace utils
{

enum LogLevel {
    INFO,
    WARNING,
    ERROR,
    SCREEN,
    MAX_LEVEL
};

class Logger
{
public:
    Logger(bool log_to_file = false, bool verbose_mode = false);
    ~Logger();

    void Log(int level, const char* format, ...);

private:
    void CreateLogFile();
    void CloseLogFile();
    bool RenameAndStoreLogFile();
    bool RemoveLogs();

    std::mutex log_mutex_m;
    bool verbose_mode_m;
    bool write_to_file_m;

    FILE * file_m = nullptr;
    //TODO: convert this to string
    char dir_name_m[1024];
    char log_dir_name_m[1024];
    char log_file_name_m[1024];
    unsigned int file_count_m;
    unsigned int line_count_m;

};

} //namespace utils

#endif // UTILITIES_LOGGER_H_
