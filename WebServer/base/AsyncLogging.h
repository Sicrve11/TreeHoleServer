/* @Author shigw    @Email sicrve@gmail.com */
// 异步将log写入文件的类，通过一个异步线程操作logfile对象，将logstream中的数据写入log
#pragma once
#include <string>
#include <vector>
#include <functional>
#include "CountDownLatch.h"
#include "LogStream.h"
#include "LogFile.h"
#include "MutexLock.h"
#include "Thread.h"
#include "noncopyable.h"


class AsyncLogging : noncopyable {
public:
    AsyncLogging(const std::string logFileName_, int flushInterval = 2);
    ~AsyncLogging();

    // 管理写回线程
    void start();
    void stop();

    void append(const char* logline, int len);  // 立即添加指定输出，例如日期等信息

private:
    typedef FixedBuffer<kLargeBuffer> Buffer;
    typedef std::shared_ptr<Buffer> BufferPtr;
    typedef std::vector<BufferPtr> BufferVector;
    
    // log文件
    std::string basename_;

    // 写回线程函数、线程、信号量等
    void threadFunc();
    Thread thread_;
    MutexLock mutex_;
    Condition cond_;
    CountDownLatch latch_;
    bool running_; 
    const int flushInterval_;   // 等待时间，轮询访问

    // buffer队列以及操作buffer的指针
    BufferVector outputBuffers_;      // 输入buffer
    BufferPtr curBufferPtr_;
    BufferPtr nextBufferPtr_;
};