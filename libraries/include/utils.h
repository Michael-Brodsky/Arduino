/*
 *	This file defines several function-like macros for MSVC11 <--> Arduino IDE 
 *	1.8 portability and convenience.
 *
 *	***************************************************************************
 *
 *	File: utils.h
 *	Date: November 11, 2019
 *	Version: 0.99
 *	Author: Michael Brodsky
 *	Email: mbrodskiis@gmail.com
 *	Copyright (c) 2012-2020 Michael Brodsky 
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

#if !defined UTILS_H__
# define UTILS_H__ 20191111L

# include <assert.h>
# include <string.h>
# include "tokens.h"
# include "library.h"

# if defined STRINGIZE
#  define Stringize(a) STRINGIZE(a)	// Converts a to "a".
# else
#  define Stringize(a) ((void)0)	// Converts a to "a".
# endif

# if defined CONCAT
#  define Concat(a, b) CONCAT(a, b)	// Concatenates a and b to ab.
# else
#  define Concat(a, b) ((void)0)	// Concatenates a and b to ab.
# endif

# if defined CONCAT3
#  define Concat3(a, b, c) CONCAT3(a, b, c)	// Concatenates a, b and c to abc.
# else
#  define Concat3(a, b, c) ((void)0)		// Concatenates a, b and c to abc.
# endif

# if !defined AssertMsg
#  define AssertMsg(x, msg) assert(((void) msg, (x))) // Prints `msg' on assertion failure of `x'.
# endif

# if !defined Print
#  define Print(val) Serial.print(val)		// Prints `val' to the serial monitor.
# endif // !defined Print

# if !defined PrintLn
#  define PrintLn(val) Serial.println(val)	// Prints `val' with a trailing `newline' to the serial monitor.
# endif // !defined PrintLn

# define NoTimer0Int TIMSK0 &= ~_BV(OCIE0A);					// Disable Timer0 cmp interrupt.
# define Timer0Int(val) OCR0A = (val); TIMSK0 |= _BV(OCIE0A);	// Sets Timer0 cmp interrupt to trigger on `val'.

void(*resetFunc)(void) = 0;	// Reboots the Arduino device.

#ifdef __arm__
// should use uinstd.h to define sbrk but Due causes a conflict
extern "C" char* sbrk(int incr);
#else  // __ARM__
extern char* __brkval;
#endif  // __arm__

// Returns the amount of free heap memory.
int freeMemory() 
{
	char top;
#ifdef __arm__
	return &top - reinterpret_cast<char*>(sbrk(0));
#elif defined(CORE_TEENSY) || (ARDUINO > 103 && ARDUINO != 151)
	return &top - __brkval;
#else  // __arm__
	return __brkval ? &top - __brkval : &top - __malloc_heap_start;
#endif  // __arm__
}

// `strcat()' for chars.
void charcat(char* buf, char c)
{
	char* ptr = buf + strlen(buf);

	*ptr = c;
	*(++ptr) = '\0';
}
#endif // !defined UTILS_H__
