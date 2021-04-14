#include "library.h" // `millis()'
#include "Timer.h"

Timer::Timer(msecs_t interval) : 
	interval_(interval) 
{

}

void Timer::interval(msecs_t interval)
{
	reset();
	interval_ = interval;
}

msecs_t Timer::interval() const
{
	return interval_;
}

void Timer::start(msecs_t intvl)
{
	if (!active())
	{
		interval(intvl);
		resume();
	}
}

void Timer::start()
{
	if (!active())
	{
		reset();
		resume();
	}
}

void Timer::stop()
{
	if (active())
	{
		end_ = millis();
		active_ = false;
	}
}

void Timer::resume()
{
	if (!active())
	{
		begin_ = millis() - elapsed();
		active_ = true;
	}
}

void Timer::reset()
{
	begin_ = end_ = millis();
}

msecs_t Timer::elapsed() const
{
	msecs_t time = active() ? millis() : end_;

	return time - begin_;
}

bool Timer::expired()
{
	return active() && (interval()) && !(elapsed() < interval());
}

bool Timer::active() const
{
	return active_;
}