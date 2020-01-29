#include "IntervalTimer.h"

IntervalTimer::IntervalTimer(Interval& interval, bool start_timer = false) :
	IClockable(), interval_(interval), running_(), resume_(), time_()
{
	if (start_timer)
		start();
}

void IntervalTimer::interval(Interval& intvl, bool rst = false)
{
	interval_ = intvl;
	if (rst)
		reset();
}

msecs_t IntervalTimer::elapsed() const
{
	return millis() - time_;
}

void IntervalTimer::start()
{
	msecs_t now = millis();

	if (resume_)
		time_ = now - interval_.interval_ + time_;
	else
		reset();
	resume_ = false;
	running_ = true;
}

void IntervalTimer::stop()
{
	running_ = false;
	resume_ = true;
	time_ = elapsed();
}

void IntervalTimer::reset()
{
	if (running_)
		time_ = millis();
	else
		resume_ = false;
}

///////////////////////////////////////////////////////////////////////////////

void IntervalTimer::clock() 
{
	if (running_ && !(elapsed() < interval_.interval_))
		trigger();
}

void IntervalTimer::trigger()
{
	reset();
	interval_.command_.execute();
}