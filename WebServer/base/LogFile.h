/* @Author shigw    @Email sicrve@gmail.com */
// 是对AppendFile的进一步封装，设置多次输入返回，以及设置锁, 用于日志写回
#pragma once
#include <string>
#include <memory>
#include "noncopyable.h"
#include "MutexLock.h"
#include "AppendFile.h"


class LogFile : noncopyable {
public:
    LogFile(const std::string& basename, int flushEveryN = 1024);
    ~LogFile();

    void append(const char* logline, int len);
    void flush();

private:
    int count_;
    std::unique_ptr<MutexLock> mutex_;
    std::unique_ptr<AppendFile> file_;

    const std::string basename_;
    const int flushEveryN_;     // 可写入次数

    void append_unlocked(const char* logline, int len);
};