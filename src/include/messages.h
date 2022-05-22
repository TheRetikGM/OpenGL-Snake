#pragma once
#include <any>
#include <engine/interfaces/Observer.h>
#include <string>

#define MSG_NAME(msg) #msg
const int MSG_SNAKE_MASK = 0x10;   // 16 bits, which will identify snake events
const int MSG_UNKNOWN           = 0xffffffff;
const int MSG_SNAKE_MOVED       = MSG_SNAKE_MASK + 0x1;
const int MSG_SNAKE_ATE_FOOD    = MSG_SNAKE_MASK + 0x2;
const int MSG_SNAKE_DIR_CHANGED = MSG_SNAKE_MASK + 0x3;
const int MSG_SNAKE_BIT_SELF    = MSG_SNAKE_MASK + 0x4;

#define CASE_MSG(msg) case msg: return MSG_NAME(msg); break;

namespace Utils
{
    constexpr const char* GetMessageName(const int& msg)
    {
        switch (msg)
        {
        CASE_MSG(MSG_UNKNOWN);
        CASE_MSG(MSG_SNAKE_MOVED);
        CASE_MSG(MSG_SNAKE_ATE_FOOD);
        CASE_MSG(MSG_SNAKE_DIR_CHANGED);
        CASE_MSG(MSG_SNAKE_BIT_SELF);
        default: return ""; break;
        }
    }

    struct Event
    {
        IObserverSubject* obj;
        int message;
        std::any args;
    };
}