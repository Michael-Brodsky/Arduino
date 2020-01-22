/*
 *	This file defines an abstract interface class for asynchrounous, event-
 *	driven objects.

 *	***************************************************************************/

/*	File: IClockable.h
 *	Date: January 20, 2020
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

#if !defined ICLOCKABLE_H__
#define ICLOCKABLE_H__ 20200120L

struct IClockable	// Asynhronous object abstract interface class.
{
	virtual ~IClockable() {}

	virtual void clock() = 0;
};

template <class T>
static IClockable* iclockable(T& object) // Converts a pointer of a derived type to a pointer of type IClockable.
{
	return static_cast<IClockable*>(&object);
}

template <class T>
static void iclock(T& object)			// Calls a derived type's `clock()' implementation.
{
	iclockable(object)->clock();
}

#endif // !defined ICLOCKABLE_H__