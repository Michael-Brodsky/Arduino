#include "library.h"
#include "Sequencer.h"

Sequencer::Sequencer(Event* events[], size_t size, Callback callback, bool wrap) :
	events_(events, size), current_(events), callback_(callback), 
	wrap_(wrap), done_(), exec_(), event_timer_()
{

}

Sequencer::Sequencer(Event** first, Event** last, Callback callback, bool wrap) :
	events_(first, last), current_(first), callback_(callback), 
	wrap_(wrap), done_(), exec_(), event_timer_()
{

}

Sequencer::container_type& Sequencer::events()
{
	return events_;
}

const Sequencer::container_type& Sequencer::events() const
{
	return events_;
}

void Sequencer::callback(Callback cb)
{
	callback_ = cb;
}

void Sequencer::start()
{
	if (status() != Status::Active)
	{
		rewind();
		begin();
		event_timer_.start();
	}
}

void Sequencer::stop()
{
	event_timer_.stop();
}

void Sequencer::reset()
{
	rewind();
	if (status() == Status::Active)
	{
		event_timer_.reset();
		begin();
	}
	else
		event_timer_.interval(0); // "Flag" to indicate sequencer is reset.

}

void Sequencer::resume()
{
	if (status() == Status::Idle)
	{
		if (event_timer_.interval() == 0) // Check if sequencer is reset.
			start();
		else 
		{
			if (exec_)
			{
				begin();
				exec_ = false;
			}
			event_timer_.resume();
		}
	}
}

void Sequencer::next()
{
	if (++current_ == std_end(events_))
		current_ = std_begin(events_);
	exec_ = true;
	event_timer_.interval((*current_)->duration_);
	event_timer_.reset();
}

void Sequencer::prev()
{
	if (current_ == std_begin(events_))
		current_ = std_end(events_);
	--current_;
	exec_ = true;
	event_timer_.interval((*current_)->duration_);
	event_timer_.reset();
}

void Sequencer::wrap(bool value)
{
	wrap_ = value;
}

bool Sequencer::wrap() const
{
	return wrap_;
}

Sequencer::Status Sequencer::status() const
{
	return event_timer_.active() 
		? Status::Active
		: (done_) 
			? Status::Done
			: Status::Idle;
}

const Sequencer::Event& Sequencer::event() const
{
	return **current_;
}

Sequencer::Event& Sequencer::event()
{
	return const_cast<Event&>(**current_);
}

msecs_t	Sequencer::elapsed() const
{
	return event_timer_.elapsed();
}

uint8_t	Sequencer::index() const
{
	return static_cast<uint8_t>(std_distance(std_begin(events_), current_) + 1);
}

void Sequencer::tick()
{
	if (event_timer_.expired())
	{
		end();
		advance();
		if (status() == Status::Active)
			begin();
	}
}

void Sequencer::begin()
{
	event_timer_.interval((*current_)->duration_);
	if((*current_)->command_)
		(*current_)->command_->execute();
	callback(current_, Event::State::Begin);
}

void Sequencer::advance()
{
	if (++current_ == std_end(events_))
	{
		if (wrap_)
			current_ = std_begin(events_);
		else
		{
			stop();
			--current_;
			done_ = true;
			callback(current_, Event::State::End); // So clients can check if done.
		}
	}
}

void Sequencer::end()
{
	callback(current_, Event::State::End);
}

void Sequencer::rewind()
{
	current_ = std_begin(events_);
	done_ = false;
}

void Sequencer::clock() 
{
	tick();
}

void Sequencer::callback(const_iterator event, Event::State state)
{
	if (callback_)
		(*callback_)(**event, state);
}