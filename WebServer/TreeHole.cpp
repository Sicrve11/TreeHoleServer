/* @Author shigw    @Email sicrve@gmail.com */

#include "TreeHole.h"
#include <cstddef>
#include <ctime>
#include <string>

TreeHole::TreeHole(const string& username, const string& whisper, int level) 
    : nodeLevel_(level), username_(username), whisper_(whisper)
{
    time_t now = time(0);    
    char* curr_time = ctime(&now); 
    dateTime_ = curr_time;
    dateTime_.pop_back();   // 去除换行符
    key_ = static_cast<unsigned long>(now);

    // level + 1, because array index is from 0 - level
    this->forward = vector<shared_ptr<TreeHole>>(level + 1, nullptr);  // Fill forward vector with 0(NULL) 

}

TreeHole::TreeHole(const string& username, const string& whisper, const string& dateTime, unsigned long key, int level)
   :nodeLevel_(level), username_(username), whisper_(whisper), dateTime_(dateTime), key_(key)
{
    this->forward = vector<shared_ptr<TreeHole>>(level + 1, nullptr); 
}

TreeHole::~TreeHole() { }

unsigned long TreeHole::getKey() {
    return key_;
}

string TreeHole::getItem() {
    string item = "\n-----\n";
    item += "“" + whisper_ + "”\n";
    item += "\t\t--- from " + username_ + ", " + dateTime_ + "\n-----\n"; 
    return item;
}

string TreeHole::getSaveItem() {
    string tmp = to_string(key_);
    string item = "|" + username_ + "|" + whisper_ + "|" + dateTime_ + "|" + tmp + "|";
    return item;
}

