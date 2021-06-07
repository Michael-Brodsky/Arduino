/*
 *	This file defines several plain-old-data (POD) types and constants that are 
 *	useful for adding context to type declarations and fixing type-correctness 
 *	issues in the Arduino API, and several object-like macros used in 
 *  conditional compilation.
 *
 *	***************************************************************************
 *
 *	File: types.h
 *	Date: March 10, 2021
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
 *  Description:
 *
 *  This file declares the following types:
 *
 *        pin_t:  An unsigned integral type that holds any GPIO pin number.
 *      msecs_t:  An unsigned integral type that represents a time in 
 *                milliseconds.
 *      usecs_t:  An unsigned integral type that represents a time in 
 *                microseconds.
 *     analog_t:  An unsigned integral type that represents an analog input 
 *                level (as converted to a digital value (see e.g. 
 *                `analogRead()').
 *
 *  This file defines the following constants:
 *
 *      InvalidPin: constant indicating an invalid digital GPIO pin number.
 *
 *  This file defines the following object-like macros:
 *
 *					  ANALOG_MAX: Maximum value an object of type `analog_t' 
 *								  can hold.
 *      __HAS_VARIADIC_TEMPLATES: defined if the implementation supports 
 *                                variadic template arguments.
 *              __HAS_NAMESPACES: defined if the implementation supports 
 *                                named namespaces.
 */

#if !defined TYPES_H__
# define TYPES_H__ 20210310L

# include <stddef.h>
# include <stdint.h>

# if !(defined __PIN_T_DEFINED || defined pin_t)
#  if !defined __PIN_T_TYPE
#   define __PIN_T_TYPE uint8_t
#  endif
typedef __PIN_T_TYPE pin_t;		    // Unsigned integral type that can hold any GPIO pin number.
#  define __PIN_T_DEFINED 1
# endif // !defined __PIN_T_DEFINED ...

static const pin_t InvalidPin = 0;  // Constant indicating an invalid digital GPIO pin number.

# if !defined __MSECS_T_DEFINED
#  if !defined __MSECS_T_TYPE
#   define __MSECS_T_TYPE unsigned long
#  endif
typedef __MSECS_T_TYPE msecs_t;	    // Unsigned integral type that represents a time in milliseconds.
#  define __MSECS_T_DEFINED 1
# endif // !defined __MSECS_T_DEFINED

# if !defined __USECS_T_DEFINED
#  if !defined __MSECS_T_TYPE
#   define __MSECS_T_TYPE unsigned long
#  endif
typedef __MSECS_T_TYPE usecs_t;	    // Unsigned integral type that represents a time in microseconds.
#  define __USECS_T_DEFINED 1
# endif // !defined __USECS_T_DEFINED

# if !defined __ANALOG_T_DEFINED
#  if !defined __ANALOG_T_TYPE
#   define __ANALOG_T_TYPE uint16_t
#  endif
typedef __ANALOG_T_TYPE analog_t;	// Unsigned integral type that represents an analog input level.
#  define __ANALOG_T_DEFINED 1 
#  if defined ANALOG_MAX 
#   undef ANALOG_MAX
#  endif
#  define ANALOG_MAX 1023U			// Maximum value an object of type `analog_t' should hold.
# endif // !defined __ANALOG_T_DEFINED

# if (defined __cplusplus && __cplusplus >= 201103L)
#  define __HAS_VARIADIC_TEMPLATES 1
# endif // defined __cplusplus && __cplusplus >= 201103L

# if (defined _WIN32 || (defined ARDUINO && ARDUINO <= 165))
#  define __HAS_NAMESPACES 1
# endif // defined _WIN32 || (defined ARDUINO && ARDUINO <= 165)

# if !defined _typename
#  if defined ARDUINO
#   define _typename					// WIN32 <--> Arduino portability macro.
#  elif defined _WIN32
#   define _typename typename			// WIN32 <--> Arduino portability macro.
#  endif
# endif // !defined _typename

#endif // !defined TYPES_H__
