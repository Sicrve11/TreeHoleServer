/* @Author shigw    @Email sicrve@gmail.com */

#pragma once
#include <sys/epoll.h>
#include <functional>
#include <memory>
#include <string>
#include "Timer.h"

class EventLoop;
class HttpData;

class Channel {
    typedef std::function<void()> CallBack;

public:
    Channel(EventLoop *loop);
    Channel(EventLoop *loop, int fd);
    ~Channel();

    // 文件描述符相关函数
    int getFd() { return fd_; }
    void setFd(int fd) { fd_ = fd; }

    // 设置事件状态相关函数
    void setEvents(__uint32_t ev) { events_ = ev; }
    __uint32_t& getEvents() { return events_; } 
    void setRevents(__uint32_t ev) { revents_ = ev; }   // 设置运行事件

    bool EqualAndUpdateLastEvents() {
        bool ret = (lastEvents_ == events_);
        lastEvents_ = events_;
        return ret;
    }
    __uint32_t getLastEvents() { return lastEvents_; } 

    // 绑定、处理指定事件
    void setReadHandler(CallBack &&readHandler) { readHandler_ = readHandler; }
    void setWriteHandler(CallBack &&writeHandler) { writeHandler_ = writeHandler; }
    void setErrorHandler(CallBack &&errorHandler) { errorHandler_ = errorHandler; }
    void setConnHandler(CallBack &&connHandler) { connHandler_ = connHandler; }

    void handleRead();
    void handleWrite();
    void handleError(int fd, int err_num, std::string short_msg);
    void handleConn();

    void handleEvents();    // 按类别处理指定事件

    // 设置上层持有者
    void setHolder(std::shared_ptr<HttpData> holder) { holder_ = holder; }
    
    std::shared_ptr<HttpData> getHolder() {
        std::shared_ptr<HttpData> ret(holder_.lock());
        return ret;
    }

private:
    EventLoop *loop_;       // 该文件描述符所属哪个reactor
    int fd_;                // 持有的文件描述符

    std::weak_ptr<HttpData> holder_;    // 指向上层持有该Channel的对象
    
    // 该文件描述符发生的事件
    __uint32_t events_;     // channel当前监听到的事件集合
    __uint32_t revents_;    // 触发的事件集合
    __uint32_t lastEvents_; // 该channel最终的事件集合状态，用于状态比对

    // 该文件描述符的处理函数
    CallBack readHandler_;
    CallBack writeHandler_;
    CallBack errorHandler_;
    CallBack connHandler_;
};

typedef std::shared_ptr<Channel> SP_Channel;
