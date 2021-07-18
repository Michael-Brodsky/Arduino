/*
 *	This file defines an abstract interface class for types that can be 
 *	serialized to/from the onboard EEPROM.
 * 
 *	***************************************************************************
 *
 *	File: ISerializeable.h
 *	Date: July 18, 2021
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
 *		`ISerializeable' is an abstract base type that serializable types 
 *		should derive from. It declares two purely virtual methods: 
 *		`serialize()' and `deserialize()' that must be implemented in derived 
 *		types. The intent is to allow clients to define generic, untemplated 
 *		methods to handle object serialization/deserialization that take any 
 *		objects of type `ISerializeable' regardless of the derived object's 
 *		actual type.
 * 
 *	***************************************************************************/

#if !defined ISERIALIZEABLE_H__
#define ISERIALIZEABLE_H__ 20210718L

class EEPROMStream;	// EEPROM stream type forward decl.

// Serializeable type abstract interface class.
struct ISerializeable
{
	virtual ~ISerializeable() = default;

	virtual void serialize(EEPROMStream&) const = 0;
	virtual void deserialize(EEPROMStream&) = 0;
};

#endif // !defined ISERIALIZEABLE_H__
