#include "zip.h"
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

using namespace utils;

ZipStatus utils::ZipUncompress(const char* source_name, const char* destination_name)
{
    char buf_normal[1024];
    char buf_reversed[1024];
    char buf_odd_char[2];
    size_t size_read, size_read_total, size_total;
    bool isOdd = false;

    if (source_name == destination_name || strcmp(source_name, destination_name) == 0)
    {
        return ZipStatus::SAME_SRC_DST;
    }

    FILE* source = fopen(source_name, "rb");
    if (nullptr == source)
    {
        return ZipStatus::FILE_NOT_EXISTS;
    }

    FILE* dest = fopen(destination_name, "wb");

    //TODO: add better way to determine file size without reading it
    //#include <fstream>
    //std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
    //size_t size = in.tellg();

    size_total = 0;
    while ((size_read = fread(buf_normal, 1, FILE_BLOCK, source)))
    {
        size_total += size_read;
    }

    fseek(source, 0, SEEK_SET);
    isOdd = (size_total % 2) ? true : false;

    size_read_total = 0;
    size_t i = 0;
    while ((size_read = fread(buf_normal, 1, FILE_BLOCK, source)))
    {
        size_read_total += size_read;
        if (isOdd && size_read_total == size_total && (size_read % 2) )
        {
            buf_odd_char[0] = buf_normal[size_read -1];
            buf_odd_char[1] = '\0';
            --size_read;
        }

        for (i = 0; i < (size_read >> 1); ++i)
        {
            buf_reversed[2 * i] = buf_normal[(2 * i) + 1];
            buf_reversed[(2 * i) + 1] = buf_normal[2 * i];
        }
        fwrite(buf_reversed, 1, size_read, dest);
    }

    if (isOdd)
    {
        fwrite(buf_odd_char, 1, 1, dest);
    }

    fclose(source);
    fclose(dest);
    return ZipStatus::OK;
}

ZipStatus utils::ZipCompress(const char* source_name, const char* destination_name) {
    ZipStatus status = ZipStatus::UNCOMP_FAILED;
    char buf_normal[FILE_BLOCK];
    char buf_reversed[FILE_BLOCK];
    char buf_odd_char[2];
    size_t size_read, size_read_total, size_total;
    bool isOdd = false;

    if (source_name == destination_name || strcmp(source_name, destination_name) == 0) {
        status = ZipStatus::SAME_SRC_DST;
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
            status = ZipStatus::OK;
        } else {
            status = ZipStatus::FILE_NOT_EXISTS;
        }
    }
    return status;
}
