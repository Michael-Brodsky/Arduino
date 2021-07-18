/*
 *	This file defines an asynchronously sweepable servo controller class.
 *
 *	***************************************************************************
 *
 *	File: SweepServo.h
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

#if !defined SWEEPSERVO_H__ 
# define SWEEPSERVO_H__ 20210718L

# include "Servo.h"			// Arduino `Servo' API.
# include "IClockable.h"	// `IClockable' interface.
# include "IServo.h"		// `IServo' interface.

template<class T>
class SweepServo : public IServo
{
public:
	using self_type = typename servo_traits<T>::type;

	static const step_t DefaultStepSize = 4U;

public:
	SweepServo();

public:
	pin_t	attach(pin_t);
	bool	attached() const;
	angle_t	initialize(angle_t angle = 0);
	angle_t initAngle() const;
	size_t	sweep(angle_t angle) override;
	angle_t	sweep() const override;
	void	stepSize(step_t);
	step_t	stepSize() const;

private:
	bool	initialized() const;
	step_t	angleToStep(angle_t) const;
	angle_t	stepToAngle(step_t) const;
	void	beginSweep(step_t);
	void	rotate(step_t, step_t);
	void	clock() override;

private:
	mutable Servo	servo_;
	step_t			current_step_;
	step_t			final_step_;
	stepdir_t		step_dir_;
	step_t			step_size_;
	size_t			steps_remaining_;
	angle_t			init_angle_;
};

template <class T>
SweepServo<T>::SweepServo() :
	current_step_(self_type::InvalidStep), final_step_(self_type::InvalidStep),
	step_dir_(), steps_remaining_(), step_size_(DefaultStepSize)
{

}

template <class T>
pin_t SweepServo<T>::attach(pin_t pin)
{
	servo_.attach(pin, self_type::min_pulse_width, self_type::max_pulse_width);
	
	return attached() ? pin : InvalidPin;
}

template <class T>
bool SweepServo<T>::attached() const 
{
	return servo_.attached();
}

template <class T>
typename SweepServo<T>::angle_t SweepServo<T>::initialize(angle_t angle)
{
	if (attached())
	{
		rotate(self_type::max_pulse_width, self_type::min_pulse_width);
		delay(self_type::min_sweep_delay);
		rotate(self_type::min_pulse_width, angleToStep(angle));
		delay(self_type::min_sweep_delay);
		current_step_ = final_step_ = servo_.readMicroseconds();
	}

	return sweep();
}

template <class T>
void SweepServo<T>::stepSize(step_t size)
{
	step_size_ = size;
}

template <class T>
size_t SweepServo<T>::sweep(angle_t angle)
{
	if (initialized())
		beginSweep(angleToStep(angle));

	return steps_remaining_;
}

template <class T>
typename SweepServo<T>::angle_t SweepServo<T>::sweep() const
{
	return initialized()
		? stepToAngle(current_step_)
		: InvalidAngle;
}

template <class T>
typename SweepServo<T>::step_t SweepServo<T>::stepSize() const
{
	return step_size_;
}

template <class T>
bool SweepServo<T>::initialized() const
{
	return current_step_ != self_type::InvalidStep;
}

template <class T>
typename SweepServo<T>::step_t SweepServo<T>::angleToStep(angle_t angle) const
{
	return static_cast<step_t>(map(
		angle,
		0,
		self_type::max_control_angle,
		self_type::min_pulse_width,
		self_type::max_pulse_width));
}

template <class T>
typename SweepServo<T>::angle_t SweepServo<T>::stepToAngle(step_t step) const
{
	return static_cast<angle_t>(map(
		step + 1U,
		self_type::min_pulse_width,
		self_type::max_pulse_width,
		0,
		self_type::max_control_angle));
}

template <class T>
void SweepServo<T>::beginSweep(step_t to)
{
	const step_t sweep_diff = static_cast<step_t>(abs(static_cast<stepdir_t>(current_step_ - to)));

	step_dir_ = static_cast<stepdir_t>(to < current_step_ ? -static_cast<stepdir_t>(step_size_) : step_size_);
	steps_remaining_ = sweep_diff / step_size_;
	final_step_ = to;
	clock();
}

template <class T>
void SweepServo<T>::rotate(step_t from, step_t to)
{
	const step_t diff = static_cast<step_t>(abs(static_cast<stepdir_t>(from - to)));
	const stepdir_t dir = static_cast<stepdir_t>(to < from ? -static_cast<stepdir_t>(step_size_) : step_size_);
	const msecs_t interval = step_interval<self_type>(step_size_);
	size_t steps = diff / step_size_;
	step_t step = from;
	while (steps--)
	{
		servo_.writeMicroseconds(step);
		step += dir;
		delayMicroseconds(interval);
	}
	servo_.writeMicroseconds(to);
}

template <class T>
void SweepServo<T>::clock()
{
	if (current_step_ != final_step_)
	{
		if (steps_remaining_)
		{
			--steps_remaining_;
			current_step_ += step_dir_;
		}
		else
			current_step_ = final_step_;
		servo_.writeMicroseconds(current_step_);
	}
}

#endif // !defined SWEEPSERVO_H__ 