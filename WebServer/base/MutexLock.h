/* @Author shigw    @Email sicrve@gmail.com */
// 对pthread提供的锁进行封装，统一管理
#pragma once
#include <cstdio>
#include <pthread.h>
#include "noncopyable.h"
class MutexLock : noncopyable {
public: 
    MutexLock() {
        pthread_mutex_init(&mutex, NULL);
    }
    ~MutexLock() {
        pthread_mutex_lock(&mutex);
        pthread_mutex_destroy(&mutex);
    }

    void lock() { pthread_mutex_lock(&mutex); }
    void unlock() { pthread_mutex_unlock(&mutex); }

    pthread_mutex_t *get() { return &mutex; }

private:
    pthread_mutex_t mutex;      // posix下抽象的锁，专门用于线程加锁
    friend class Condition;
};


// 使用RAII技术对锁进行封装(构造自动加锁，离开代码段后自动解锁)
class MutexLockGuard : noncopyable {
public:
    explicit MutexLockGuard(MutexLock &_mutex) : mutex(_mutex) {
        mutex.lock(); 
    }

    ~MutexLockGuard() {
        mutex.unlock();
    }

private:
    MutexLock &mutex;
};
