/* @Author shigw    @Email sicrve@gmail.com */
// 直接写入文件的类，操作文件，内部有一个buffer，是为了确保写入文件的完整性
#pragma once
#include <cstddef>
#include <cstdio>
#include <string>
#include "noncopyable.h"

class AppendFile : noncopyable {
public:
    explicit AppendFile(std::string filename);
    ~AppendFile();

    void append(const char *logline, const size_t len);
    void flush();

private:
    size_t write(const char *logline, size_t len);

    FILE* fp_;
    char buffer_[64 * 1024];
};