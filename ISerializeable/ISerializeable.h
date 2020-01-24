/*
 *	This file defines types and functions for implementing serializeable 
 *  objects.

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
 *  This file defines an abstract interface class `ISerializeable' and three 
 *  function templates, eepromGet(), eepromPut() and eepromUpdate() for 
 *  reading and writing data to the on-board eeprom. ISerializeable declares 
 *  two virtual functions: 
 *
 *      void serialize(), and
 *      void deserialize()
 *
 *  which must be implemented in derived classes. Implementations should write
 *  any appropriate data in their `serialize()' functions and read data in 
 *  their `deserialize()' functions.
 *
 *  A static variable `eeprom_addr' holds the current eeprom read/write address 
 *  and is advanced automatically by the function templates on each eeprom 
 *  operation. IMPORTANT: `eeprom_addr' must be reset to its initial starting 
 *  value between read and write operations, otherwise the next operation will 
 *  occur past the end of the region where the data is stored (the function 
 *  templates only advance `eeprom_addr').
 *
 *  Examples:
 *
 *  Objects that are not part of an inheritance hierarchy and don't contain 
 *  pointers or other objects can be "trivially" serialized/deserialized with 
 *  the `this' pointer:
 *
 *      struct Object : public ISerializeable 
 *      {
 *          size_t size_;
 *          float value_;
 *          void serialize() override { eepromPut(*this); }
 *          void deserialize() override { eepromGet(*this); }
 *      };
 */

#if !defined ISERIALIZEABLE_H__
#define ISERIALIZEABLE_H__ 20200120L

#include <EEPROM.h>	// EEPROM read/write support.

static size_t eeprom_addr = 0;

struct ISerializeable		// Serializeable type abstract interface class.
{
	virtual ~ISerializeable() { }

	virtual void serialize() const = 0;
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

template <class T>
void eepromUpdate(T& value)	// Writes `value' as data of type `T' to the EEPROM if `value' differs from the current content.
{
    T current;              // `T' must be default-constructable.
    EEPROM.get(eeprom_addr, current);
	if(current != value)    // `T' must be equal-comparable.
        eepromPut(value);
}
#endif // !defined ISERIALIZEABLE_H__
