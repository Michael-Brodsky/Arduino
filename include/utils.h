/*
 *	This file defines several function-like macros for Win32 <--> Arduino 
 *	portability and convenience.

 *	***************************************************************************/

/*	File: utils.h
 *	Date: November 11, 2019
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

#if !defined UTILS_H__
#define UTILS_H__ 20191111L

#if defined _WIN32
 #include <iostream>
 #include <Windows.h>
#endif
#include <assert.h>
#include <types.h>
#include <tokens.h>

#if defined STRINGIZE
 #define Stringize(a) STRINGIZE(a)	// Converts a to "a".
#else
 #define Stringize(a) ((void)0)		// Converts a to "a".
#endif

#if defined CONCAT
 #define Concat(a, b) CONCAT(a, b)	// Concatenates a and b to ab.
#else
 #define Concat(a, b) ((void)0)		// Concatenates a and b to ab.
#endif

#if defined CONCAT3
 #define Concat3(a, b, c) CONCAT3(a, b, c)	// Concatenates a, b and c to abc.
#else
 #define Concat3(a, b, c) ((void)0)			// Concatenates a, b and c to abc.
#endif

#if !defined AssertMsg
 #define AssertMsg(x, msg) assert(((void) msg, (x))) // Prints `msg' on assertion failure of `x'.
#endif

#if !defined Print
 #if defined ARDUINO
  #define Print(val) Serial.print(val)		// Prints `val' to the serial monitor.
 #elif defined _WIN32 // defined ARDUINO
  #define Print(val) std::cout << (val)		// Prints `val' to the console.
 #else
  #define Print(val) ((void)0)	
 #endif               // defined _WIN32
#endif // !defined Print

#if !defined PrintLn
 #if defined ARDUINO
  #define PrintLn(val) Serial.println(val)		// Prints `val' with a trailing `newline' to the serial monitor.
 #elif defined _WIN32 // defined ARDUINO
  #define PrintLn(val) Print(val) << std::endl	// Prints `val' with a trailing `newline' to the console.
 #else
  #define PrintLn(val) ((void)0)	
 #endif               // defined _WIN32
#endif // !defined PrintLn

#if !defined _typename
 #if defined ARDUINO
  #define _typename						// WIN32 <--> Arduino portability macro.
 #elif defined _WIN32
  #define _typename typename			// WIN32 <--> Arduino portability macro.
 #endif
#endif // !defined _typename

#if defined ARDUINO
 #define GetTimerTick() millis()		// Implementation-specific function that returns the elapsed time in milliseconds.
#elif defined _WIN32 // defined ARDUINO
 #define GetTimerTick() GetTickCount()	// Implementation-specific function that returns the elapsed time in milliseconds.
#endif               // defined _WIN32
#if !defined GetTimerTick
 #define GetTimerTick() ((void)0)
#endif // !defined GetTimerTick

#if !defined ARDUINO
 #define F 
 #define PROGMEM 
#endif

#if !defined noreturn
 #if defined _WIN32
  #define noreturn __declspec(noreturn)
 #elif defined ARDUINO 
  #define noreturn __attribute__((noreturn))
 #elif (defined __cplusplus && __cplusplus >= 201103L)
  #define noreturn [[noreturn]] 
 #else
  #define noreturn
 #endif
#endif // !defined noreturn 

#if !defined noexcept
 #if defined __cplusplus 
  #if (__cplusplus < 201103L)	// noexcept operator defined by C++11
   #if defined _WIN32
    #define noexcept throw()
   #elif defined ARDUINO 
    #define noexcept __attribute__((nothrow))
   #endif
  #endif // __cplusplus < 201103L
 #endif // defined __cplusplus 
#endif // !defined noexcept 

#endif // !defined UTILS_H__
