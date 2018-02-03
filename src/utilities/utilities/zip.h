/*
 * zip.h
 *
 *  Created on: Apr 17, 2016
 *      Author: Kurossa
 */
#ifndef UTILITIES_ZIP_H_
#define UTILITIES_ZIP_H_

#define FILE_BLOCK (1024)

namespace utils
{

enum ZipStatus {
    OK,
    SAME_SRC_DST,
    UNCOMP_FAILED,
    COMP_FAILED,
    FILE_NOT_EXISTS,
    ZIP_NUM
};

ZipStatus ZipUncompress(const char* source_name, const char* destination_name);
ZipStatus ZipCompress(const char* source_name, const char* destination_name);

}
#endif // UTILITIES_ZIP_H_
