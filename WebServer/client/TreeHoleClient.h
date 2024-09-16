/* @Author shigw    @Email sicrve@gmail.com */

#pragma once
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <iostream>

using namespace std;

class TreeHoleClient {
public:
    TreeHoleClient(const string& ip, int port, const string& user);
    ~TreeHoleClient();

    int setSocketNonBlocking1(int);
    void loop();

private:
    int sockfd_;
    char buff_[4096];

    string servIP_;
    int servPort_;
    string username_;
    
};

