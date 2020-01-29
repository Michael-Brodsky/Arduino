/*
 *	This files declares an asynchronous interval timer class.
 
 *	***************************************************************************/

/*	File: IntervalTimer.h
 *	Date: Janury 24, 2020
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
 */

#if !defined TIMER_H__
 #define TIMER_H__ 20200124L
 #if (defined ARDUINO && ARDUINO >= 100)
  #include "arduino.h"
 #else
  #include "WProgram.h"
 #endif

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
	IntervalTimer(Interval&, bool);

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

#endif // !defined TIMER_H__


