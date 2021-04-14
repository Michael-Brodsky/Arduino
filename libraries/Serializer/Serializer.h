/*
 *	This file defines types and functions for serializing objects to the
 *	onboard EEPROM.
 *
 *	***************************************************************************
 *
 *	File: Serializer.h
 *	Date: April 13, 2021
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
 *	***************************************************************************
 *
 *	Description:
 *
 *		The `Serializer' class offers the convenience of tracking the current
 *		EEPROM read/write address so clients don't have to. This is useful
 *		in preventing address corruption when serializing many objects or
 *		large amounts of data.
 *
 *		`ISerializeable' is an abstract base type that serializable types
 *		should derive from. It declares two purely virtual methods:
 *		`serialize()' and `deserialize()' that must be implemented in derived
 *		types. The intent is to allow clients to define generic, untemplated
 *		methods to handle object serialization/deserialization that take any
 *		objects of type `ISerializeable' regardless of the derived object's
 *		actual type.
 *
 *		The `eepromGet()' function template offers an alternative to the
 *		Arduino API `EEPROM.get()' function by returning a value read from
 *		the EEPROM directly to the caller instead of having to pass a return
 *		variable as a parameter. This makes it useful, for instance,
 *		in class initializers. Note: template argument deduction is not
 *		possible, so each call must be instantiated explicitly for each type
 *		of `T':
 *
 *			time_t t = eepromGet<time_t>(addr);
 *
 *			class X {
 *			public:
 *				X(), time_(eepromGet<time_t>(0U)) {}
 *			private:
 *				time_t time_;
 *			};
 *
 *		The `eepromUpdate()' function template makes up for the Arduino API's
 *		lacks of a generic `EEPROM.update()' function, as it can only update
 *		one byte of data at a time. `eepromUpdate()' can update values of any
 *		type, like the Arduino `EEPROM.get()' and `EEPROM.put()' functions,
 *		with a single line of client code.
 *
 *		The `load() function overloads create and return either a new,
 *		deserialized objects or collection of objects of a specified type. The
 *		`store()' function overloads serialize either an object or collection
 *		of objects of a specified type to the onboard EEPROM.
 *
 *	***************************************************************************/

#if !defined SERIALIZER_H__
# define SERIALIZER_H__ 20210413L

# include <stddef.h>			// `size_t' type.
# include <EEPROM.h>			// Arduino EEPROM api.
# include "ISerializeable.h"	// `ISerializeable' interface.

// Returns the value from a specified memory address.
template <class T>
T eepromGet(size_t address)
{
	T temp;
	EEPROM.get(address, temp);
	return temp;
}

// Updates the value at a specified memory address if it differs from the given value.
template <class T>
void eepromUpdate(size_t address, const T& value)
{
	if (eepromGet<T>(address) != value)
		EEPROM.put(address, value);
}

// Type for serializing objects to/from non-valatile memory.
class Serializer
{
	static size_t addr_;	// The current memory read/write address.

public:
	Serializer() = default;
	~Serializer() = default;

public:
	// Returns the current read/write address.
	size_t address() const;
	// Resets the read/write address to 0.
	void reset();

	// Returns an object of type `T' from the current memory address 
	// and advances the address by the size of `T'.
	template <class T>
	T get()
	{
		size_t temp = addr_;
		addr_ += sizeof(T);
		return eepromGet<T>(temp);
	}

	// Reads and copies the value of an object of type `T' from the 
	// current memory address to the `value' parameter and advances 
	// the address by the size of `T'.
	template <class T>
	void get(T& value)
	{
		EEPROM.get(addr_, value);
		addr_ += sizeof(value);
	}

	// Writes the value of an object of type `T' from the supplied 
	// `value' parameter to current memory address and advances the 
	// address by the size of `T'.
	template <class T>
	void put(const T& value)
	{
		EEPROM.put(addr_, value);
		addr_ += sizeof(value);
	}

	// Writes the value of an object of type `T' from the supplied 
	// `value' parameter to current memory address and advances the 
	// address by the size of `T' only if the supplied `value' 
	// differs from the stored value at the current address.
	template <class T>
	void update(const T& value)
	{
		eepromUpdate(addr_, value);
		addr_ += sizeof(value);
	}

	// Creates and returns a new, deserialized collection of objects of type `T'.
	template <class T, size_t Size>
	void load(const T(&objects)[Size])
	{
		reset();
		for (auto it = objects; it < objects + Size; it++)
			load(*it);
	}

	// Creates and returns a new, deserialized object of type `T'.
	template <class T>
	T* load()
	{
		T* object = new T;

		static_cast<ISerializeable*>(object)->deserialize(*this);

		return object;
	}

	// Serializes a collection of objects of type `T' to the onboard EEPROM.
	template <class T, size_t Size>
	void store(const T(&objects)[Size])
	{
		reset();
		for (auto it = objects; it < objects + Size; it++)
			store(*it);
	}

	// Serializes an object of type `T' to the onboard EEPROM.
	template <class T>
	void store(const T& object)
	{
		static_cast<ISerializeable*>(object)->serialize(*this);
	}
};

#endif // !defined SERIALIZER_H__ 
