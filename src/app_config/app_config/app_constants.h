/*
 * app_constants.h
 *
 *  Created on: Apr 27, 2016
 *      Author: mariusz
 */

#ifndef APP_CONSTS_HPP_
#define APP_CONSTS_HPP_

/* Location of configuration file */
const char config_file[] = "/etc/audio_app.cfg";
/* Location audio files in ram (/tmp/ is ramfs) */
const char ram[] = "/tmp/";
/* Location of encoded files */
const char encoded[] = "/root/";
/* Network interface; default: eth0 */
const char network_interface[] = "eth0";

typedef enum eErrorCode {
    ERROR_CODE_OK = 0,
    ERROR_CODE_NOK,
    ERROR_CODE_PLAY_MODE_ERROR,
    ERROR_CODE_NO_FILE_NUM,
    ERROR_CODE_WRONG_VOLUME,
    ERROR_CODE_GET_CFG_ERROR,
    ERROR_CODE_SET_CFG_ERROR,
    ERROR_CODE_GET_TIME_ERROR,
    ERROR_CODE_SET_TIME_ERROR,
    ERROR_CODE_NUM
}eErrorCode_t;


#endif /* APP_CONSTS_HPP_ */
