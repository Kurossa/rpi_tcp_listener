#include "zip.h"
#include <utilities/logger.h>
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

using namespace utils;

eZipStaus_t ZipUncompress(const char* source_name, const char* destination_name) {
    eZipStaus_t status = ZIP_UNCOMP_FAILED;
    char buf_normal[1024];
    char buf_reversed[1024];
    char buf_odd_char[2];
    size_t size_read, size_read_total, size_total;
    bool isOdd = false;

    if (source_name == destination_name || strcmp(source_name, destination_name) == 0) {
        logPrintf(LogLevel::ERROR, "Uncompress failed: same source and destination.\n");
        status = ZIP_SAME_SRC_DST;
    } else {
        // check if file exists
        FILE* source = fopen(source_name, "rb");
        if (source) {
            FILE* dest = fopen(destination_name, "wb");

            size_total = 0;
            while ((size_read = fread(buf_normal, 1, FILE_BLOCK, source))) {
                size_total += size_read;
            }
            fseek(source, 0, SEEK_SET);

            isOdd = (size_total % 2) ? true : false;

            size_read_total = 0;
            size_t i = 0;
            while ((size_read = fread(buf_normal, 1, FILE_BLOCK, source))) {
                size_read_total += size_read;
                if (isOdd && size_read_total == size_total && (size_read % 2) ) {
                    buf_odd_char[0] = buf_normal[size_read -1];
                    buf_odd_char[1] = '\0';
                    --size_read;
                }
                i = 0;
                for (i = 0; i < (size_read >> 1); ++i) {
                    buf_reversed[2 * i] = buf_normal[(2 * i) + 1];
                    buf_reversed[(2 * i) + 1] = buf_normal[2 * i];
                }
                fwrite(buf_reversed, 1, size_read, dest);
            }

            if (isOdd) {
                fwrite(buf_odd_char, 1, 1, dest);
            }

            fclose(source);
            fclose(dest);
            status = ZIP_OK;
        } else {
            status = ZIP_FILE_NOT_EXISTS;
            logPrintf(LogLevel::ERROR, "Uncompress failed: Source file does not exists.\n");
        }
    }

    return status;
}

eZipStaus_t ZipCompress(const char* source_name, const char* destination_name) {
    eZipStaus_t status = ZIP_UNCOMP_FAILED;
    char buf_normal[1024];
    char buf_reversed[1024];
    char buf_odd_char[2];
    size_t size_read, size_read_total, size_total;
    bool isOdd = false;

    if (source_name == destination_name || strcmp(source_name, destination_name) == 0) {
        status = ZIP_SAME_SRC_DST;
        logPrintf(LogLevel::ERROR, "Compress failed: same source and destination.\n");
    } else {
        FILE* source = fopen(source_name, "rb");
        if (source) {
            FILE* dest = fopen(destination_name, "wb");

            size_total = 0;
            while ((size_read = fread(buf_normal, 1, FILE_BLOCK, source))) {
                size_total += size_read;
            }
            fseek(source, 0, SEEK_SET);

            isOdd = (size_total % 2) ? true : false;

            size_read_total = 0;
            size_t i = 0;
            while ((size_read = fread(buf_normal, 1, FILE_BLOCK, source))) {
                size_read_total += size_read;
                if (isOdd && size_read_total == size_total && (size_read % 2) ) {
                    buf_odd_char[0] = buf_normal[size_read -1];
                    buf_odd_char[1] = '\0';
                    --size_read;
                }
                i = 0;
                for (i = 0; i < (size_read >> 1); ++i) {
                    buf_reversed[2 * i] = buf_normal[(2 * i) + 1];
                    buf_reversed[(2 * i) + 1] = buf_normal[2 * i];
                }
                fwrite(buf_reversed, 1, size_read, dest);
            }

            if (isOdd) {
                fwrite(buf_odd_char, 1, 1, dest);
            }

            fclose(source);
            fclose(dest);
            status = ZIP_OK;
        } else {
            status = ZIP_FILE_NOT_EXISTS;
            logPrintf(LogLevel::ERROR, "Compress failed: Source file does not exists.\n");
        }
    }
    return status;
}
