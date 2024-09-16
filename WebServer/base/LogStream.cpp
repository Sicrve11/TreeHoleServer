/* @Author shigw    @Email sicrve@gmail.com */
// 定义了buffer类，同时重载各种不同的数据类型输入buffer的流运算符<<
#include "LogStream.h"
#include <algorithm>

const char digits[] = "9876543210123456789";
const char* zero = digits + 9;

// From muduo
template <typename T>
size_t convert(char buf[], T value) {       // 优雅的将整形转换成C风格字符串的方法
    T i = value;
    char* p = buf;

    do {
        int lsd = static_cast<int>(i % 10);
        i /= 10;
        *p++ = zero[lsd];
    } while (i != 0);

    if (value < 0) {
        *p++ = '-';
    }
    *p = '\0';
    std::reverse(buf, p);   // 不包括\0

    return p - buf;
}

template class FixedBuffer<kSmallBuffer>;
template class FixedBuffer<kLargeBuffer>;

template <typename T>
void LogStream::formatInteger(T v) {
    // buffer容不下kMaxNumericSize个字符的话会被直接丢弃
    if (buffer_.getAvailLen() >= kMaxNumericSize) {
        size_t len = convert(buffer_.getCurPtr(), v);
        buffer_.addDataLen(len);
    }
}

LogStream& LogStream::operator<<(short v) {
    *this << static_cast<int>(v);
    return *this;
}

LogStream& LogStream::operator<<(unsigned short v) {
    *this << static_cast<unsigned int>(v);
    return *this;
}

LogStream& LogStream::operator<<(int v) {
    formatInteger(v);
    return *this;
}

LogStream& LogStream::operator<<(unsigned int v) {
    formatInteger(v);
    return *this;
}

LogStream& LogStream::operator<<(long v) {
    formatInteger(v);
    return *this;
}

LogStream& LogStream::operator<<(unsigned long v) {
    formatInteger(v);
    return *this;
}

LogStream& LogStream::operator<<(long long v) {
    formatInteger(v);
    return *this;
}

LogStream& LogStream::operator<<(unsigned long long v) {
    formatInteger(v);
    return *this;
}

LogStream& LogStream::operator<<(double v) {
    if (buffer_.getAvailLen() >= kMaxNumericSize) {
        int len = snprintf(buffer_.getCurPtr(), kMaxNumericSize, "%.12g", v);
        buffer_.addDataLen(len);
    }
    return *this;
}

LogStream& LogStream::operator<<(long double v) {
    if (buffer_.getAvailLen() >= kMaxNumericSize) {
        int len = snprintf(buffer_.getCurPtr(), kMaxNumericSize, "%.12Lg", v);
        buffer_.addDataLen(len);
    }
    return *this;
}
