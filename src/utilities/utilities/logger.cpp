/*
 * app_logger.cpp
 *
 *  Created on: Feb 2, 2016
 *      Author: mariusz
 */
#include "logger.h"
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <math.h>
#include <pthread.h>
#include <sys/types.h>
#include <dirent.h>
#include <iostream>
#include <set>
#include <sys/stat.h>
#include <sys/time.h>
#include <stdlib.h>

using namespace utils;

static FILE * pLogFile_s = NULL;
static char pDirName_s[] = "log/";
static char pLogDirName_s[128] = "";
static char pLogFileName_s[] = "log";
static unsigned int LogFileNum_s = 0;
static unsigned int LogLineCount_s = 0;

//const char KNRM[] =  "\x1B[0m";
#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

static pthread_mutex_t logMutex_s;
static bool log_verbose_mode_s = false;
static bool log_write_to_file_s = false;

/* Log level */
static char s_acLevel[LogLevel::MAX_LEVEL][LOG_MAX_LEVEL_NAME] = { "INF", "WAR", "ERR", "INF" };

static void logGetCompactTimestamp(char *pStrBuf) {
    time_t now;
    struct tm *tm_now;

    if (NULL == pStrBuf) {
        return;
    }

    time(&now);
    tm_now = localtime(&now);
    sprintf(pStrBuf, "%d%02d%02d%02d%02d%02d", tm_now->tm_year + 1900, tm_now->tm_mon + 1, tm_now->tm_mday, tm_now->tm_hour, tm_now->tm_min,
            tm_now->tm_sec);
    return;
}

/* Timestamp for log message (loose format) */
static void logGetTimestamp(char *pStrBuf) {
    time_t now;
    struct tm *tm_now;
    struct timeval tm_timeval;
    long usec;

    if (NULL == pStrBuf) {
        return;
    }

    time(&now);
    tm_now = localtime(&now);
    gettimeofday(&tm_timeval, NULL);
    usec = (tm_timeval.tv_usec)/1000;

    sprintf(pStrBuf, "%d-%02d-%02d %02d:%02d:%02d,%03ld", tm_now->tm_year + 1900, tm_now->tm_mon + 1, tm_now->tm_mday, tm_now->tm_hour,
            tm_now->tm_min, tm_now->tm_sec, usec);

    return;
}

/* Log rotate: add time stamps to log file name, so each log file will be different from the others */
static int logChange() {
    int iRet = -1;

    char acDate[LOG_DATE_LEN + 1];
    char acFileNameNew[LOG_FILENAME_LEN + 1];
    char acFileNameOld[LOG_FILENAME_LEN + 1];

    memset(acDate, 0x00, LOG_DATE_LEN + 1);
    logGetCompactTimestamp(acDate);

    memset(acFileNameOld, 0x00, LOG_FILENAME_LEN + 1);
    sprintf(acFileNameOld, "%s%s.txt", pLogDirName_s, pLogFileName_s);

    memset(acFileNameNew, 0x00, LOG_FILENAME_LEN + 1);
    sprintf(acFileNameNew, "%s%s_%s_%02d.txt", pLogDirName_s, pLogFileName_s, acDate, LogFileNum_s);

    iRet = rename(acFileNameOld, acFileNameNew);
    if (0 != iRet) {
        printf("Error, failed to call rename %s to %s.\n", acFileNameOld, acFileNameNew);
        return -1;
    }

    /* Log file number threshold checking */
    if (LogFileNum_s > MAX_LOG_FILE_NUM) {
        printf("Error, log file number too high.\n");
    }

    return 0;
}

static void logRemoveLogs(void) {
    unsigned char isFile = 0x8;
    std::set<std::string> logFileNames;
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(pLogDirName_s)) == NULL) {
        printf("[ERROR]LOG_Init: Failed to read folder %s\n", pLogDirName_s);
    }

    while ((dirp = readdir(dp)) != NULL) {
        if (dirp->d_type == isFile) {
            char fileName[128] = {0};
            strcat(fileName,pLogDirName_s);
            strcat(fileName,dirp->d_name);
            logFileNames.insert(fileName);
        }
    }

    std::set<std::string>::iterator it = logFileNames.begin();
    for (int i = 0; i < ((int)logFileNames.size() - MAX_LOG_FILES + 1); ++i, ++it) {
        printf("LOG FILE: %s\n",it->c_str());
        remove(it->c_str());
    }

    closedir(dp);
}

void utils::logSetVerboseMode(bool value) {
    log_verbose_mode_s = value;
}

void utils::logSetLogToFile(bool value) {
    log_write_to_file_s = value;
}

int utils::logInit() {
    //log_verbose_mode_s
    //log_write_to_file_s
    if (log_write_to_file_s) {
        char acFileFullName[LOG_FILENAME_LEN + 1];
        int iRet = -1;

        sprintf(pLogDirName_s, "%s/%s", getenv("HOME"), pDirName_s);

        struct stat sb;
        if (!stat(pLogDirName_s, &sb) == 0 || !S_ISDIR(sb.st_mode)) {
            mkdir(pLogDirName_s, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        }

        logRemoveLogs();

        /* Check folder existing */
        memset(acFileFullName, 0x00, LOG_FILENAME_LEN + 1);
        sprintf(acFileFullName, "%s%s.txt", pLogDirName_s, pLogFileName_s);
        pLogFile_s = fopen(acFileFullName, "w+");
        if (NULL == pLogFile_s) {
            printf("[ERROR]LOG_Init: Failed to open file %s.\n", acFileFullName);
            return -1;
        }

        /* Mutex lock initialize */
        iRet = pthread_mutex_init(&logMutex_s, NULL);
        if (0 != iRet) {
            printf("[ERROR]LOG_Init: Failed to call pthread_mutex_init()\n");
            return -1;
        }

        LogLineCount_s = 0;
        ++LogFileNum_s;
        LogFileNum_s %= MAX_LOG_FILE_NUM;
    }

    return 0;
}

void utils::logPrintf(int iLevel, const char* pStrFormat, ...) {
    if(!log_write_to_file_s && !log_verbose_mode_s) {
        return;
    }

    char acMsg[LOG_MSG_LEN + 1];
    char acDate[LOG_DATE_LEN + 1];
    char acBuf[LOG_BUF_LEN + 1];

    va_list list;
    va_start(list, pStrFormat);

    /* Parameters check */
    if (iLevel >= LogLevel::MAX_LEVEL) {
        assert(0);
    }

    /* Get message content */
    memset(acMsg, 0x00, LOG_MSG_LEN + 1);
    vsnprintf(acMsg, LOG_BUF_LEN + 1, pStrFormat, list);

    /* Get time stamp */
    memset(acDate, 0x00, LOG_DATE_LEN + 1);
    logGetTimestamp(acDate);

    /* Format log messages to write to file */
    memset(acBuf, 0x00, sizeof(acBuf));
    sprintf(acBuf, "%s [%s]: %s", acDate, s_acLevel[iLevel], acMsg);


    if (log_verbose_mode_s) {
        /* Output the error message directly to screen. */
        if (iLevel == LogLevel::ERROR) {
            printf("%s%s%s\r", KRED,acBuf,KNRM);
        } else if (iLevel == LogLevel::SCREEN) {
            printf("%s%s%s\r", KGRN,acBuf,KNRM);
        }
    }
    if (log_write_to_file_s) {
        pthread_mutex_lock(&logMutex_s);

        /* Log file rotate, if the threshold (maximum line number) reached */
        if (LogLineCount_s >= MAX_LOG_LINE_NUM) {
            fclose(pLogFile_s);
            pLogFile_s = NULL;

            logChange();
            logInit();
        }

        fputs(acBuf, pLogFile_s);
        fflush(pLogFile_s);
        LogLineCount_s++;

        pthread_mutex_unlock(&logMutex_s);
    }

    va_end(list);
    return;
}

void utils::logClose() {
    if (log_write_to_file_s) {
        if (NULL == pLogFile_s) {
            return;
        }

        fclose(pLogFile_s);
        pLogFile_s = NULL;

        logChange();

        pthread_mutex_destroy(&logMutex_s);
    }
    return;
}

