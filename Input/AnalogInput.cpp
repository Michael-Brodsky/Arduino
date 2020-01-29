#include <Arduino.h>
#include "AnalogInput.h"

AnalogInput::AnalogInput(pin_t pin) : 
	IInput(), pin_(pin), input_level_(), is_triggered_(), begin_(), end_() 
{
	attach(pin);
}

void AnalogInput::attach(pin_t pin)
{
	pinMode(pin, 0);
}

analog_t AnalogInput::operator()()
{
	return (input_level_ = static_cast<analog_t>(analogRead(pin_))); 
}

bool AnalogInput::operator<(analog_t level)
{
	return operator()() < level;
}

void AnalogInput::trigger(iterator begin, iterator end)
{
	assert(!(begin == nullptr || end == nullptr));
	begin_= begin;
	end_ = end;
}

bool AnalogInput::triggered(ICommand* command = nullptr)
{
	iterator it = begin_;

	(void)operator()();
	while(it != end_)
	{
		if(input_level_ < (*it)->level_)
			break;
		++it;
	}
	if(!(is_triggered_ || it == end_))
	{
		if(command)
			command->execute();
		if((*it)->command_)
			(*it)->command_->execute();
	}
	is_triggered_ = it != end_;

	return is_triggered_;
}

void AnalogInput::clock()
{
	(void)triggered();
}
