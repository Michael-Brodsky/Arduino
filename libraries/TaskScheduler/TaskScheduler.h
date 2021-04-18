 /*
  *	This file declares an asyncronous task scheduling class.
  *
  *	***************************************************************************
  *
  *	File: TaskScheduler.h
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
  *		The `TaskScheduler' is non-preemptive task scheduler that can 
  *		be used to give applications the appearance of concurrency, manage 
  *		processor load and schedule events in time-sensitive applications. A 
  *		"task" in this context is any callable object, such as a free-standing 
  *		function or member method. 
  * 
  *		Tasks are encapsulated in the `Task' type, a nested type within 
  *		`TaskScheduler', and divided into three fields: the `Command' object 
  *		that executes the task, the scheduling interval in milliseconds and 
  *		the task `State' which can be either `Idle' or `Active'.
  * 
  *		Clients specify the collection of tasks at time of construction. The 
  *		`TaskScheduler' is driven by the `tick()' method, which can be 
  *		synchronous (in a loop) or interrupt-driven. On each call to `tick()', 
  *		the `TaskScheduler' iterates through the task collection, checks 
  *		whether any `Active' tasks are currently scheduled and executes them. 
  *		`Idle' tasks are never executed.
  * 
  *		Clients can set or query task states within the collection using the 
  *		`state()' method. There is currently no method to add additional tasks 
  *		to the collection at runtime. Client modification to the collection 
  *		source object has no effect on the current collection.
  *
  *****************************************************************************/

#if !defined TASKSCHEDULER_H__ 
#define TASKSCHEDULER_H__ 20210409L

#include "types.h"		// "Arduino" & `stdint' types.
#include "algorithm.h"	// `std_find()' function.
#include "IClockable.h"	// `ICommand', `IClockable' and `ClockCommand' types.

 // Asyncronous task scheduling type.
class TaskScheduler 
{
public:
	// Scheduled task type.
	class Task 
	{
		friend class TaskScheduler;	// `TaskScheduler' needs access to private members.

	public: 
		// Enumerates the valid task scheduling states.
		enum class State 
		{ 
			Idle,	// Task is idle.
			Active	// Task is active.
		};

	public:
		// Task constructor.
		Task(ICommand& command, msecs_t interval, State state) : 
			command_(command), interval_(interval), last_(), state_(state) 
		{}

	public:
		bool operator==(const Task& other) // Equal comparable on interval.
		{ 
			return this->interval_ == other.interval_; 
		}

		bool operator<(const Task& other) 	// Less than comparable on interval.
		{ 
			return this->interval_ < other.interval_; 
		}

	private:
		ICommand&	command_;	// Command to execute when task is scheduled.
		msecs_t		interval_;	// Current scheduling interval in milliseconds.
		msecs_t		last_;		// Time last scheduled.
		State		state_;		// Current scheduling state.
	};

private:
	using iterator = Task**;	// `Task' collection iterator type. 

public:
	template <size_t Size>
	explicit TaskScheduler(Task* (&)[Size]);
	TaskScheduler(Task* [], size_t);
	TaskScheduler(iterator, iterator);

public:
	// Sets the given task to the specified scheduling state.
	void		state(Task*, Task::State); 
	// Sets the given task's scheduling state according to the specified boolean value.
	void		state(Task*, bool);
	// Returns the current scheduling state of the given task.
	Task::State	state(const Task*) const;
	// Checks for and executes any scheduled tasks.
	void		tick();

private:
	// Checks whether a given task is currently scheduled.
	bool	scheduled(const Task*) const; 

private: 
	iterator	begin_;	// Points to the beginning of the current task collection.
	iterator	end_;	// Points to the end of the current task collection.
};

///////////////////////////////////////////////////////////////////////////////

template <size_t Size>
TaskScheduler::TaskScheduler(Task* (&tasks)[Size]) : 
	begin_(tasks), end_(tasks + Size) 
{
}

#endif // !defined TASKSCHEDULER_H__ 