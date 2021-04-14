/*
 *	This file defines several types for dealing with STL-style iterators.
 *
 *	***************************************************************************
 *
 *	File: iterator.h
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

#if !defined ITERATOR_H__
# define ITERATOR_H__ 20210315L 

# include <stddef.h>

# pragma region std_iterator_traits

struct std_input_iterator_tag {};
struct std_output_iterator_tag {};
struct std_forward_iterator_tag : public std_input_iterator_tag {};
struct std_bidirectional_iterator_tag : public std_forward_iterator_tag {};
struct std_random_access_iterator_tag : public std_bidirectional_iterator_tag {};

// std_iterator - iterator base type used to simplify definition of the 
// required types for iterators. All iterator classes should derive from
// this type.

template<class Category, class T, class Distance = ptrdiff_t, class Pointer = T*, class Reference = T&>
struct std_iterator // iterator base type.
{
	typedef Category iterator_category;
	typedef T value_type;
	typedef Distance difference_type;
	typedef Pointer pointer;
	typedef Reference reference;
};

// std_iterator_traits - type trait class providing a uniform interface to 
// the properties of iterator types.

template<class It>					// `It' - the iterator type for which to receive properties.			
struct std_iterator_traits				// Iterator type trait class.
{
	typedef typename It::value_type value_type;
	typedef typename It::difference_type difference_type;
	typedef typename It::iterator_category iterator_category;
	typedef typename It::pointer pointer;
	typedef typename It::reference reference;
};

template<class T>					// `T' - pointer type
struct std_iterator_traits<T*>			// std_iterator_traits specialization for pointer types.	
{ 
	typedef T value_type;
	typedef ptrdiff_t difference_type;
	typedef std_random_access_iterator_tag iterator_category;
	typedef T* pointer;
	typedef T& reference;
};

template<class T>					// `T' - pointer type.
struct std_iterator_traits<const T*>	// std_iterator_traits specialization for const pointer types.
{ 
	typedef T value_type;
	typedef ptrdiff_t difference_type;
	typedef std_random_access_iterator_tag iterator_category;
	typedef const T* pointer;
	typedef const T& reference;
};

#pragma endregion

#pragma region iterator_related_functions

// Returns address of `val' even if the `address of' operator (&) is overridden.
template<class T> inline
T* std_addressof(T& val) 
{
	return (reinterpret_cast<T*>((&const_cast<char&>(reinterpret_cast<const volatile char&>(val)))));
}

// Returns the address of the data pointed to by iterator `it'.
template<class InputIt> inline 
typename std_iterator_traits<InputIt>::pointer
std_address_of(InputIt it)	
{
	return std_addressof(*it);
}

// Returns `true' if iterator `it' is dereferencable, else returns `false'.
template<class InputIt> inline
bool std_is_dereferenceable(InputIt it)	
{
	return std_addressof(*it) != nullptr;
}

// Returns the distance between two iterators `first' and `last'.
template<class InputIt> inline
typename std_iterator_traits<InputIt>::difference_type 
std_distance(InputIt first, InputIt last) 
{
	return std_addressof(*last) - std_addressof(*first);	// For iterator categories that don't support unary offset operations.
}

namespace // Should be a nested namespace.
{
	// `advance()' specialization for random access iterators.
	template<class InputIt, class Distance>
	inline void advance_impl(InputIt& it, Distance n, std_random_access_iterator_tag)
	{	
		it += n;
	}

	// `advance()' specialization for bidirectional iterators.
	template<class InputIt, class Distance>
	inline void advance_impl(InputIt& it, Distance n, std_bidirectional_iterator_tag)
	{	
		if(n < 0) 
			while(n++) 
				--it;
		else 
			while(n--) 
				++it;
	}

	// `advance()' specialization for input, output and forward iterators.
	template<class InputIt, class Distance>
	inline void advance_impl(InputIt& it, Distance n, std_input_iterator_tag)
	{	
		if(n < 0) 
			n = -n;
		while(n--) 
			++it;
	}
}

// Increments iterator `it' by `n' elements.
template<class InputIt, class Distance>
void std_advance(InputIt& it, Distance n) 
{	
	advance_impl(it, n, typename std_iterator_traits<InputIt>::iterator_category()); 
}

// Returns the `n-th' successor of iterator `it'.
template<class ForwardIt>
ForwardIt std_next(ForwardIt it, typename std_iterator_traits<ForwardIt>::difference_type n = 1) 
{
	advance(it, n);
	return it;
}

// Returns the `n-th' predecessor of iterator `it'.
template<class BidirIt>
BidirIt std_prev(BidirIt it, typename std_iterator_traits<BidirIt>::difference_type n = 1)
{
	advance(it, -n);
	return it;
}

// Returns an iterator to the first element of container `c'.
template<class C>
auto std_begin(C& c) -> decltype(c.begin())
{
	return c.begin();
}

// Returns a pointer to the first element of array `arr'.
template<class T, size_t Size> 
inline T* std_begin(T (&arr)[Size])
{	
	return (arr);
}

// Returns an iterator to one past the last element of container `c'.
template<class C>
auto std_end(C& c) -> decltype(c.end())
{
	return c.end();
}

// Returns a pointer to one past the last element of array `arr'.
template<class T, size_t Size> 
inline T* std_end(T (&arr)[Size])
{	
	return (arr + Size);
}

#pragma endregion

#endif // !defined ITERATOR_H__