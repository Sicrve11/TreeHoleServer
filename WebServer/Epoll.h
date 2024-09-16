/* @Author shigw    @Email sicrve@gmail.com */

#pragma once
#include <sys/epoll.h>
#include <memory>
// #include <unordered_map>
#include <vector>
#include "Channel.h"
#include "HttpData.h"
#include "Timer.h"

class Epoll {
public:
    Epoll();
    ~Epoll();

    // 管理监听事件, 并设置超时时限
    void epoll_add(SP_Channel request, int timeout);
    void epoll_mod(SP_Channel request, int timeout);
    void epoll_del(SP_Channel request);

    // 核心函数
    std::vector<SP_Channel> poll();
    std::vector<SP_Channel> getEventsRequest(int events_num);
    
    // 事件的时间控制函数
    void add_timer(SP_Channel request_data, int timeout);
    void handleExpired();

    int getEpollFd() { return epollFd_; }       // 返回epoll信息
    
private:
    static const int MAXFDS = 100000;

    int epollFd_;                                   // epoll的文件描述符
    std::vector<struct epoll_event> events_;        // epoll的监听队列
    SP_Channel fd2chan_[MAXFDS];      // 这里就是预先定义好的每个事件的channel指针
    // epoll_add的时候其实是拿着对应的channel，来进行add
    std::shared_ptr<HttpData> fd2http_[MAXFDS];
    TimerManager timerManager_;
};