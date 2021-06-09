/*
 *	This file declares a simple interval timer type.
 *
 *	***************************************************************************
 *
 *	File: Timer.h
 *	Date: April 9, 2021
 *	Version: 0.99
 *	Author: Michael Brodsky
 *	Email: mbrodskiis@gmail.com
 *	Copyright (c) 2012-2021 Michael Brodsky
 *
 *	***************************************************************************
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
 *	**************************************************************************
 *
 *	Description:
 *
 *	The `Timer' class encapsulates the behaviors of a simple interval timer 
 *	and can be used to schedule or time events. The interval is specified in 
 *	milliseconds. Clients an can set the interval, then start, stop, resume or 
 *	reset the timer. The class also provides methods to check how much time 
 *	has elapsed since a timer was started or last resumed, whether the current 
 *	interval has expired and whether a timer is currently running. Once the 
 *	the current interval expires, the timer remains "expired" until reset or 
 *	restarted. When running, the elapsed time continues to increase whether 
 * 	or not the current interval is expired, until the timer is stopped. A 
 *	timer whose interval is set to 0 never expires (`expired()' method always 
 *	returns `false'), but otherwise behaves identically to one with a non-zero 
 *	interval.
 * 
 *	Examples:
 * 
 *		Timer timer;
 * 
 *		void setup { 
 *			timer.interval(1000U); // Set interval to 1 second.
 *			timer.start();
 *		}
 * 
 *		void loop {
 *			if(timer.expired()) {
 *				timer.stop(); // Elapsed time stops increasing once stopped.
 *				Serial.print("expired");
 *			}
 *		}
 *
 *****************************************************************************/
#if !defined TIMER_H__ 
# define TIMER_H__ 20210409L

#include "types.h"	// "Arduino" types.

// An interval timer type.
class Timer
{
public:
	Timer() = default;
	explicit Timer(msecs_t interval);

public:
	// Sets the timer interval.
	void	interval(msecs_t interval);
	// Returns the current timer interval.
	msecs_t	interval() const;
	// Resets and starts the timer using a specified interval.
	void	start(msecs_t);
	// Resets and starts the timer using the current interval.
	void	start();
	// Stops the timer.
	void	stop();
	// Starts the timer without restting it.
	void	resume();
	// Resets the current timer interval.
	void	reset();
	// Returns the time elapsed from the current interval since last started or resumed.
	msecs_t elapsed() const;
	// Checks if the current interval has expired.
	bool	expired();
	// Checks whether the timer is currently running.
	bool	active() const;

private:
	msecs_t interval_;	// The current timer interval.
	msecs_t begin_;		// Interval start/resume time.
	msecs_t end_;		// Interval stop time.
	bool	active_;	// Flag indicating whether the timer is currently running.
};

#endif // !defined TIMER_H__ 