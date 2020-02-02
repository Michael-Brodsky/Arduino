/*
 *	This file defines a type for printing formatted text on an lcd device. 
 
 *	***************************************************************************/

/*	File: LCD.h
 *	Date: February 2, 2020
 *	Version: 0.99
 *	Author: Michael Brodsky
 *	Email: mbrodskiis@gmail.com
 *	Copyright (c) 2012-2020 Michael Brodsky */

/*	***************************************************************************
 *
 *	This file is part of "Pretty Good" (Pg). "Pg" is free software:
 *	you can redistribute it and/or modify it under the terms of the
 *	GNU General Public License as published by the Free Software Foundation,
 *	either version 3 of the License, or (at your option) any later version.
 *
 *	This file is distributed in the hope that it will be useful, but
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
 *	The `LCD' type defines two nested types, `Position' and `Cursor', that  
 *	encapsulate printing position and formatting information, and print 
 *	specified text on a `LiquidCrystal' object (see Arduino LiquidCrystal 
 *	library). 
 *
 *	The `Position' type simply encapsulates print position information as a 
 *	`row' and `column' (i.e. a cursor position). It can be used as an 
 *	argument to the `Cursor' type's constructor but is not strictly required.
 *
 *	The `Cursor' type aggregates position with formatting information and 
 *	prints specified text to an object of type `Liquid Crystal' using the 
 *	function call operator `operator()'. The function call operator is a 
 *	variadic function template that accepts parameter packs of zero or more 
 *	arguments that are expanded and passed to `snprintf()' for formatting.  
 *	Thus your formatting strings must conform to those used by the C 
 *	`sprintf()' or `printf()' functions.
 *
 *	Examples:
 *
 *		LCD::Cursor cursor(0, 0, "(%s)");	// Constructs a Cursor object that
 *											// that prints a string in 
 *											// parentheses at column 0, row 0.
 *		... or ...
 *
 *		LCD::Position position = { 0, 0 };	
 *		LCD::Cursor cursor(position, "(%s)");
 *		...
 *		LiquidCrystal lcd(...);
 *		...
 *		cursor(lcd, "Hello");				// Prints (Hello) on object `lcd' 
 *											// which must be of type `LiquidCrystal'
 *
 *		***** Simple sketch to print formatted time *****
 *
 *		#include <LCD.h>
 *		#include <TimeLib.h>	// Arduino time library.
 *
 *		const char str_fmt[] = "%02d:%02d:%02d";  // Print format is "dd:dd:dd"
 *		LCD::Position position = { 4, 0 };	      // Print position: col=4, row=0
 *		LCD::Cursor cursor(position, str_fmt);	  // Cursor object
 *		LiquidCrystal lcd(...);					  // LiquidCrystal object.
 *
 *		void setup()
 *		{
 *			lcd.begin(16, 2);	// Assuming a 16x2 LCD display device.
 *		}
 *
 *		void loop()
 *		{
 *			setTime(millis() / 1000UL);
 *			cursor(lcd, hour(), minute(), second()); // Prints `millis()' as hh::mm::ss
 *													 // centered on top row of lcd
 *			delay(1000);							 // every second.
 *		}
 *
 */

#if !defined LCD_H__
# define LCD_H__ 20200202L

# include "library.h"		// Arduino lib dependencies.
# include <stdio.h>			// snprintf()
# include <LiquidCrystal.h>	// Arduino LiquidCrystal lib.

struct LCD	// Type that prints formatted text on an lcd device.
{
	static const uint8_t PrintBufferSize = 16;	// Print buffer size in chars.
												// This can be adjusted as needed.

	struct Position		// Print position type.
	{
		const uint8_t	col_;	// Column number.
		const uint8_t	row_;	// Row number.
	};

	struct Cursor		// Self-printing text type.
	{
		const uint8_t	col_;	// Column number where printing starts.
		const uint8_t	row_;	// Row number where printing starts.
		const char*		fmt_;	// Print formatting string.

		/*** Constructors ***/
		Cursor() :
			col_(), row_(), fmt_() { }
		Cursor(uint8_t col, uint8_t row, const char* fmt = nullptr) :
			col_(col), row_(row), fmt_(fmt) { }
		Cursor(LCD::Position pos, const char* fmt = nullptr) :
			col_(pos.col_), row_(pos.row_), fmt_(fmt) { }

		template<class ... Args>
		uint8_t operator()(LiquidCrystal& lcd, const Args& ... args) 
		{	// Prints formatted `args' on the specified `lcd' object.
			char buf[PrintBufferSize];

			lcd.setCursor(col_, row_);
			(void)snprintf(buf, PrintBufferSize, fmt_, args ...);

			return lcd.print(buf);
		}
	};
};

#endif // !defined LCD_H__
