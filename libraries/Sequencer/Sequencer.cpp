#include "Sequencer.h"

Sequencer::Sequencer(Event* sequence[], size_t size, Callback callback, bool wrap) :
	begin_(sequence), end_(sequence + size), current_(sequence),
	callback_(callback), wrap_(wrap), done_(), event_timer_()
{

}

Sequencer::Sequencer(iterator begin, iterator end, Callback callback, bool wrap) : 
	begin_(begin), end_(end), current_(begin),
	callback_(callback), wrap_(wrap), done_(), event_timer_()
{

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
		begin();
	else
		event_timer_.interval(0);
}

void Sequencer::resume()
{
	if (status() == Status::Idle)
	{
		if (event_timer_.interval() == 0)
			start();
		else
			event_timer_.resume();
	}
}

void Sequencer::next()
{
	if (++current_ == end_)
		current_ = begin_;
}

void Sequencer::prev()
{
	if (current_ == begin_)
		current_ = end_;
	--current_;
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

msecs_t	Sequencer::elapsed() const
{
	return event_timer_.elapsed();
}

uint8_t	Sequencer::index() const
{
	return static_cast<uint8_t>(std_distance(begin_, current_) + 1);
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
	(*current_)->command_->execute();
	(*callback_)(**current_, Event::State::Begin);
}

void Sequencer::advance()
{
	if (++current_ == end_)
	{
		if (wrap_)
			current_ = begin_;
		else
		{
			stop();
			--current_;
			done_ = true;
		}
	}
}

void Sequencer::end()
{
	(*callback_)(**current_, Event::State::End);
}

void Sequencer::rewind()
{
	current_ = begin_;
	done_ = false;
}

void Sequencer::clock() 
{
	tick();
}