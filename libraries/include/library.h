/*
 *  This file includes one of several library dependencies that expose the 
 *  Arduino API to the client.
 *
 *	***************************************************************************
 *
 *	File: library.h
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
 *  This file may include the following C++ header files:
 *
 *      Arduino.h or WProgram.h: Arduino API declarations, depending on the 
 *                               Arduino IDE implementation being used.
 *                      types.h: Arduino POD types, constants and conditional-
 *                               compilation macros.      
 */

#if !defined LIBRARY_H__
# define LIBRARY_H__ 20210310L

# if defined ARDUINO
#  if ARDUINO >= 100 
#   include "Arduino.h"
#  else
#   include "WProgram.h"
#  endif // ARDUINO >= 100 
# endif // defined ARDUINO

# include "types.h"

#endif // !defined LIBRARY_H__
