#if !defined TIMER_H__
#define TIMER_H__ 20200124L

#include <stdint.h>
#include <IClockCommand.h>

class Timer : public IClockable 
{
public:
    struct Interval 
    {
        msecs_t interval_;
        ICommand& command_;
        Interval(msecs_t interval, ICommand& command) : 
            interval_(interval), command_(command) { }
        Interval& operator=(const Interval& other) 
        { 
            interval_ = other.interval_; 
            command_ = other.command_; 
            return *this; 
        }
    };
    
public:
    Timer(Interval& interval, bool start_timer = false) : 
        IClockable(), interval_(interval), running_(), start_time_()   
        { 
            if(start_timer) 
                start(); 
        }
public:
    void interval(Interval& intvl) 
    {
        interval_ = intvl;
    }
    
    void start() 
    { 
        reset(); 
        running_ = true; 
    }
    
    void stop() 
    { 
        running_ = false; 
    }
    
    void reset() 
    { 
        start_time_ = GetTimerTick(); 
    }
    
private:
    void clock() override 
    { 
        if(running_ && !(GetTimerTick() - start_time_ < interval_.interval_))
           trigger();
    }
    
    void trigger() 
    { 
        reset(); 
        interval_.command_.execute(); 
    }
private:
    Interval& interval_;
    bool running_;
    msecs_t start_time_;
};

#endif // !defined TIMER_H__
