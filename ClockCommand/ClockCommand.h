/*
 *	This file defines a class for executing asynchronous "clock" commands.
 
 *	***************************************************************************/

/*	File: ClockCommand.h
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
 *	This file defines a concrete class `ClockCommand' that clients can use to 
 *	execute asynchronous "clock" commands on objects of type `IClockable'. 
 *	`ClockCommand' simplifies definition of "clock" command objects to one 
 *	line of code.
 *
 *	Examples:
 *
 *		class Object : public IClockable 
 *		{
 *		private:
 *			void clock() override { ... } // Only accessible through interface.
 *		};
 *
 *		Object obj1;
 *			...
 *		ClockCommand cmd(obj1);
 *			...
 *		cmd.execute();
 */

#if !defined CLOCKCOMMAND_H__
#define CLOCKCOMMAND_H__ 20191119L

#include <IClockable.h>	// IClockable interface class.
#include <ICommand.h>	// ICommand interface class.

struct ClockCommand : public Command<void, IClockable>
{
	typedef Callback<void, IClockable>::callback_t callback_t;

	explicit ClockCommand(IClockable& receiver) : 
		Command(&receiver, static_cast<callback_t>(&IClockable::clock)) { }

	virtual ~ClockCommand() { }
};

#endif // !defined CLOCKCOMMAND_H__