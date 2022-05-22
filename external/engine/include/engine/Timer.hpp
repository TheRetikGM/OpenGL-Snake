#pragma once
#include <functional>
#include <algorithm>

class Timer
{
public:
    std::function<void(Timer*)> OnEnd = [](Timer* t) {};
    bool bEnded = true;

    Timer() = default;
    virtual ~Timer() {}

    // Duration in seconds
    inline void Start(float duration, std::function<void(Timer*)> onEnd) { Restart(duration, onEnd); }
    // Duration in seconds
    void Restart(float duration, std::function<void(Timer*)> onEnd)
    {
        OnEnd = onEnd;
        fDuration = duration;
        fCurrentDuration = 0.0f;
        bEnded = false;
    }
    void Update(float dt)
    {
        if (bEnded)
            return;

        fCurrentDuration += dt;
        if (fCurrentDuration >= fDuration)
        {
            OnEnd(this);
            if (!bRepeat)
                bEnded = true;
            fCurrentDuration = 0.0f;
        }
    }
    inline Timer& Repeat(bool b) { bRepeat = b; return *this;}
    inline Timer& Stop() { bEnded = true; return *this; }
    inline Timer& SetDuration(float duration_in_seconds) { fDuration = duration_in_seconds; return *this; }
protected:
    float fDuration = 0.0f;
    float fCurrentDuration = 0.0f;
    bool bRepeat = false;
};