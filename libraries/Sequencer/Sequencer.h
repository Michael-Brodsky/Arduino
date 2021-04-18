/*
 *	This files declares a class that executes a sequence of Command objects 
 *	chronologically.
 *
 *	***************************************************************************
 *
 *	File: Sequencer.h
 *	Date: April 15, 2021
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
 *		The `Sequencer' class encapsulates the behavior of a chronological 
 *		event sequencer. It executes a collection of Command objects (see 
 *		<ICommand.h>) in order at specified intervals. Events are encapsulated 
 *		in the nested `Event' type and have three properties: a human readable 
 *		name, a duration and the Command object that executes the event. 
 *		Clients pass a collection of `Event' objects to the `Sequencer' at 
 *		time of construction and use its member methods to control execution.
 *		`Sequencer' objects can be operated synchronously with the `tick()' 
 *		method, or asynchronously with the `clock()' method (see 
 *		<IClockable.h>). Each call to `tick()' or `clock()' executes the 
 *		current event, subsequent calls check the event's time elapsed and 
 *		advance to the next event in the sequence when the current event 
 *		has expired. The `wrap()' method sets whether a sequence will 
 *		wrap-around and repeat or stop after the last event in the collection.
 * 
 *		Clients can start, stop, resume and reset the current sequence with 
 *		the respective methods and obtain the current event's information, the 
 *		time elapsed and its index (position) in the sequence. The `Sequencer' 
 *		can update clients via callbacks at the beginning and end of an event.
 *
 *	**************************************************************************/

#if !defined SEQUENCER_H__
# define SEQUENCER_H__ 20210415L

# include "iterator.h"		// `std_distance()'
# include "IComponent.h"	// `IComponent' interface.
# include "IClockable.h"	// `IClockable" and `ICommand' interfaces.
# include "Timer.h"			// `Timer' class.

// Type that executes sequences of Commands at specified intervals.
class Sequencer : public IClockable, public IComponent 
{
public:
	// Encapsulates information about an event.
	struct Event
	{
		// Enumerates the valid Event states.
		enum class State
		{
			Begin,	// State at the start of an event.
			End		// State at the completion of an event.
		};

		const char* name_;		// Human-readable name.
		msecs_t		duration_;	// Duration in milliseconds.
		ICommand*	command_;	// Event command object.

		// Default constructor.
		Event() = default;
		// Event constructor.
		Event(const char name[], msecs_t duration, ICommand* command) :
			name_(name), duration_(duration), command_(command)
		{}
		// Copy constructor.
		Event(const Event& other)
		{
			name_ = other.name_;
			duration_ = other.duration_;
			command_ = other.command_;
		}
		// Copy assignment operator.
		Event& operator=(const Event& other)
		{
			name_ = other.name_;
			duration_ = other.duration_;
			command_ = other.command_;

			return *this;
		}
	};

	// Enumerates the valid Sequencer states.
	enum class Status  
	{
		Idle = 0,	// Sequencer is not active.
		Active,		// Sequencer is active.
		Done		// Current sequence of events is completed.
	};

	using Callback = void(*)(const Event&, Event::State);	// Client callback type.
	using iterator = Event**;								// `Event' collection iterator.

public:
	// Default constructor.
	Sequencer() = default;
	// Sequence constructor.
	template <size_t Size>
	explicit Sequencer(Event* (&)[Size], Callback, bool wrap = false);
	// Sequence constructor.
	Sequencer(Event* [], size_t, Callback, bool wrap = false);
	// Sequence constructor.
	Sequencer(iterator, iterator, Callback, bool wrap = false);

public:
	// Starts the current sequence.
	void	start();
	// Stops the current sequence.
	void	stop();
	// Resets the current sequence to the beginning.
	void	reset();
	// Resumes the sequence at the current event.
	void	resume();
	// Advances to the next sequence event.
	void	next(); 
	// Advances to the previous sequence event.
	void	prev();
	// Sets the sequence wrap-around mode.
	void	wrap(bool);
	// Returns the current sequence wrap-around mode.
	bool	wrap() const;
	// Returns the Sequencer's current status.
	Status	status() const;
	// Returns a reference to the current event.
	const Event& event() const;
	// Returns the current event's time elapsed in milliseconds.
	msecs_t	elapsed() const;
	// Returns the index of the current event within the sequence.
	uint8_t	index() const;
	// Steps through and executes the current sequence chronologically.
	void	tick();

private:
	// Begins the current event.
	void	begin();
	// Advances to the next event in the current sequence.
	void	advance();
	// Ends the current event.
	void	end();
	// Rewinds the sequence to the first event.
	void	rewind();
	// Calls the `tick()' method.
	void	clock() override;

private:
	iterator	begin_;			// Points to the beginning of the current sequence collection.
	iterator	end_;			// Points to the end of the current sequence collection.
	iterator	current_;		// Points to the current event in the sequence collection.
	Callback	callback_;		// Client callback.
	bool		wrap_;			// Flag indicating whether the sequence wraps-around continuously.
	bool		done_;			// Flag indicating whether the current sequence is completed.
	Timer		event_timer_;	// Sequence event timer.
};

template <size_t Size>
Sequencer::Sequencer(Event* (&sequence)[Size], Callback callback, bool wrap) :
	begin_(sequence), end_(sequence + Size), current_(sequence), 
	callback_(callback), wrap_(wrap), done_(), event_timer_()
{

}

#endif // !defined SEQUENCER_H__