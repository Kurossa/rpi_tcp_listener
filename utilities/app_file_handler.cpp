#include <app_file_handler.hpp>
#include <app_logger.hpp>
#include <time.h>
#include <termios.h>

cFileHandler::cFileHandler() : fd_m(0), pathname_m(NULL) {
}

eFileStatus_t cFileHandler::open_fd(const char *pathname, int flags, mode_t mode) {
    eFileStatus_t status = FILE_STATUS_GENERAL_FAIL;
    pathname_m = pathname;
    fd_m = open(pathname, flags, mode);
    if (fd_m == -1) {
        logPrintf(ERROR_LOG, "cFileHandler: file %s", pathname);
        perror(NULL);
        status = FILE_STATUS_OPEN_ERROR;
    } else {
        status = FILE_STATUS_SUCCESS;
    }
    return status;
}

cFileHandler::~cFileHandler() {
    if (close(fd_m) == -1) {
        logPrintf(ERROR_LOG, "fd close() error\n");
    }
}

eFileStatus_t cFileHandler::read_fd(uint8_t* data, uint16_t size, int& size_read, struct timeval* tv) {
    eFileStatus_t status = FILE_STATUS_GENERAL_FAIL;
    fd_set rfds;
    FD_ZERO(&rfds);
    FD_SET(fd_m, &rfds);

    int retval = select(fd_m + 1, &rfds, NULL, NULL, tv);
    if (retval < 0) {
        perror("cFileHandler::read_fd: select() error");
        status = FILE_STATUS_SELECT_FAILED;
    } else if (retval == 0) {
        status = FILE_STATUS_SELECT_TIMEOUT;
    } else {
        if (FD_ISSET(fd_m, &rfds)) {
            size_read = read(fd_m, data, size);
            if (size_read >= 0) {
                status = FILE_STATUS_SUCCESS;
            } else {
                status = FILE_STATUS_READ_FAILED;
            }
        } else {
            printf("readFd: fd not set !\n");
            status = FILE_STATUS_FD_NOT_SET;
        }
    }
    return status;
}

eFileStatus_t cFileHandler::write_fd(uint8_t* data, uint16_t size, struct timeval* tv) {
    eFileStatus_t status = FILE_STATUS_GENERAL_FAIL;
    if (size) {
        fd_set rfds;
        FD_ZERO(&rfds);
        FD_SET(fd_m, &rfds);

        int retval = select(fd_m + 1, NULL, &rfds, NULL, tv);
        if (retval < 0) {
            perror(" cFileHandler::write_fd: select() error");
            status = FILE_STATUS_SELECT_FAILED;
        } else if (retval == 0) {
            // printf("write_fd: file %s,  timeout expired, data not written to fd\n", pathname_m);
            status = FILE_STATUS_SELECT_TIMEOUT;
        } else {
            if (FD_ISSET(fd_m, &rfds)) {
                int ret = write(fd_m, data, size);
                tcdrain(fd_m);
                if (ret >= 0) {
                    status = FILE_STATUS_SUCCESS;
                } else {
                    perror("write_fd: write() error");
                    status = FILE_STATUS_READ_FAILED;
                }
            } else {
                printf("write_fd: fd not set !\n");
                status = FILE_STATUS_FD_NOT_SET;
            }
        }
    }
    return status;
}
