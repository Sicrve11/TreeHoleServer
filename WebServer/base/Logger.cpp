/* @Author shigw    @Email sicrve@gmail.com */
// 日志类
#include "Logger.h"
#include "AsyncLogging.h"
#include "LogStream.h"
#include "CurrentThread.h"
#include "Thread.h"
#include <time.h>  
#include <sys/time.h> 

std::string Logger::logFileName_ = "./WebServer.log";

static pthread_once_t once_control_ = PTHREAD_ONCE_INIT;
static AsyncLogging *AsyncLogger_;      // 异步写回对象

void once_init() {
    AsyncLogger_ = new AsyncLogging(Logger::getLogFileName());
    AsyncLogger_->start();  
}

void output(const char* msg, int len) {
    pthread_once(&once_control_, once_init);          // 只调用一次, 输出线程只启动一次
    AsyncLogger_->append(msg, len);             // 添加打印时间
}

Logger::Impl::Impl(const char *fileName, int line)
  : filename_(fileName), line_(line), outStream_() { formatTime(); }

// 在创建时输出时间
void Logger::Impl::formatTime() {
    struct timeval tv;
    gettimeofday (&tv, NULL);
    
    time_t time = tv.tv_sec;

    char str_t[26] = {0};
    struct tm* p_time = localtime(&time);   
    strftime(str_t, 26, "%Y-%m-%d %H:%M:%S\n", p_time);
    outStream_ << str_t;
}


Logger::Logger(const char* fileName, int line) : impl_(fileName, line) { }


Logger::~Logger() {
    impl_.outStream_ << "\n -- " << impl_.filename_ << ':' << impl_.line_ << '\n';
    const LogStream::Buffer& buf(getStream().getBuffer());
    output(buf.getStartPtr(), buf.getDataLen());
}
