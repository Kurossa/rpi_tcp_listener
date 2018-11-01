/*
 * app_logger.cpp
 *
 *  Created on: Feb 2, 2016
 *      Author: Kurossa
 */

#include "logger.h"
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <cassert>
#include <dirent.h>
#include <iostream>
#include <set>
#include <sys/stat.h>
#include <sys/time.h>

using namespace utils;

namespace
{

// Define colors in terminal;
const char KNRM[] = "\x1B[0m";
const char KRED[] = "\x1B[31m";
const char KGRN[] = "\x1B[32m";
const char KYEL[] = "\x1B[33m";
const char KBLU[] = "\x1B[34m";
const char KMAG[] = "\x1B[35m";
const char KCYN[] = "\x1B[36m";
const char KWHT[] = "\x1B[37m";

// Log level string
const char log_level_str[LogLevel::MAX_LEVEL][16] = { "INF", "WAR", "ERR", "INF" };
const size_t name_len           = 3072;
const size_t msg_len            = 512;
const size_t max_log_file_num   = 99;
const size_t max_log_line_num   = 10000;
const size_t max_log_files      = 5;

bool GetCompactTimestamp(char *pStrBuf) {
    time_t now;
    struct tm *tm_now;

    if (NULL == pStrBuf) {
        return false;
    }

    time(&now);
    tm_now = localtime(&now);
    sprintf(pStrBuf, "%d%02d%02d%02d%02d%02d"
            , tm_now->tm_year + 1900
            , tm_now->tm_mon + 1
            , tm_now->tm_mday
            , tm_now->tm_hour
            , tm_now->tm_min
            , tm_now->tm_sec);

    return true;
}

/* Timestamp for log message (loose format) */
bool GetTimestamp(char *pStrBuf) {
    time_t now;
    struct tm *tm_now;
    struct timeval tm_timeval;
    long usec;

    if (NULL == pStrBuf) {
        return false;
    }

    time(&now);
    tm_now = localtime(&now);
    gettimeofday(&tm_timeval, NULL);
    usec = (tm_timeval.tv_usec)/1000;

    sprintf(pStrBuf, "%d-%02d-%02d %02d:%02d:%02d,%03ld"
            , tm_now->tm_year + 1900
            , tm_now->tm_mon + 1
            , tm_now->tm_mday
            , tm_now->tm_hour
            , tm_now->tm_min
            , tm_now->tm_sec
            , usec);

    return true;
}

}

/* Log rotate: add time stamps to log file name, so each log file will be different from the others */
bool Logger::RenameAndStoreLogFile()
{
    char date[msg_len];
    char next_file_name[name_len];
    char prev_file_name[name_len];

    memset(date, 0x00, msg_len);
    GetCompactTimestamp(date);

    memset(prev_file_name, 0x00, name_len);
    sprintf(prev_file_name, "%s%s.txt", log_dir_name_m, log_file_name_m);

    memset(next_file_name, 0x00, name_len);
    sprintf(next_file_name, "%s%s_%s_%02d.txt", log_dir_name_m, log_file_name_m, date, file_count_m);

    int result = rename(prev_file_name, next_file_name);
    if (0 != result)
    {
        printf("Error, failed to call rename %s to %s.\n", prev_file_name, next_file_name);
        return false;
    }

    /* Log file number threshold checking */
    if (file_count_m > max_log_file_num)
    {
        assert(0);
        printf("Error, log file number too high.\n");
    }

    return true;
}

bool Logger::RemoveLogs()
{
    unsigned char is_file = 0x8;
    std::set<std::string> log_file_names;
    DIR *dp;
    struct dirent *dirp;

    if((dp  = opendir(log_dir_name_m)) == NULL)
    {
        printf("[ERROR]LOG_Init: Failed to read folder %s\n", log_dir_name_m);
        return false;
    }

    while ((dirp = readdir(dp)) != NULL)
    {
        if (dirp->d_type == is_file)
        {
            char file_name[name_len] = {0};
            strcat(file_name, log_dir_name_m);
            strcat(file_name, dirp->d_name);
            log_file_names.insert(file_name);
        }
    }

    std::set<std::string>::iterator it = log_file_names.begin();
    int num_files_to_remove = log_file_names.size() - max_log_files + 1;
    for (int i = 0; i < num_files_to_remove; ++i, ++it)
    {
        printf("LOG FILE: %s\n",it->c_str());
        remove(it->c_str());
    }
    closedir(dp);

    return true;
}

Logger::Logger(bool log_to_file, bool verbose_mode) :
    verbose_mode_m(verbose_mode)
  , write_to_file_m(log_to_file)
  , file_m(nullptr)
  , file_count_m(0)
  , line_count_m(0)
{
    strcpy(dir_name_m, "log/");
    strcpy(log_file_name_m, "log");
    CreateLogFile();
}

Logger::~Logger()
{
    CloseLogFile();
    if (write_to_file_m)
    {
        RenameAndStoreLogFile();
    }
}

void Logger::CreateLogFile()
{
    if (write_to_file_m)
    {
        char full_filename[name_len];
        sprintf(log_dir_name_m, "%s/%s", getenv("HOME"), dir_name_m);

        struct stat sb;
        if (!stat(log_dir_name_m, &sb) == 0 || !S_ISDIR(sb.st_mode)) {
            mkdir(log_dir_name_m, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        }

        RemoveLogs();

        /* Check folder existing */
        memset(full_filename, 0x00, name_len);
        sprintf(full_filename, "%s%s.txt", log_dir_name_m, log_file_name_m);
        file_m = fopen(full_filename, "w+");
        if (NULL == file_m) {
            printf("[ERROR]LOG_Init: Failed to open file %s.\n", full_filename);
            write_to_file_m = false;
        }
        else
        {
            line_count_m = 0;
            ++file_count_m;
            file_count_m %= max_log_files;
        }
    }
}

void Logger::CloseLogFile()
{
    if (write_to_file_m)
    {
        if (NULL == file_m)
        {
            return;
        }

        fclose(file_m);
        file_m = NULL;
    }
}


void Logger::Log(int iLevel, const char* pStrFormat, ...)
{
    /* Parameters check */
    if(!write_to_file_m && !verbose_mode_m) {
        return;
    }

    if (iLevel >= LogLevel::MAX_LEVEL) {
        assert(0);
        return;
    }

    char acMsg[msg_len + 1];
    char acDate[msg_len + 1];
    char acBuf[msg_len + 1];

    va_list list;
    va_start(list, pStrFormat);

    /* Get message content */
    memset(acMsg, 0x00, msg_len + 1);
    vsnprintf(acMsg, msg_len + 1, pStrFormat, list);

    va_end(list);

    /* Get time stamp */
    memset(acDate, 0x00, msg_len + 1);
    GetTimestamp(acDate);

    /* Format log messages to write to file */
    memset(acBuf, 0x00, sizeof(acBuf));
    sprintf(acBuf, "%s [%s]: %s", acDate, log_level_str[iLevel], acMsg);


    if (verbose_mode_m) {
        std::lock_guard<std::mutex> lock(log_mutex_m);
        switch (iLevel)
        {
        case LogLevel::WARNING:
            /* Output the warning message directly to screen. */
            printf("%s%s%s\n", KYEL, acBuf, KNRM);
            break;
        case LogLevel::ERROR:
            /* Output the error message directly to screen. */
            printf("%s%s%s\n", KRED, acBuf, KNRM);
            break;
        case LogLevel::SCREEN:
            printf("%s%s%s\n", KGRN, acBuf, KNRM);
            break;
        default:
            break;
        }
    }

    if (write_to_file_m) {
        std::lock_guard<std::mutex> lock(log_mutex_m);

        /* Log file rotate, if the threshold (maximum line number) reached */
        if (line_count_m >= max_log_line_num) {
            CloseLogFile();
            RenameAndStoreLogFile();
            CreateLogFile();
        }

        fputs(acBuf, file_m);
        fflush(file_m);
        line_count_m++;
    }
}

