//#include "Arduino.h"
#include "IntervalTimer.h"

IntervalTimer::IntervalTimer(Interval* interval) :
	IClockable(), interval_(interval), running_(), resume_(), time_()
{
}

void IntervalTimer::interval(Interval* interval)
{
	interval_ = interval;
	if(running_)
		reset();
}

msecs_t IntervalTimer::elapsed() const
{
	return running_ ? millis() - time_ : time_;
}

void IntervalTimer::start()
{
	if (!running_)
	{
		running_ = true;
		if (resume_)
		{
			time_ = elapsed();
			resume_ = false;
		}
		else
			reset();
	}
}

void IntervalTimer::stop()
{
	if (running_)
	{
		time_ = elapsed();
		running_ = false;
		resume_ = true;
	}
}

void IntervalTimer::reset()
{
	time_ = running_ ? millis() : 0;
	resume_ = false;
}

///////////////////////////////////////////////////////////////////////////////

void IntervalTimer::clock() 
{
	if (running_ && interval_ && interval_->expired(elapsed()))
		trigger();
}

void IntervalTimer::trigger()
{
	interval_->command_.execute();
	reset();
}