/*
 *	This file defines several STL-style functional algorithms.
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