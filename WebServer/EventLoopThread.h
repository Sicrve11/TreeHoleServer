/* @Author shigw    @Email sicrve@gmail.com */
// 对Eventloop进行的封装，让每个线程调用loop
#pragma once
#include <assert.h>
#include "EventLoop.h"
#include "base/noncopyable.h"
#include "base/MutexLock.h"
#include "base/Condition.h"
#include "base/Thread.h"

class EventLoopThread : noncopyable {
public:
    EventLoopThread();
    ~EventLoopThread();

    EventLoop* startLoop();

private:
    EventLoop* loop_;
    Thread thread_;
    void threadFunc();

    MutexLock mutex_;
    Condition cond_;
    bool is_exit_;
};




