/* @Author shigw    @Email sicrve@gmail.com */

#include "CountDownLatch.h"
#include "MutexLock.h"

CountDownLatch::CountDownLatch(int count) 
    : mutex_(), cond_(mutex_), count_(count) {}

// 如果不成功则会阻塞
void CountDownLatch::wait() {   
    MutexLockGuard lock(mutex_);
    while(count_ > 0) {
        cond_.wait();
    }
}

// 成功后通知所有阻塞的进程
void CountDownLatch::countDown() {
    MutexLockGuard lock(mutex_);
    --count_;
    if(count_ == 0) {
        cond_.notifyAll();      // 这里是唤醒all，即为随机唤醒一个
    }
}
