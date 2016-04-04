/*
 * device_type.h
 *
 *  Created on: 1 gru 2015
 *      Author: mkrzysie
 */

#ifndef SOURCES_DEVICE_TYPE_H_
#define SOURCES_DEVICE_TYPE_H_

#define CRC_DEFAULT (0)
#define STATUS_DEFAULT (0)

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


#endif /* SOURCES_DEVICE_TYPE_H_ */
