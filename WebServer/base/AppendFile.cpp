/* @Author shigw    @Email sicrve@gmail.com */

#include "AppendFile.h"
#include <cstddef>
#include <cstdio>
#include <stdio.h>

// 将缓冲区和流输入关联起来，确保流输入的完整性
AppendFile::AppendFile(std::string filename) : fp_(fopen(filename.c_str(), "ae")) {
    setbuffer(fp_, buffer_, sizeof buffer_);
}

AppendFile::~AppendFile() { fclose(fp_); }

void AppendFile::append(const char *logline, const size_t len) {
    size_t n = this->write(logline, len);
    size_t remain = len - n;
    while(remain > 0) {
        size_t t = this->write(logline + n, remain);
        if(t == 0) {
            int err = ferror(fp_);
            if(err) fprintf(stderr, "AppendFile::append() failed !\n");
            break;
        }
        n += t;
        remain = len - n;
    }
}

void AppendFile::flush() { fflush(fp_); }

size_t AppendFile::write(const char *logline, size_t len) {
    return fwrite_unlocked(logline, 1, len, fp_);
}