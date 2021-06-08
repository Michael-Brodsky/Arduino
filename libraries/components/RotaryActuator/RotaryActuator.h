/*
 *	This file declares an asynchronous rotary actuator controller class.
 *
 *	***************************************************************************
 *
 *	File: RotaryActuator.h
 *	Date: May 31, 2021
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

#if !defined ROTARYACTUATOR_H__
# define ROTARYACTUATOR_H__ 20210531L

# include "IComponent.h"	// `IComponent' interface class.
# include "SweepServo.h"	// `SweepServo' type.

// Asynchronous rotary actuator controller class.
class RotaryActuator : public IComponent, public IClockable
{
public:
	using angle_t = IServo::angle_t;							// Type that stores angles values in degrees.
	static const angle_t InvalidAngle = IServo::InvalidAngle;	// Constant indicating an invalid angle.

	// Enumerates valid RotaryActuator states.
	enum class State
	{
		Init = 0,	// Initializing
		Idle,		// Idle (inactive)
		Active,		// Active
		Error		// An error occured.
	};

	using callback_type = void(*)(State);	// RotaryActuator callback signature.

public:
	// RotaryActuator constructor.
	RotaryActuator(IServo&, callback_type callback = nullptr);

public:
	// Initializes the RotaryActuator object.
	void	begin();
	// Commands the actuator to rotate to the given position.
	void	position(angle_t);
	// Returns the actuator's current position.
	angle_t position() const;
	// Returns the actuator's current state.
	State	state() const;
	// Steps the actuator to the currently commanded position.
	void	step();

private:
	// Initiates the actuator repositioning process.
	size_t	rotate(angle_t);
	// Executes the callback, if any, on a state change.
	void	state(State);
	// Calls the `step()' method.
	void	clock() override;

private:
	IServo&			servo_;		// The actuator's servo hardware mechanism.
	angle_t			cmd_angle_;	// The currently commanded rotation angle.
	callback_type	callback_;	// The current callback.
	State			state_;		// The actuator's current state.
};

#endif // !defined ROTARYACTUATOR_H__