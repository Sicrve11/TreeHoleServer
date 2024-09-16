#include "TreeHoleClient.h"
#include <string>
using namespace std;

int main(int argc, char* argv[]) {
    if(argc != 4) {     // 检查是否正确调用
        printf("Usage : %s <IP> <port> <username>\n", argv[0]);
        exit(1);
    }
    
    // 设置用户名
    string servIP, name;
    int port;

    servIP = string(argv[1]);
    port = atoi(argv[2]);
    name = string(argv[3]);
    
    TreeHoleClient thc(servIP, port, name);
    thc.loop();

    return 0;
}