/*
 *	This files declares an asynchronous interval timer class.
 
 *	***************************************************************************/

/*	File: IntervalTimer.h
 *	Date: Janury 30, 2020
 *	Version: 0.99
 *	Author: Michael Brodsky
 *	Email: mbrodskiis@gmail.com
 *	Copyright (c) 2012-2020 Michael Brodsky */

/*	***************************************************************************
 *
 *  This file is part of "Pretty Good" (Pg). "Pg" is free software: 
 *	you can redistribute it and/or modify it under the terms of the 
 *	GNU General Public License as published by the Free Software Foundation, 
 *	either version 3 of the License, or (at your option) any later version.
 *
 *  This file is distributed in the hope that it will be useful, but 
 *	WITHOUT ANY WARRANTY; without even the implied warranty of 
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 *	GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License 
 *	along with this file. If not, see <http://www.gnu.org/licenses/>.
 *
 *	**************************************************************************/

/*
 *	Description:
 *
 *		The `IntervalTimer' class can be used to execute a `Command' object 
 *		(see ICommand.h) asynchronously (see IClockable.h) at specified 
 *		intervals. It contains a nested class `Interval' that encapsulates the 
 *		time interval and command to execute.
 *
 *	Examples:
 *
 *	// Sketch to blink the on-board LED every second: 
 *
 *	#include <IntervalTimer.h>
 *
 *	class LedToggleCommand : public ICommand
 *	{
 *	public:
 *	  LedToggleCommand(pin_t led_pin, bool led_on = false) : 
 *	    led_pin_(led_pin), led_on_(led_on) { pinMode(led_pin, OUTPUT); }
 *	public:
 *	  void execute() override 
 *	  { 
 *		led_on_ = !led_on_; 
 *		digitalWrite(led_pin_, led_on_); 
 *	  }
 *	private:
 *	  pin_t   led_pin_;
 *	  bool    led_on_;
 *	};
 *	
 *	LedToggleCommand led_toggle(13);
 *	IntervalTimer::Interval led_interval(1000, led_toggle);
 *	IntervalTimer led_timer(led_interval, true);
 *	
 *	void setup() 
 *	{
 *	}
 *	
 *	void loop() 
 *	{
 *	  iclock(led_timer);
 *	}
 */

#if !defined INTERVALTIMER_H__
#define INTERVALTIMER_H__ 20200130L

#include <ClockCommand.h>	// `IClockable' and `ICommand' interface classes.

class IntervalTimer : public IClockable	// Interval timer type.
{
public:
	struct Interval	// IntervalTimer interval type.
	{
		msecs_t interval_;	// Time interval in milliseconds.
		ICommand& command_;	// Command to execute when elapsed.

		// Assignment constructor.
		Interval(msecs_t interval, ICommand& command) :
			interval_(interval), command_(command)
		{
		}

		// Assignment operator.
		Interval& operator=(const Interval& other)
		{
			interval_ = other.interval_;
			command_ = other.command_;
			return *this;
		}
	};

public:
	// Assignment constructor.
	IntervalTimer(Interval&, bool start_timer = false);

public:
	// Assigns the specified timer interval.
	void	interval(Interval&, bool);

	// Returns the current timer interval's elapsed time.
	msecs_t elapsed() const;

	// Starts the current timer interval.
	void	start();

	// Stops the current timer interval.
	void	stop();

	// Resets the current timer interval.
	void	reset();

private:
	// `IClockable' `clock()' function implementation.
	void	clock() override;

	// Resets the current timer interval and executes its `Command' object.
	void	trigger();

private:
	Interval&	interval_;	// The current timer interval.
	bool		running_;	// Flag indicating whether the timer is currently running.
	bool		resume_;	// Flag indicating whether current interval will resume or be reset when started.
	msecs_t		time_;		// Stores the current timer interval's start/elapsed time.
};

#endif // !defined INTERVALTIMER_H__


