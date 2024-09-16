/* @Author shigw    @Email sicrve@gmail.com */

#pragma once
#include <cstddef>
#include <string>
#include <vector>
#include <memory>
using namespace std;

class TreeHole {
public:
    TreeHole(const string& username, const string& whisper, int level);
    TreeHole(const string& username, const string& whisper, const string& dateTime, unsigned long key, int level);
    ~TreeHole();

    unsigned long getKey();
    string getItem();
    string getSaveItem();

    int nodeLevel_;      // Linear array to hold pointers to next node of different level
    vector<shared_ptr<TreeHole>> forward;

private:
    string username_;
    string whisper_;
    string dateTime_;
    unsigned long key_;
};