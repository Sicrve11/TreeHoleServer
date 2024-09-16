/* @Author shigw    @Email sicrve@gmail.com */
// 就是Acceptor
#include "Server.h"
#include <cstring>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <functional>
#include "Util.h"
#include "base/Logger.h"


Server::Server(EventLoop *loop, int threadNum, int port, const string& treeholefile, int maxItems, int maxLevels)
 :  loop_(loop), 
    threadNum_(threadNum), 
    acceptChannel_(new Channel(loop_)),
    eventLoopThreadPool_(new EventLoopThreadPool(loop_, threadNum)),
    skTreeHole_(new SkipList(treeholefile, maxItems, maxLevels)),
    port_(port),
    listenFd_(socket_bind_listen(port_)),       // 这里直接listen了
    started_(false)
{
    acceptChannel_->setFd(listenFd_);       // 设置监听套接字的channel
    handle_for_sigpipe();
    if(setSocketNonBlocking(listenFd_) < 0) {
        perror("set socket non block failed");
        abort();
    }
}


Server::~Server() {
    acceptChannel_.reset();
    eventLoopThreadPool_.reset();
    skTreeHole_.reset();
}


void Server::start() {
    started_ = true;

    // 设置监听套接字的状态信息
    acceptChannel_->setEvents(EPOLLIN | EPOLLET);    // 边缘触发模式
    acceptChannel_->setReadHandler(bind(&Server::handNewConn, this));
    acceptChannel_->setConnHandler(bind(&Server::handThisConn, this));
    loop_->addToPoller(acceptChannel_, 0);      // 将套接字加入主线程的poller中

    eventLoopThreadPool_->start();
}


void Server::handNewConn() {
    struct sockaddr_in client_addr;
    memset(&client_addr, 0, sizeof(struct sockaddr_in));
    socklen_t client_addr_len = sizeof(client_addr);

    int accept_fd = 0;
    while((accept_fd = accept(listenFd_, (struct sockaddr *)&client_addr, &client_addr_len)) > 0) {
        EventLoop *loop = eventLoopThreadPool_->getNextLoop();      // 从线程池拿出一个loop
        LOG << "New connection from " << inet_ntoa(client_addr.sin_addr) << ":"
            << ntohs(client_addr.sin_port);
        cout << "New connection from " << inet_ntoa(client_addr.sin_addr) << ":"
            << ntohs(client_addr.sin_port) << endl;

        if(accept_fd >= MAXFDS) {
            close(accept_fd);
            continue;
        }

        if(setSocketNonBlocking(accept_fd) < 0) {
            LOG << "Set non block failed!";
            // perror("Set non block failed!");
            return;
        }

        setSocketNodelay(accept_fd);

        shared_ptr<HttpData> req_info(new HttpData(loop, accept_fd, skTreeHole_));
        req_info->getChannel()->setHolder(req_info);
        loop->queueInLoop(bind(&HttpData::newEvent, req_info));     // 这里的newEvent函数应该会负责将自己的channel加入poller，并设置定时器

    }

    acceptChannel_->setEvents(EPOLLIN | EPOLLET);       // 
}


void Server::handThisConn() {
    loop_->updatePoller(acceptChannel_);
}


EventLoop* Server::getLoop() const {
    return loop_;
}