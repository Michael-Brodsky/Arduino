/*
 *	This file defines several plain-old-data (POD) types that are useful for 
 *	adding context to type declarations and fixing type-correctness issues in 
 *	the Arduino API.

 *	***************************************************************************/

/*	File: types.h
 *	Date: November 3, 2019
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

#if !defined TYPES_H__
#define TYPES_H__ 20191103L

#include <stddef.h>
#include <stdint.h>

#if !(defined __PIN_T_DEFINED || defined pin_t)
 #if !defined __PIN_T_TYPE
  #define __PIN_T_TYPE uint8_t
 #endif
typedef __PIN_T_TYPE pin_t;		// Unsigned integral type that can hold any GPIO pin number.
 #define __PIN_T_DEFINED 1
#endif // !defined __PIN_T_DEFINED ...

static const pin_t InvalidPin = 0;

#if !defined __MSECS_T_DEFINED
 #if !defined __MSECS_T_TYPE
  #define __MSECS_T_TYPE unsigned long
 #endif
typedef __MSECS_T_TYPE msecs_t;	// Unsigned integral type that represents a time in milliseconds.
 #define __MSECS_T_DEFINED 1
#endif // !defined __MSECS_T_DEFINED

#if !defined __USECS_T_DEFINED
 #if !defined __MSECS_T_TYPE
  #define __MSECS_T_TYPE unsigned long
 #endif
typedef __MSECS_T_TYPE usecs_t;	// Unsigned integral type that represents a time in microseconds.
 #define __USECS_T_DEFINED 1
#endif // !defined __USECS_T_DEFINED

#if !defined __ANALOG_T_DEFINED
 #if !defined __ANALOG_T_TYPE
  #define __ANALOG_T_TYPE uint16_t
 #endif
typedef __ANALOG_T_TYPE analog_t;	// Unsigned integral type that represents an analog input level.
 #define __ANALOG_T_DEFINED 1
#endif // !defined __ANALOG_T_DEFINED

#if (defined __cplusplus && __cplusplus >= 201103L)
 #define __HAS_VARIADIC_TEMPLATES 1
#endif // defined __cplusplus && __cplusplus >= 201103L

#if (defined _WIN32 || (defined ARDUINO && ARDUINO <= 165))
 #define __HAS_NAMESPACES 1
#endif // defined _WIN32 || (defined ARDUINO && ARDUINO <= 165)

#endif // !defined TYPES_H__