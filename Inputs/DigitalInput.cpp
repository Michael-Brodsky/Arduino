#include <Arduino.h>
#include "DigitalInput.h"

DigitalInput::DigitalInput(pin_t pin, Mode mode) : 
	IInput(), pin_(), input_state_(), trigger_type_(), trigger_state_(), trigger_command_()
{
	attach(pin, mode); 
}

void DigitalInput::attach(pin_t pin, const Mode& mode)
{
	if((pin_ = pin)) 
	{
		pinMode(pin, static_cast<uint8_t>(mode)); 
		operator()(); 
	}
}

void DigitalInput::trigger(const Trigger& trigger, const State& state, ICommand* command = nullptr)  
{
	trigger_type_ = trigger;
	trigger_state_ = state;
	trigger_command_ = command;
}

DigitalInput::State DigitalInput::operator()() 
{
	return (input_state_ = static_cast<State>(digitalRead(pin_))); 
}

bool DigitalInput::operator()(const State& state)
{
	return operator()() == state;
}

bool DigitalInput::operator==(const State& state) const
{ 
	return input_state_ == state;
}

bool DigitalInput::operator!=(const State& state) const
{ 
	return !(*this == state); 
}

bool DigitalInput::triggered(ICommand* command = nullptr) 
{
	bool is_triggered = false;
	State previous = input_state_;
	State current = operator()();

	switch(trigger_type_)
	{
	case Trigger::Level:
		if(current == trigger_state_)
			is_triggered = true;
		break;
	case Trigger::Edge:
		if(current == trigger_state_ && current != previous) 
			is_triggered = true;
		break;
	default:
		break;
	}
	input_state_ = current;
	if(is_triggered)
	{
		if(command)
			command->execute();
		else if(trigger_command_)
			trigger_command_->execute();
	}

	return is_triggered;
}
