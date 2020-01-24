/*
 *	This file defines types and functions for serializing objects.

 *	***************************************************************************/

/*	File: ISerializeable.h
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

#if !defined ISERIALIZEABLE_H__
#define ISERIALIZEABLE_H__ 20200120L

#include <EEPROM.h>	// EEPROM read/write support.

static size_t eeprom_addr = 0;

struct ISerializeable		// Serializeable type abstract interface class.
{
	virtual ~ISerializeable() { }

	virtual void serialize() = 0;
        virtual void deserialize() = 0;
};

template <class T>
void eepromGet(T& value)	// Reads data of type `T' from the EEPROM and assigns it to `value'.
{
	EEPROM.get(eeprom_addr, value);
	eeprom_addr += sizeof(T);
}

template <class T>
void eepromPut(T& value)	// Writes `value' as data of type `T' to the EEPROM.
{
	EEPROM.put(eeprom_addr, value);
	eeprom_addr += sizeof(T);
}

#endif // !defined ISERIALIZEABLE_H__
