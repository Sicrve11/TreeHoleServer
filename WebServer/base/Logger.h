/* @Author shigw    @Email sicrve@gmail.com */
// 日志类
#pragma once
#include <pthread.h>
#include <string.h>
#include <string>
#include <stdio.h>
#include "LogStream.h"

class AsyncLogging;

class Logger {
public:
    Logger(const char *fileName, int line);
    ~Logger();

    LogStream& getStream() { return impl_.outStream_; }

    static void setLogFileName(std::string fileName) { logFileName_ = fileName; }
    static std::string getLogFileName() { return logFileName_; }

private:
    static std::string logFileName_;    // 日志文件名称

    class Impl {
    public:
        Impl(const char* fileName, int line);

        // 产生日志的源文件及其行数
        std::string filename_;
        int line_;

        LogStream outStream_;       // 统一输出的流
        void formatTime();      // 打印时间
    };

    Impl impl_;
};

#define LOG Logger(__FILE__, __LINE__).getStream()