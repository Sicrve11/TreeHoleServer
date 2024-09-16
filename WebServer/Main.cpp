/* @Author shigw    @Email sicrve@gmail.com */

#include <cstdlib>
#include <iostream>
#include <string>
#include <signal.h>
#include "EventLoop.h"
#include "Server.h"
#include "base/Logger.h"

using namespace std;

Server* myServer = nullptr;

void signalHandler(int signal) {
    std::cout << "Signal (" << signal << ") received. Shutting down..." << std::endl;
    LOG << "Signal (" << signal << ") received. Shutting down...";
    delete myServer;  // 调用析构函数, 写回数据
    exit(signal);
}

int main(int argc, char* argv[]) {

    // 绑定信号处理函数
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);
    
    int threadNum = 4;
    int port = 8080;
    std::string logPath = "./WebServer.log";
    std::string FILE_PATH = "dumpFile.txt";
    int maxItems = 10;
    int maxLevels = 10;

    // parse args 扩展执行程序时输入的参数
    // 待补充

    Logger::setLogFileName(logPath);    // 开启日志
    cout << "Logger started!" << endl;

    EventLoop mainLoop;      // 主reactor
    myServer = new Server(&mainLoop, threadNum, port, FILE_PATH, maxItems, maxLevels);    // 服务器

    // 启动
    myServer->start();
    cout << "Server started!" << endl;

    mainLoop.loop();

    return 0;
}