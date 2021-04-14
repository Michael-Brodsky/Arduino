/*
 *	This file defines a self-printing type for use with LCD displays.
 *
 *	***************************************************************************
 *
 *	File: Curses.h
 *	Date: April 9, 2021
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
 *	**************************************************************************
 *
 *	Description:
 * 
 *		The `Cursor' struct is a self-printing type capable of formatted 
 *		printing at a specified location on a liquid crystal display device.
 *		It is useful when multiple values must repeatedly be displayed on an 
 *		LCD screen in a specific format. Consider a digital clock that 
 *		displays the current date and time:
 * 
 *			01 Apr 2021
 *			Thu 12:00:00
 * 
 *		The print location and formating don't change, only the values do. 
 *		Clients can initialized Cursor objects at startup and simply call 
 *		them with the necessary value parameters.
 * 
 *		Print location and formating parameters, as well as a pointer to a 
 *		formatting buffer, are provided by the client at object construction 
 *		and are modifiable. `Cursor' uses a function call operator overload to 
 *		format and print the actual characters on the screen. 
 * 
 *	Examples:
 * 
 *		LiquidCrystal lcd(...);
 *		char buf[17U]; // Make this a big as required! Arduino lacks `snprintf()' support.
 * 
 *		Cursor date_format(0, 0, "%02d %s %4d", buf);		// Print formats for
 *		Cursor time_format(0, 1U, "%s %02d:%02d:%02d", buf); // clock example above. 
 *		...
 *		void myPrintFunction(day, month, year, dayOfWeek, hour, min, sec) { 
 *			date_format(lcd, day, month, year);
 *			time_format(dayOfWeek, hour, min, sec);
 *		}
 * 
 *		// Client calls myPrintFunction() when display needs to be refreshed.
 *
 *****************************************************************************/

#include <stdio.h>			// `sprintf()'     
#include "types.h"			// `stdint' types.
#include <LiquidCrystal.h>	// LCD device API.

// LCD formatted-printing type.
struct Cursor 
{
	uint8_t		col_;	// The cursor column.
	uint8_t		row_;	// The cursor row.
	const char* fmt_;	// `printf()' style format specifier.
	char*		buf_;	// Formatting buffer.

	explicit Cursor(unsigned col = 0, unsigned row = 0, const char* fmt = nullptr, char* buf = nullptr) :
		col_(col), row_(row), buf_(buf), fmt_(fmt) {}

	// Prints the given arguments on the LCD device.
	template<class ... Args>
	uint8_t operator()(LiquidCrystal& lcd, Args ... args)
	{
		lcd.setCursor(col_, row_);
		sprintf(buf_, fmt_, args ...);
		return lcd.print(buf_);
	}
};
