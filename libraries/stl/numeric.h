/*
 *	This file defines several from the C++ Standard Template Library (STL) 
 *  numerics library.
 *
 *	***************************************************************************
 *
 *	File: numeric.h
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
 *  You should have received a copy of the GNU General Public License
 *	along with this file. If not, see <http://www.gnu.org/licenses/>.
 *
 *	***************************************************************************
 *
 *	Description:
 *
 *		This file defines some of the functions in the <numeric> header of
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

#if !defined NUMERIC_H__
# define NUMERIC_H__ 20210609L

# include "iterator.h"

template<class InputIt, class T>
T std_accumulate(InputIt first, InputIt last, T init)
{
    for (; first != last; ++first) 
        init = init + *first; 

    return init;
}

template<class InputIt, class T, class BinaryOperation>
T std_accumulate(InputIt first, InputIt last, T init, BinaryOperation op) 
{
    for (; first != last; ++first) 
        init = op(init, *first);

    return init;
}

template<class ForwardIt, class T>
void std_iota(ForwardIt first, ForwardIt last, T value)
{
    while (first != last) 
    {
        *first++ = value;
        ++value;
    }
}

template<class InputIt1, class InputIt2, class T>
T std_inner_product(InputIt1 first1, InputIt1 last1, InputIt2 first2, T init)
{
    while (first1 != last1) 
    {
        init = init + *first1 * *first2;
        ++first1;
        ++first2;
    }

    return init;
}

template<class InputIt1, class InputIt2, class T, class BinaryOperation1, class BinaryOperation2>
    T std_inner_product(InputIt1 first1, InputIt1 last1, InputIt2 first2, T init,
        BinaryOperation1 op1, BinaryOperation2 op2)
{
    while (first1 != last1) 
    {
        init = op1(init, op2(*first1, *first2)); 
        ++first1;
        ++first2;
    }

    return init;
}

template< class InputIt, class OutputIt >
OutputIt std_adjacent_difference(InputIt first, InputIt last, OutputIt d_first)
{
    if (first == last) return d_first;

    typedef typename std_iterator_traits<InputIt>::value_type value_t;
    value_t acc = *first;
    *d_first = acc;
    while (++first != last) 
    {
        value_t val = *first;
        *++d_first = val - acc;
        acc = val;
    }

    return ++d_first;
}

template< class InputIt, class OutputIt, class BinaryOperation >
OutputIt std_adjacent_difference(InputIt first, InputIt last, OutputIt d_first, BinaryOperation op)
{
    if (first == last) return d_first;

    typedef typename std_iterator_traits<InputIt>::value_type value_t;
    value_t acc = *first;
    *d_first = acc;
    while (++first != last) 
    {
        value_t val = *first;
        *++d_first = op(val, acc); 
        acc = val;
    }

    return ++d_first;
}

template< class InputIt, class OutputIt >
OutputIt std_partial_sum(InputIt first, InputIt last, OutputIt d_first)
{
    if (first == last) return d_first;

    typename std_iterator_traits<InputIt>::value_type sum = *first;
    *d_first = sum;

    while (++first != last) 
    {
        sum = sum + *first;
        *++d_first = sum;
    }

    return ++d_first;
}

template< class InputIt, class OutputIt, class BinaryOperation >
OutputIt std_partial_sum(InputIt first, InputIt last, OutputIt d_first, BinaryOperation op)
{
    if (first == last) return d_first;

    typename std_iterator_traits<InputIt>::value_type sum = *first;
    *d_first = sum;

    while (++first != last) 
    {
        sum = op(sum, *first);
        *++d_first = sum;
    }

    return ++d_first;
}

#endif // !defined NUMERIC_H__
