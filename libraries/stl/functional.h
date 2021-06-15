/*
 *	This file defines several C++ Standard Template Library (STL) functional 
 *	algorithms.
 *
 *	***************************************************************************
 *
 *	File: functional.h
 *	Date: March 15, 2021
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
 *  ***************************************************************************
 *
 *	Description:
 *
 *		This file defines some of the functions in the <functional> header of
 *		a C++ Standard Template Library (STL) implementation. The functions 
 *		behave according to the ISO C++11 Standard:	(ISO/IEC 14882:2011).
 *
 *		The Standard requires that STL objects reside in the `std' namespace.
 *		However, because later implementations of the Arduino IDE lack
 *		namespace support, this entire library resides in the global namespace
 *		and, to avoid naming collisions, all standard function names are
 *		preceded by `std_'. Thus, for example:
 *
 *			std::find = std_find,
 *			std::begin = std_begin,
 *			std::end = std_end,
 *
 *		and so forth. Otherwise function names are identical to those defined
 *		by the Standard.
 *
 *	**************************************************************************/

#if !defined FUNCTIONAL_H__
#define FUNCTIONAL_H__ 20210315L

#include <stddef.h>

template<class T>
struct std_divides 
{
	T operator()(const T &lhs, const T &rhs) const 
	{
		return lhs / rhs;
	}
};

template<class T>
struct std_equal_to 
{
	bool operator()(const T &lhs, const T &rhs) const 
	{
		return lhs == rhs;
	}
};

template<class T>
struct std_greater 
{
	bool operator()(const T &lhs, const T &rhs) const 
	{
		return lhs > rhs;
	}
};

template<class T>
struct std_greater_equal 
{
	bool operator()(const T &lhs, const T &rhs) const 
	{
		return lhs >= rhs;
	}
};

template<class T>
struct std_less 
{
	bool operator()(const T &lhs, const T &rhs) const 
	{
		return lhs < rhs;
	}
};

template<class T>
struct std_less_equal 
{
	bool operator()(const T &lhs, const T &rhs) const 
	{
		return lhs <= rhs;
	}
};

template<class T>
struct std_logical_and
{
	bool operator()(const T &lhs, const T &rhs) const 
	{
		return lhs && rhs;
	}
};

template<class T>
struct std_logical_or
{
	bool operator()(const T &lhs, const T &rhs) const 
	{
		return lhs || rhs;
	}
};

template<class T>
struct std_logical_not
{
	bool operator()(const T &arg) const 
	{
		return !arg;
	}
};

template<class T>
struct std_minus
{
	T operator()(const T &lhs, const T &rhs) const 
	{
		return lhs - rhs;
	}
};

template<class T>
struct std_modulus
{
	T operator()(const T &lhs, const T &rhs) const 
	{
		return lhs % rhs;
	}
};

template<class T>
struct std_multiplies
{
	T operator()(const T &lhs, const T &rhs) const 
	{
		return lhs * rhs;
	}
};

template<class T>
struct std_negate
{
	T operator()(const T &arg) const 
	{
		return -arg;
	}
};

template<class T>
struct std_not_equal_to
{
	bool operator()(const T &lhs, const T &rhs) const 
	{
		return lhs != rhs;
	}
};

template<class T>
struct std_plus
{
	T operator()(const T &lhs, const T &rhs) const 
	{
		return lhs + rhs;
	}
};

#endif // !defined FUNCTIONAL_H__