/*
 *	This file defines an abstract interface class and one concrete type for 
 *	implementing asynchronous, event-driven objects.
 *
 *	***************************************************************************
 *
 *	File: IClockable.h
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
 *	The `IClockable' type is an abstract interface class with one purely 
 *	virtual method `clock()' which must be implemented by derived types. It 
 *	is useful when several tasks, encapsulated in `IClockable' objects, need 
 *	to be executed asynchronously by the client, such as in multitasking or 
 *	time-sensitive applications (see e.g. <TaskScheduler.h>). 
 * 
 *	The `ClockCommand' type is a concrete type derived from `ICommand' (see 
 *	<ICommand.h>) that takes a reference to an `IClockable' object and, when 
 *	executed, calls its `clock()' method. It's provided because it is often 
 *	convenient to implement scheduled tasks as `IClockable' objects, and then 
 *	create a collection of `ClockCommands' to execute them at specified 
 *	intervals. Since `ClockCommand' derives from the `ICommand' interface it 
 *	can be mixed & matched in collections with other concrete command types, 
 *	thus making it useful when, for instance, not all tasks are of type 
 *	`IClockable' and use different execution methods.
 * 
 *	Examples:
 * 
 *	class Clockable : public IClockable
 *	{
 *	public:
 *		...
 *	private:
 *		void clock() override { Serial.print("foobar"); } // Only accessible through interface.
 *	};
 * 
 *	Clockable object;
 * 
 *	ClockCommand command(object);
 * 
 *	command.execute(); // Calls object.clock();
 *****************************************************************************/


#if !defined ICLOCKABLE_H__ 
# define ICLOCKABLE_H__ 20210409L

#include "ICommand.h" // `ICommand' interface class.

// Abstract interface class for clockable types.
struct IClockable
{
	virtual void clock() = 0;

	virtual ~IClockable() = default;
};

// Command type to call `clock()' method of IClockable objects.
class ClockCommand : public ICommand
{
public:
	explicit ClockCommand(IClockable& receiver) : receiver_(receiver) {}

public:
	void execute() override { receiver_.clock(); }

private:
	IClockable& receiver_;
};

#endif // !defined ICLOCKABLE_H__ 
