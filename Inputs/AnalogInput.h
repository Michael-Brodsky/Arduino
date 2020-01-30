
 *	***************************************************************************/

/*	File: AnalogInput.h
 *	Date: November 19, 2019
 *	Version: 0.99
 *	Author: Michael Brodsky
 *	Email: mbrodskiis@gmail.com
 *	Copyright (c) 2012-2020 Michael Brodsky */

/*	***************************************************************************
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

/*
 *	Description:
 *
 */

#if !defined ANALOGINPUT_H__
#define ANALOGINPUT_H__ 20200119L
#include <assert.h>			// assert() macro.
#include <IInput.h>			// IInput interface class.
#include <ClockCommand.h>	// ClockCommand interface class.

class AnalogInput : public IInput, public IClockable	// Asynchronous analog GPIO input polling type.
{
public:		/**** Member Types and Constants ****/
	struct Trigger	// AnalogInput trigger type.
	{
		analog_t	level_;		// Input trigger level.
		ICommand*	command_;	// Command to execute when input is triggered.
	};

	typedef Trigger** iterator;	// AnalogInput trigger iterator type.

public:		/**** Ctors ****/ 
	explicit AnalogInput(pin_t);				// Attach constructor.

public:		/**** Member Functions ****/
	void		attach(pin_t);					// Attaches a GPIO input pin.
	analog_t	operator()();					// Returns the attached pin's current input level.
	bool		operator<(analog_t);			// Checks whether the attached pin's current input level is less than a given level.
	void		trigger(iterator, iterator);	// Assigns a set of input trigger objects.
	bool		triggered(ICommand*) override;	// Checks whether the attached pin is currently triggered.

private:
	void		clock() override;				// IClockable `clock()' function implementation.

private:	/**** Member Objects ****/
	pin_t		pin_;							// The attached GPIO input pin.
	analog_t	input_level_;					// The attached pin's current input level.
	bool		is_triggered_;					// Flag indicating whether the attached input is currently triggered.
	iterator	begin_;							// Iterator pointing to the beginning of the current set of triggers.
	iterator	end_;							// Iterator pointing to the end of the current set of triggers.
};

#endif // !defined ANALOGINPUT_H__