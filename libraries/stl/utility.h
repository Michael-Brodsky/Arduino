/*
 *	This file defines several types and functions in the C++ Standard Template 
 *	Library (STL) general utility library.
 *
 *	***************************************************************************
 *
 *	File: utility.h
 *	Date: June 9, 2021
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
 *  You should have received a std_copy of the GNU General Public License
 *	along with this file. If not, see <http://www.gnu.org/licenses/>.
 *
 *	**************************************************************************
 *
*	Description:
 *
 *		This file defines some of the objects in the <utility> header of
 *		a C++ Standard Template Library (STL) implementation. The objects
 *		behave according to the ISO C++11 Standard:	(ISO/IEC 14882:2011).
 *
 *		The Standard requires that STL objects reside in the `std' namespace.
 *		However, because later implementations of the Arduino IDE lack
 *		namespace support, this entire library resides in the global namespace
 *		and, to avoid naming collisions, all standard object names are
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

#if !defined UTILITY_H__
# define UTILITY_H__ 20210609L

# include <stddef.h>
# include "type_traits.h"

// IN ALGORITHM.H
//template<class ForwardIt, class ForwardIt> 
//inline void std_iter_swap(ForwardIt lhs, ForwardIt rhs)
//{	
//	std_swap(*lhs, *rhs);
//}

//template<class T>
//void std_swap(T& a, T& b) 
//{
//	T tmp = a;
//	a = b;
//	b = tmp;
//}

template<class T>
void std_swap(T& a, T& b)
{
	T tmp(std_move(a));
	a = std_move(b);
	b = std_move(tmp);
}

template<class T, size_t N>
void std_swap(T (&a)[N], T (&b)[N])
{
	if(a != b) {
		size_t i = 0;
		while(i < N)
		{
			std_swap(a[i], b[i]);
			++i;
		}
	}
}

template <class T>
inline T&& std_forward(typename std_remove_reference<T>::type& t) 
{
	return static_cast<T&&>(t);
}

template <class T>  
inline T&& std_forward(typename std_remove_reference<T>::type&& t)
{
	return static_cast<T&&>(t);
}

template <class T> 
inline typename std_remove_reference<T>::type&& 
	std_move(T&& t)
{
	return static_cast<typename std_remove_reference<T>::type&&>(t);
}

// In TYPE_TRAITS.H
//
//template<class T>
//typename std_add_rvalue_reference<T>::type declval();

template<class T> inline
typename std_remove_reference<T>::type&& move(T&& arg)
{
	return (typename std_remove_reference<T>::type&&)arg;
}

struct std_piecewise_construct_t { explicit std_piecewise_construct_t() = default; };

template<class T1, class T2>
struct std_pair
{
	typedef std_pair<T1, T2> self_type;
	typedef T1 first_type;
	typedef T2 second_type;

	std_pair() :
		first(), second()
	{	
	}

	std_pair(const T1& value_1, const T2& value_2) :
		first(value_1), second(value_2)
	{	
	}

	void swap(self_type& rhs)
	{	
		if (this != &rhs)
		{	
			std_swap(first, rhs.first);
			std_swap(second, rhs.second);
		}
	}

	self_type& operator=(const self_type& rhs)
	{	
		first = rhs.first;
		second = rhs.second;
		return *this;
	}

	template<class U, class V>
	self_type& operator=(const std_pair<U, V>& rhs)
	{	
		first = rhs.first;
		second = rhs.second;
		return *this;
	}

	T1 first;
	T2 second;
};

template<class T1, class T2> 
inline void std_swap(std_pair<T1, T2>& lhs, std_pair<T1, T2>& rhs)
{	
	lhs.swap(rhs);
}

template<class T1, class T2> 
inline bool operator==(const std_pair<T1, T2>& lhs, const std_pair<T1, T2>& rhs)
{	
	return lhs.first == rhs.first && lhs.second == rhs.second;
}

template<class T1, class T2> 
inline bool operator!=(const std_pair<T1, T2>& lhs, const std_pair<T1, T2>& rhs)
{	
	return !(lhs == rhs);
}

template<class T1, class T2> 
inline bool operator<(const std_pair<T1, T2>& lhs, const std_pair<T1, T2>& rhs)
{	
	return (lhs.first < rhs.first ||
			!(rhs.first < lhs.first) && lhs.second < rhs.second);
}

template<class T1, class T2> 
inline bool operator>(const std_pair<T1, T2>& lhs, const std_pair<T1, T2>& rhs)
{	
	return rhs < lhs;
}

template<class T1, class T2> 
inline bool operator<=(const std_pair<T1, T2>& lhs, const std_pair<T1, T2>& rhs)
{	
	return !(rhs < lhs);
}

template<class T1, class T2> 
inline bool operator>=(const std_pair<T1, T2>& lhs, const std_pair<T1, T2>& rhs)
{	
	return !(lhs < rhs);
}

template< class T1, class T2 >
std_pair<T1, T2> std_make_pair(T1 t1, T2 t2)
{
	return (std_pair<T1, T2>(t1, t2));
}

#endif // !defined UTILITY_H__