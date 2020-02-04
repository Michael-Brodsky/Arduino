/*
 *	This file defines several types for setting and reading the state of 
 *	digital outputs.

 *	***************************************************************************/

/*	File: Outputs.h
 *	Date: February 4, 2020
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

#if !defined OUTPUTS_H__
# define OUTPUTS_H__ 20200204L

# include <library.h>	// pin_t type. 
# include <ICommand.h>	// ICommand interface class.

class DigitalOutput	// Type that encapsulates a GPIO digital output.
{
public:
	typedef bool State;	// DigitalOutput state type.

public:
	// Attach constructor.
	DigitalOutput(pin_t);

public:
	// Returns the current output state.
	State operator()() const;

	// Sets the output to a specified state.
	void operator()(State);

private:
	pin_t pin_;	// The attached output pin.
};

class OutputToggleCommand : public ICommand // Command that toggles the state of a digital output.
{
public:
	// Assignment constructor.
	explicit OutputToggleCommand(DigitalOutput&);

	// Virtual destructor.
	virtual ~OutputToggleCommand();

public:
	// Toggles the state of the assigned output.
	virtual void execute() override;

protected:
	DigitalOutput& output_;	// The assigned output.
};

class OutputStateCommand : public OutputToggleCommand	// Command that sets the state of a digital output.
{
public:
	// Assignment constructor.
	OutputStateCommand(DigitalOutput& output, DigitalOutput::State state);

public:
	// Sets the output to the assigned state.
	void execute() override;

private:
	DigitalOutput::State state_;	// The assigned state.
};

#endif // !defined OUTPUTS_H__
