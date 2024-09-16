/* @Author shigw    @Email sicrve@gmail.com */

#include "EventLoopThread.h"

// 子线程运行的函数，与主线程进行同步
void EventLoopThread::threadFunc() {
    EventLoop loop;

    {
        MutexLockGuard lock(mutex_);
        loop_ = &loop;      // 这个loop变量会被返回
        cond_.notify();     // 信号量notify后会唤醒阻塞的进程
    }

    loop.loop();        // 执行loop函数
    loop_ = NULL;
}

EventLoopThread::EventLoopThread() 
  : loop_(NULL), 
    thread_(bind(&EventLoopThread::threadFunc, this), "EventLoopThread"),
    mutex_(), 
    cond_(mutex_),
    is_exit_(false) { }

EventLoopThread::~EventLoopThread() {
    is_exit_ = true;
    if(loop_ != NULL) {
        loop_->quit();
        thread_.join();
    }
}

EventLoop* EventLoopThread::startLoop() {
    assert(!thread_.isstarted());
    thread_.start();

    {
        MutexLockGuard lock(mutex_);
        while(loop_ == NULL) cond_.wait();    // wait阻塞后，就会主动释放锁
    }

    return loop_;       // 主线程返回在子线程中创建的loop
}    

