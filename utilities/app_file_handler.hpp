#ifndef UTILITIES_FILE_HANDLER_FILE_HANDLER_HPP
#define UTILITIES_FILE_HANDLER_FILE_HANDLER_HPP

#include "stdint.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

typedef enum eFileStatus {
    FILE_STATUS_GENERAL_FAIL = 0,
    FILE_STATUS_SUCCESS,
    FILE_STATUS_OPEN_ERROR,
    FILE_STATUS_SELECT_FAILED,
    FILE_STATUS_SELECT_TIMEOUT,
    FILE_STATUS_READ_FAILED,
    FILE_STATUS_WRITE_FAILED,
    FILE_STATUS_FD_NOT_SET,
    FILE_STATUS_NOTHING_TO_WRITE,
    FILE_STATUS_NUM
} eFileStatus_t;

class cFileHandler {
    int fd_m;
    const char* pathname_m;
public:
    cFileHandler();
    ~cFileHandler();
    eFileStatus_t open_fd(const char *pathname, int flags, mode_t mode);
    eFileStatus_t read_fd(uint8_t* data, uint16_t size, int& size_read, struct timeval* tv);
    eFileStatus_t write_fd(uint8_t* data, uint16_t size, struct timeval* tv);
    int get_fd() {
        return fd_m;
    }
    inline void set_fd(int fd) {
        fd_m = fd;
    }
    inline void set_pathname(const char* pathname) {
        pathname_m = pathname;
    }
};

#endif /*UTILITIES_FILE_HANDLER_FILE_HANDLER_HPP*/
