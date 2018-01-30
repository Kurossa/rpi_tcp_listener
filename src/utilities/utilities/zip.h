/*
 * zip.hpp
 *
 *  Created on: Apr 17, 2016
 *      Author: Kurossa
 */
#ifndef UTILITIES_ZIP_H_
#define UTILITIES_ZIP_H_

#define FILE_BLOCK (1024)
typedef enum eZipStatus {
    ZIP_OK,
    ZIP_SAME_SRC_DST,
    ZIP_UNCOMP_FAILED,
    ZIP_COMP_FAILED,
    ZIP_FILE_NOT_EXISTS,
    ZIP_NUM
}eZipStaus_t;

eZipStaus_t ZipUncompress(const char* source_name, const char* destination_name);
eZipStaus_t ZipCompress(const char* source_name, const char* destination_name);

#endif // UTILITIES_ZIP_H_
