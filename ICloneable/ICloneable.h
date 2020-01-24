/*
 *  This file defines an abstract interface class for creating/cloning 
 *  objects using the C++ "Virtual Constructor Idiom".
 
 *	***************************************************************************/

/*	File: ICloneable.h
 *	Date: January 24, 2020
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

#if !defined ICLONEABLE_H__
#define ICLONEABLE_H__ 20200124L

struct ICloneable 
{
    virtual ~ICloneable() { }
    
    virtual ICloneable* clone() const = 0;  // Uses the copy constructor
    virtual ICloneable* create() const = 0; // Uses the default constructor
};

#endif // !defined ICLONEABLE_H__

