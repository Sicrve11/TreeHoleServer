/* @Author shigw    @Email sicrve@gmail.com */
// 定义了buffer类，同时重载各种不同的数据类型输入buffer的流运算符<<
#pragma once
#include <assert.h>
#include <cstring>
#include <string.h>
#include <string>
#include "noncopyable.h"


class AsyncLogging;     // 只使用这个类的指针，不引入头文件是防止互相引用出错


// 定义缓冲区类，char
const int kSmallBuffer = 4000;
const int kLargeBuffer = 4000 * 1000;

template <int SIZE>     // 非类型模版参数，当做常量来处理（只有int可行）
class FixedBuffer : noncopyable {
public:
    FixedBuffer() : cur_(start_) {}
    ~FixedBuffer() {}

    const char* getStartPtr() const { return start_; }
    char* getCurPtr() const { return cur_; }

    int getDataLen() const { return cur_ - start_; }
    int getAvailLen() const { return getEndPtr() - cur_; }

    void initBuffer() { memset(start_, 0, sizeof(start_)); }
    void clearBuffer() { cur_ = start_; }
    void addDataLen(size_t len) { cur_ += len; }

    void append(const char* buf, size_t len) {
        if(getAvailLen() > static_cast<int>(len)) {
            memcpy(cur_, buf, len);
            cur_ += len;
        }
    }

private:
    const char* getEndPtr() const { return start_ + sizeof(start_); }
    char* cur_;
    char start_[SIZE];
};



class LogStream : noncopyable {
public:
    typedef FixedBuffer<kSmallBuffer> Buffer;

    LogStream& operator<<(bool v) {
        buffer_.append(v ? "1" : "0", 1);
        return *this;
    }

    LogStream& operator<<(short);
    LogStream& operator<<(unsigned short);
    LogStream& operator<<(int);
    LogStream& operator<<(unsigned int);
    LogStream& operator<<(long);
    LogStream& operator<<(unsigned long);
    LogStream& operator<<(long long);
    LogStream& operator<<(unsigned long long);

    LogStream& operator<<(const void*);

    LogStream& operator<<(float v) {
        *this << static_cast<double>(v);
        return *this;
    }
    LogStream& operator<<(double);
    LogStream& operator<<(long double);

    LogStream& operator<<(char v) {
        buffer_.append(&v, 1);
        return *this;
    }

    LogStream& operator<<(const char* str) {
        if (str) {
            buffer_.append(str, strlen(str));
        } else {
            buffer_.append("(null)", 6);
        }
        
        return *this;
    }

    LogStream& operator<<(const unsigned char* str) {
        return operator<<(reinterpret_cast<const char*>(str));
    }

    LogStream& operator<<(const std::string& v) {
        buffer_.append(v.c_str(), v.size());
        return *this;
    }

    void append(const char* data, int len) {
        buffer_.append(data, len);
    }

    const Buffer& getBuffer() const { return buffer_; }
    void clearBuffer() { buffer_.clearBuffer(); }

private:
    Buffer buffer_;     // 暂时存放写入的信息
    static const int kMaxNumericSize = 32;

    template <typename T>
    void formatInteger(T);      // 对整形统一输入
};

