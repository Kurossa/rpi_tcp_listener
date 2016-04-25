#include <app_logger.hpp>
#include <app_zip.hpp>
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

eZipStaus_t zip_uncompress(const char* source_name, const char* destination_name) {
    eZipStaus_t status = ZIP_UNCOMP_FAILED;
    char buf_normal[1024];
    char buf_reversed[1024];
    size_t size_read, size_total;
    bool isOdd = false;

    if (source_name == destination_name || strcmp(source_name, destination_name) == 0) {
        logPrintf(ERROR_LOG, "Uncompress failed: same source and destination.\n");
        status = ZIP_SAME_SRC_DST;
    } else {
        // check if file exists
        FILE* source = fopen(source_name, "r");
        if (source) {
            FILE* dest = fopen(destination_name, "wb");

            // clean and more secure
            // feof(FILE* stream) returns non-zero if the end of file indicator for stream is set

            size_total = 0;
            while ((size_read = fread(buf_normal, 1, FILE_BLOCK, source))) {
                size_total += size_read;
            }
            fseek(source, 0, SEEK_SET);

            if (size_total % 2) {
                --size_total;
                isOdd = true;
            } else {
                isOdd = false;
            }

            uint32_t i = 0;
            while ((size_read = fread(buf_normal, 1, FILE_BLOCK, source))) {
                for (i = 0; i < (size_read >> 1); ++i) {
                    buf_reversed[2 * i] = buf_normal[(2 * i) + 1];
                    buf_reversed[(2 * i) + 1] = buf_normal[2 * i];
                }
                fwrite(buf_reversed, 1, size_read, dest);
            }

            if (isOdd) {
                size_read = fread(buf_normal, 1, 1, source);
                fwrite(buf_normal, 1, size_read, dest);
            }

            fclose(source);
            fclose(dest);
            status = ZIP_OK;
        } else {
            status = ZIP_FILE_NOT_EXISTS;
            logPrintf(ERROR_LOG, "Uncompress failed: Source file does not exists.\n");
        }
    }

    return status;
}

eZipStaus_t zip_compress(const char* source_name, const char* destination_name) {
    eZipStaus_t status = ZIP_UNCOMP_FAILED;
    char buf_normal[1024];
    char buf_reversed[1024];
    size_t size_read, size_total;
    bool isOdd = false;

    if (source_name == destination_name || strcmp(source_name, destination_name) == 0) {
        status = ZIP_SAME_SRC_DST;
        logPrintf(ERROR_LOG, "Compress failed: same source and destination.\n");
    } else {
#warning TODO: add checking null pointers and same destination as source
        FILE* source = fopen(source_name, "r");
        if (source) {
            FILE* dest = fopen(destination_name, "wb");

            size_total = 0;
            while ((size_read = fread(buf_normal, 1, BUFSIZ, source))) {
                size_total += size_read;
            }
            fseek(source, 0, SEEK_SET);

            isOdd = false;
            if (size_total % 2) {
                --size_total;
                isOdd = true;
            } else {
                isOdd = false;
            }

            uint32_t i = 0;
            while ((size_read = fread(buf_normal, 1, FILE_BLOCK, source))) {
                for (i = 0; i < (size_read >> 1); ++i) {
                    buf_reversed[2 * i] = buf_normal[(2 * i) + 1];
                    buf_reversed[(2 * i) + 1] = buf_normal[2 * i];
                }
                fwrite(buf_reversed, 1, size_read, dest);
            }

            if (isOdd) {
                size_read = fread(buf_normal, 1, 1, source);
                fwrite(buf_normal, 1, size_read, dest);
            }

            fclose(source);
            fclose(dest);
            status = ZIP_OK;
        } else {
            status = ZIP_FILE_NOT_EXISTS;
            logPrintf(ERROR_LOG, "Compress failed: Source file does not exists.\n");
        }
    }
    return status;
}
