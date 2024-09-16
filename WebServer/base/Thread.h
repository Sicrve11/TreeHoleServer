/* @Author shigw    @Email sicrve@gmail.com */
// 封装thread，主要是线程启动、结束、获取线程id、区分线程等操作
#pragma once
#include <pthread.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <functional>
#include <memory>
#include <string>
#include "CountDownLatch.h"
#include "noncopyable.h"

class Thread : noncopyable {
public:
    typedef std::function<void()> ThreadFunc;
    
    explicit Thread(const ThreadFunc&, const std::string& name =  std::string());
    ~Thread();

    // 核心函数
    void start();
    int join();

    // 获取状态函数
    bool isstarted() const { return is_started; }
    pid_t tid() const { return tid_; }
    const std::string& name() const { return name_; }

private:
    pid_t tid_;             // 进程id
    pthread_t pthreadId_;   // 线程id
    ThreadFunc func_;
    std::string name_;
    CountDownLatch latch_;

    bool is_started;
    bool is_joined;

    void setDefaultName();
};
