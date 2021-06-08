#include "RotaryActuator.h"

RotaryActuator::RotaryActuator(IServo& servo, callback_type callback) :
	servo_(servo), cmd_angle_(InvalidAngle), callback_(callback), state_(State::Init)
{

}

void RotaryActuator::begin()
{
	state(servo_.sweep() == InvalidAngle ? State::Error : State::Idle);
}

void RotaryActuator::position(angle_t angle)
{
	if (angle != InvalidAngle && state_ == State::Idle)
	{
		if (rotate(angle))
			state(State::Active);
	}
}

RotaryActuator::angle_t RotaryActuator::position() const
{
	return state_ == State::Idle ? servo_.sweep() : InvalidAngle;
}

RotaryActuator::State RotaryActuator::state() const
{
	return state_;
}

void RotaryActuator::step()
{
	switch (state_)
	{
	case RotaryActuator::State::Init:
	case RotaryActuator::State::Active:
		if (cmd_angle_ != servo_.sweep())
			servo_.clock();
		else
			state(State::Idle);
		break;
	default:
		break;
	}
}

size_t RotaryActuator::rotate(angle_t angle)
{
	cmd_angle_ = angle;
	return servo_.sweep(angle);
}

void RotaryActuator::state(State new_state)
{
	if (new_state != state_)
	{
		state_ = new_state;
		if(callback_)
			(*callback_)(new_state);
	}
}

void RotaryActuator::clock()
{
	step();
}