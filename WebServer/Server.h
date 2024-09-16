/* @Author shigw    @Email sicrve@gmail.com */

#pragma once
#include <memory>
#include "Channel.h"
#include "EventLoop.h"
#include "EventLoopThreadPool.h"
#include "SkipList.h"

using namespace std;

class Server {
public:
    Server(EventLoop *loop, int threadNum, int port, const string& treeholefile, int maxItems, int maxLevels);
    ~Server();

    void start();
    void handNewConn();
    void handThisConn();

    EventLoop* getLoop() const;

private:
    EventLoop *loop_;       // 主reactor
    int threadNum_;

    SP_Channel acceptChannel_;                              // 已连接队列中取出的套接字
    unique_ptr<EventLoopThreadPool> eventLoopThreadPool_;   // loop线程池
    shared_ptr<SkipList> skTreeHole_;                       // 树洞跳表
    
    int port_;
    int listenFd_;      // 监听的套接字

    static const int MAXFDS = 100000;
    bool started_;
};



