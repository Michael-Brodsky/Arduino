/*
 *	This file defines two types for serializing objects to and from the 
 *	onboard EEPROM.
 *	
 *	***************************************************************************
 *
 *	File: EEPROMStream.h
 *	Date: April 22, 2021
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
 *	The `EEPROMStream' class exposes several static methods for reading, 
 *	writing and updating objects to and from the onboard EEPROM buffer. Each of 
 *	these methods return the number of bytes read or written, allowing clients 
 *	to track the current EEPROM read/write position after each call. 
 * 
 *		float value = 42.0f;
 *		EEPROMStream::address_type address = 0;
 * 
 *		address_type += EEPROMStream::put(address, value);
 * 
 *	The class also exposes methods to serialize and deserializes individual or 
 *	collections of objects of type `ISerializeable'. Concrete objects of type  
 *	`ISerializeable' must be default constructible and implement the virtual 
 *	methods `serialize()' and `deserialize()' declared by the `ISerializeable' 
 *	interface. The concrete methods handle the object-specific serialization. The 
 *	`EEPROMStream::load' and `::store' methods take serializable objects or 
 *	collections as parameters and call each object's `serialize()' or 
 *	`deserialize()' method accordingly.
 * 
 *		class Foo : public ISerializeable 
 *		{
 *		public: 
 *			Foo() = default;
 *			Foo(int arg1, float arg2) : ISerializeable(), arg1_(arg1), arg2_(arg2) {}
 *		public:
 *			void serialize(EEPROMStream& stream) { stream << arg1_; stream << arg2_; }
 *			void deserialize(EEPROMStream& stream) { stream >> arg1_; stream >> arg2_; } 
 *		private:
 *			int arg1_;		// Member to serialize.
 *			float arg2_;	// Another member to serialize.	
 *		}
 * 
 *		Foo foo(42, 23.0f);
 *		EEPROMStream eeprom;
 *		eeprom.reset();		// Initialize read/write address to zero.
 *		eeprom.store(foo);	// Serializes (writes) object `foo' to EEPROM.
 *		...
 *		Foo bar;
 *		eeprom.reset();
 *		eeprom.load(bar);	// Deserializes (reads) object `bar' from EEPROM.
 *							// foo == bar.
 * 
 *	Object persistence is maintained by serializing objects whenever their 
 *	internal states change, and deserializing them whenever the device is 
 *	reset or power-cycled. Clients must serialize and deserialize objects in 
 *	the same order because `EEPROMStream' maintains the current EEPROM read/
 *	write address internally. The `EEPROMStream::reset' method is provided to 
 *	initialize the address to a known state.
 * 
 *	The stream insertion and extraction operators `EEPROMStream::operator<<' 
 *	and `::operator>>' work similarly to the C++ `std::iostream' operators except 
 *	that here, they read/write to and from the onboard EEPROM memory buffer 
 *	rather than standard i/o. Each operator call advances the `EEPROMStream' 
 *	internal address pointer according to the number of bytes read or written. 
 *	The insertion operator uses the `EEPROMStream::update' method to write to 
 *	the EEPROM memory buffer, thus data is only written if it differs from the 
 *	currently stored data.
 *
 *	***************************************************************************
 * 
 *	The `SerializableList' class template encapsulates a collection of objects 
 *	that are not of type `ISerializeable' and makes them serializable through 
 *	the `ISerializeable' interface. The class assigns client-defined 
 *	`serialize()' and `deserialize()' functions and calls these functions for 
 *	each object in the collection. The template parameter `T' specifies the 
 *	object type.
 * 
 *		struct Object {...} // Object doesn't derive from `ISerializeable'.
 * 
 *	Client defines `serialize()' and `deserialize()' for type `Object',   
 *	which take references to `EEPROMStream' and `Object' and read/write 
 *	members of type `Object' to and from the stream.
 * 
 *		void fserialize(EEPROMStream& s, const Object& obj) {...}
 *		void fdeserialize(EEPROMStream& s, Object& obj) {...}
 * 
 *	`SerializableList' stores collections of objects internally as type 
 *	`ArrayWrapper', a wrapper class for C-style arrays that behaves like 
 *	the STL `std::array' type, but without a fixed size (see <array.h>).
 *	Clients can assign a collection using the `SerializableList::list' 
 *	method or dynamically allocate memory to the internal list with the 
 *	`SerializableList::resize' method. This is useful when clients need  
 *	to modify the list size between serialize/deserialize operations. 
 *	Each call to `SerializableList::resize' destroys the previous list 
 *	contents. Unless the list size is fixed, it should be stored in the 
 *	EEPROM, usually before the first list object, and updated accordingly.
 * 
 *		EEPROMStream eeprom;
 *		SerializableList<Object> list(&fserialize, &fdeserialize);
 * 
 *		// Deserialize:
 *		eeprom.reset();
 *		SerializableList::address_type n = 0; // Holds the list size.
 *		eeprom >> n; // Read the list size from eeprom.
 *		list.resize(n); // Reallocate list storage, if necessary.
 *		eeprom.load(list);	// Deserialize objects in list.
 * 
 *		// Serialize:
 *		eeprom.reset();
 *		eeprom << list.size();	// Store the current list size.
 *		eeprom.store(list);	// Serialize objects in list.
 * 
 *	***************************************************************************/

#if !defined EEPROMSTREAM_H__ 
#define EEPROMSTREAM_H__ 20210422L

# include "library.h"			// Arduino API
# include "EEPROM.h"			// Arduino EEPROM api.
# include "array.h"				// `ArrayWrapper' type, `std_begin()' & `std_end()'.
# include "ISerializeable.h"	// `ISerializeable' interface.

// Type that serializes objects to and from the onboard EEPROM.
class EEPROMStream
{
public:
	using address_type = unsigned;	// Arduino EEPROM addressing type alias.
	using size_type = unsigned;		// Serializable object size type.

public:
	EEPROMStream() = default;

public:
	// EEPROM stream insertion operator.
	template<class T>
	EEPROMStream& operator<<(const T&);
	// EEPROM stream extraction operator.
	template<class T>
	EEPROMStream& operator>>(T&);
	// Returns a mutable reference to the current read/write address.
	address_type& address();
	// Returns a immutable reference to the current read/write address.
	const address_type& address() const;
	// Resets the EEPROM read/write address to zero.
	void reset();
	// Deserializes a collection of objects of type `T' from the EEPROM.
	template<class T, size_type N>
	void load(T(&t)[N]);
	// Deserializes a collection of objects of type `T' from the EEPROM.
	template<class T>
	void load(T t[], size_type n);
	// Deserializes a collection of objects of type `T' from the EEPROM.
	template<class T>
	void load(T* first, T* last);
	// Deserializes an object of type `T' from the EEPROM.
	template<class T>
	void load(T& t);
	// Serializes a collection of objects of type `T' to the EEPROM.
	template<class T, size_type N>
	void store(const T(&t)[N]);
	// Serializes a collection of objects of type `T' to the EEPROM.
	template<class T>
	void store(const T t[], size_type n);
	// Serializes a collection of objects of type `T' to the EEPROM.
	template<class T>
	void store(const T* first, const T* last);
	// Serializes an object of type `T' to the EEPROM.
	template<class T>
	void store(const T& t);

public:
	// Reads the value of an object of type `T' from the EEPROM at the given address. 
	template<class T>
	static address_type	get(address_type, T&);
	// Reads the value of a `String' object from the EEPROM at the given address. 
	static address_type	get(address_type, String&);
	// Reads the value of a c-string object from the EEPROM at the given address. 
	static address_type	get(address_type, char*);
	// Writes the value of an object of type `T' to the EEPROM at the given address. 
	template<class T>
	static address_type	put(address_type, const T&);
	// Writes the value of a `String' object to the EEPROM at the given address. 
	static address_type	put(address_type, const String&);
	// Writes the value of a c-string object to the EEPROM at the given address. 
	static address_type	put(address_type, const char*);
	// Writes the value of an object of type `T' to the EEPROM at the given address 
	// if it differs from the currently stored value at that address.
	template<class T>
	static address_type	update(address_type, const T&);

private:
	address_type address_;	// The current EEPROM read/write address.
};

#pragma region EEPROMStream
template<class T>
EEPROMStream& EEPROMStream::operator<<(const T& t)
{
	address_ += update(address_, t);

	return *this;
}

template<class T>
EEPROMStream& EEPROMStream::operator>>(T& t)
{
	address_ += get(address_, t);

	return *this;
}

template<class T, EEPROMStream::size_type N>
void EEPROMStream::load(T(&t)[N])
{
	for (size_type i = 0; i < N; ++i)
		load(t[i]);
}

template<class T>
void EEPROMStream::load(T t[], size_type n)
{
	for (size_type i = 0; i < n; ++i)
		load(t[i]);
}

template<class T>
void EEPROMStream::load(T* first, T* last)
{
	for (auto it = first; it < last; ++it)
		load(*it);
}

template<class T>
void EEPROMStream::load(T& t)
{
	T* p = &t;

	static_cast<ISerializeable*>(p)->deserialize(*this);
}

template<class T, EEPROMStream::size_type N>
void EEPROMStream::store(const T(&t)[N])
{
	for (size_type i = 0; i < N; ++i)
		store(t[i]);
}

template<class T>
void EEPROMStream::store(const T t[], size_type n)
{
	for (size_type i = 0; i < n; ++i)
		store(t[i]);
}

template<class T>
void EEPROMStream::store(const T* first, const T* last)
{
	for (auto it = first; it < last; ++it)
		store(*it);
}

template<class T>
void EEPROMStream::store(const T& t)
{
	const T* p = &t;

	static_cast<const ISerializeable*>(p)->serialize(*this);
}

template<class T>
EEPROMStream::address_type EEPROMStream::get(address_type address, T& value)
{
	EEPROM.get(address, value);

	return sizeof(value);
}

template<class T>
EEPROMStream::address_type EEPROMStream::put(address_type address, const T& value)
{
	EEPROM.put(address, value);

	return sizeof(value);
}

template <class T>
EEPROMStream::address_type EEPROMStream::update(address_type address, const T& value)
{
	T t;
	address_type n = get(address, t);

	if (t != value)
		n = put(address, value);

	return n;	// Return the number of bytes read, or written if the update occured.
}
#pragma endregion

#endif // !defined EEPROMSTREAM_H__ 