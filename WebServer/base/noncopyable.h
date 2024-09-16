/* @Author shigw    @Email sicrve@gmail.com */
// 该基类限制了复制函数和拷贝构造函数，因此继承了该类的派生类无法被复制
#pragma once

class noncopyable {
protected:
    noncopyable() {}
    ~noncopyable() {}

private:
    noncopyable(const noncopyable&) = delete;
    const noncopyable& operator=(const noncopyable&) = delete;
};

// 通过类的继承机制来实现函数功能的禁用，在C++11中，可以通过显示定义为 = delete 来进行禁用