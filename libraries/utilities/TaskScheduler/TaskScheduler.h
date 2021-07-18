/*
 *	This files declares an asynchronous task scheduling (multitasking) class.
 *
 *	***************************************************************************
 *
 *	File: TaskScheduler.h
 *	Date: July 18, 2021
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
 *	**************************************************************************/

#if !defined TASKSCHEDULER_H__
# define TASKSCHEDULER_H__ 20210718L 

# include "library.h"		// Arduino API.
# include "types.h"			// `Arduino' and `stdint' types.
# include "array.h"			// STL fixed-size array types.
# include "IClockable.h"	// `IClockable' interface.

// Asynchronous task scheduling type.
class TaskScheduler
{
public:
	// Scheduled task type.
	class Task
	{
		friend class TaskScheduler; // Taskscheduler needs access to private members.

	public:
		// Task state type.
		enum class State
		{
			Idle = 0,	// Task is active.
			Active		// Task is idle (inactive).
		};

	public:
		// Default constructor.
		Task() = default;
		// Task constructor.
		Task(ICommand*, msecs_t, State);
		// No copy constructor.
		Task(const Task&) = delete;
		// No copy assignment operator.
		Task& operator=(const Task&) = delete;

	public:
		// Returns a mutable iterator to the task command object.
		ICommand*		command();
		// Returns an immutable iterator to the task command object.
		const ICommand* command() const;
		// Returns a mutable reference to the task interval. 
		msecs_t&		interval();
		// Returns an immutable reference to the task interval. 
		const msecs_t&	interval() const;
		// Returns a mutable reference to the task state.
		State&			state();
		// Returns an immutable reference to the task state.
		const State&	state() const;
		// Returns true if another task has the same command object.
		bool			operator==(const Task&) const;

	private:
		ICommand*	command_;	// The current command object.
		msecs_t		interval_;	// The current scheduling interval.
		msecs_t		last_;		// The last time this task was scheduled.
		State		state_;		// The current task state.
	};

	using container_type = ArrayWrapper<Task*>;
	using iterator = container_type::iterator;
	using const_iterator = container_type::const_iterator;

public:
	TaskScheduler() = default;
	template <size_t Size>
	explicit TaskScheduler(Task* (&)[Size]);
	TaskScheduler(Task* [], size_t);
	TaskScheduler(Task**, Task**);

public:
	// Sets the current tasks collection from a range.
	void tasks(Task**, Task**);
	// Checks for and executes any currently scheduled tasks.
	void tick();

private:
	// Returns `true' if the given task is scheduled for execution, else returns `false'.  
	bool scheduled(const_iterator) const;

private:
	container_type tasks_;	// The current tasks collection.
};

template <size_t Size>
TaskScheduler::TaskScheduler(Task* (&tasks)[Size]) : 
	tasks_(tasks) 
{

}

#endif // !defined TASKSCHEDULER_H__