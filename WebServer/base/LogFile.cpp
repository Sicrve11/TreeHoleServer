/* @Author shigw    @Email sicrve@gmail.com */

#include "LogFile.h"
#include "MutexLock.h"

using namespace std;

LogFile::LogFile(const string& basename, int flushEveryN) 
    : count_(0), mutex_(new MutexLock), basename_(basename), flushEveryN_(flushEveryN)
{
    file_.reset(new AppendFile(basename));
}

LogFile::~LogFile() { }

void LogFile::append(const char* logline, int len) {
    MutexLockGuard lock(*mutex_);
    this->append_unlocked(logline, len);
}

void LogFile::flush() {
    MutexLockGuard lock(*mutex_);
    file_->flush();
}

void LogFile::append_unlocked(const char* logline, int len) {
    file_->append(logline, len);
    ++count_;
    if(count_ >= flushEveryN_) {        // 如果写入的次数超过指定数目才写会文件
        count_ = 0;
        file_->flush();
    }
}

