/* @Author shigw    @Email sicrve@gmail.com */
#include "Thread.h"
#include <cassert>
#include <cstdio>
#include <linux/unistd.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/prctl.h>
#include "CountDownLatch.h"
#include "CurrentThread.h"

using namespace std;

void Thread::setDefaultName() {
    if(name_.empty()) {
        char buf[32];
        // 限制输入大小的缓冲区输入函数
        snprintf(buf, sizeof(buf), "Thread");
        name_ = buf;
    }
}

Thread::Thread(const ThreadFunc& func, const std::string& name)
   :tid_(0), pthreadId_(0), func_(func), name_(name), latch_(1),
    is_started(false), is_joined(false) { 
        setDefaultName(); 
    }


Thread::~Thread() {
    // 将正在运行的，没有被销毁的，进行销毁
    if(is_started && !is_joined) pthread_detach(pthreadId_);
}


// 获取当前线程的信息 tid 以及名称等信息
namespace CurrentThread {
    __thread int t_cachedTid = 0;
    __thread char t_tidString[32];
    __thread int t_tidStringLength = 6;
    __thread const char* t_threadName = "default";
}

// 获取linux下线程唯一的标识符tid
pid_t gettid() { return static_cast<pid_t>(::syscall(SYS_gettid)); }

void CurrentThread::cacheTid() {    
    if (t_cachedTid == 0) {
        t_cachedTid = gettid();
        t_tidStringLength =
            snprintf(t_tidString, sizeof t_tidString, "%5d ", t_cachedTid);
    }
}

// 跟python多进程编程一样，都需要额外设置运行函数，负责传入数据啥的
struct ThreadData {
    typedef Thread::ThreadFunc ThreadFunc;
    pid_t* tid_;
    string name_;
    ThreadFunc func_;
    CountDownLatch* latch_;

    ThreadData(const ThreadFunc& func, const string& name, pid_t* tid, CountDownLatch* latch) 
        : tid_(tid), name_(name), func_(func), latch_(latch) { }

    void runInThread() {
        *tid_ = CurrentThread::tid();   // 运行保存所属进程信息
        tid_ = NULL;
        latch_->countDown();            // 负责与外部调用函数进行同步
        latch_ = NULL;
        
        // 维护线程名称
        CurrentThread::t_threadName = name_.empty() ? "Thread" : name_.c_str();
        prctl(PR_SET_NAME, CurrentThread::t_threadName);    // 设置当前线程名称

        func_();
        CurrentThread::t_threadName = "finished";
    }
};

void* runThread(void* obj) {      // 线程服务函数
    ThreadData* data = static_cast<ThreadData*>(obj);
    data->runInThread();
    delete data;
    return NULL;
}

// 核心函数
void Thread::start() {
/*
    start 函数主要涉及到这些内容：
    1.通过自定义一个ThreadData数据结构，以及自定义一个线程函数来管理线程状态
    2.设置一个latch信号量对线程运行状态进行同步，确保线程运行
*/
    assert(!is_started);
    is_started = true;


    // pthread_create创建的线程不会直接运行主线程的代码，而是执行传递给它的线程函数
    // 因此需要自定义运行函数，同时会通常自定义数据结构来保存线程的名称以及其他信息
    ThreadData* data = new ThreadData(func_, name_, &tid_, &latch_);
    if(pthread_create(&pthreadId_, NULL, &runThread, data)) {     
        // 如果线程创建失败
        is_started = false;
        delete data;
    } else {                // 如果线程创建成功 返回0
        latch_.wait();      // 确保真的在运行后主线程才返回
        assert(tid_ > 0);
    }
}


int Thread::join() {
    assert(is_started);
    assert(!is_joined);
    is_joined = true;       // 以免被多次销毁
    return pthread_join(pthreadId_, NULL);
}
