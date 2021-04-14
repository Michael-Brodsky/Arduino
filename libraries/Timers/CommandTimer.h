/*
 *	This files declares a timer class that can execute commands at specified 
 *	intervals.
 *
 *	***************************************************************************
 *
 *	File: CommandTimer.h
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
 *		The `CommandTimer' class can be used to execute `Command' objects 
 *		(see ICommand.h) at specified intervals. `CommandTimer' extends 
 *		the base class 'Timer' by adding three methods: `command()', `repeats() 
 *		and `tick()', otherwise it behaves identically except when the current  
 *		interval expires, the timer is automatically stopped or reset based on 
 *		the parameter passed to the `repeats()' method. (see <Timer.h>). The 
 *		timer is advanced by the client using the `tick()' method. Each call to 
 *		`tick()' checks time elapsed since it was last started or resumed and, 
 *		if the elapsed time is equal to or greater than the interval time, 
 *		executes the current `Command', which the client provides using the 
 *		`command()' method. The `repeats()' method determines whether the timer 
 *		is reset and continues running, or is stopped when the current interval 
 *		expires. If the current interval does not repeat, the timer must be 
 *		manually restarted. Clients can manually call the `expired()' method, 
 *		however, this is only useful if the current interval does not repeat 
 *		because otherwise any call to `expired()' always return `false'. 
 * 
 *	Examples:
 * 
 *		void foo() { Serial.print(42); }
 * 
 *		Command<void> command(&foo);
 * 
 *		CommandTimer timer(1000U, command, false);	// Executes `command' once after 1 sec.
 * 
 *		void setup() { 
 *			timer.start();
 *		}
 * 
 *		void loop() { 
 *			timer.tick();
 *		}
 *		
 *	**************************************************************************/

#if !defined COMMANDTIMER_H__
# define COMMANDTIMER_H__ 20210403L

# include "ICommand.h"	// `ICommand' interface.
# include "Timer.h"		// `Timer' class.

// Timed command type.
class CommandTimer : public Timer 
{
public:
	CommandTimer(msecs_t, ICommand&, bool repeats = false);

public:
	// Sets the timer command.
	void		command(ICommand&);
	// Returns the current timer command.
	ICommand&	command() const;
	// Sets the interval repeat mode.
	void		repeats(bool);
	// Returns the current interval's repeat mode.
	bool		repeats() const;
	// Checks the current interval's elapsed time and executes a `Command' if expired. 
	void		tick();

private:
	// Executes the current `Command' object and either resets or stops the timer.
	void		execute();

private:
	bool		repeats_;	// Flag indicating whether the current interval repeats once expired.
	ICommand&	command_;	// The `Command' object to execute when the current interval expires. 
};

#endif // !defined COMMANDTIMER_H__


