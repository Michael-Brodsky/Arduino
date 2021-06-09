/*
 *	This file defines several STL-style algorithms.
 *
 *	***************************************************************************
 *
 *	File: algorithm.h
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

#if !defined ALGORITHM_H__
#define ALGORITHM_H__ 20210315L

#include "functional.h"
#include "utility.h"
#include "iterator.h"

#pragma region non-modifying_sequence_operations

template<class InputIt, class Function>
Function std_for_each(InputIt first, InputIt last, Function fn)
{
	while (first != last) 
	{
		fn(*first);
		++first;
	}
	return fn;
}

template<class InputIt, class T>
	typename std_iterator_traits<InputIt>::difference_type
		std_count(InputIt first, InputIt last, const T& value) 
{	
	typename std_iterator_traits<InputIt>::difference_type n = 0;
	for(; first != last; ++first) 
		if(*first == value) 
			n++;
	return n;
}

template<class InputIt1, class InputIt2>
	std_pair<InputIt1, InputIt2>
		std_mismatch(InputIt1 first1, InputIt1 last1, InputIt2 first2)
{
	while (first1 != last1 && *first1 == *first2) 
	{
		++first1, ++first2;
	}
	return std_make_pair(first1, first2);
}
	
template<class InputIt, class T>
	InputIt std_find(InputIt first, InputIt last, const T& value)
{
	InputIt it = last;
	for(; first != last; ++first) 
	{
		if(*first == value) 
		{ 
			it = first;
			break;
		}
	}
	return it;
}

template<class ForwardIt1, class ForwardIt2>
	ForwardIt1 std_find_end(
		ForwardIt1 first, ForwardIt1 last, 
		ForwardIt2 seq_first, ForwardIt2 seq_last)
{
	ForwardIt1 result = last;
	if(seq_first != seq_last) 
	{
		while(1) 
		{
			ForwardIt1 search_result = std_search(first, last, seq_first, seq_last);
			if(search_result == last) 
				break;
			else 
			{
				result = search_result;
				first = result;
				++first;
			}
		}
	}
	return result;
}

template <class ForwardIt>
ForwardIt std_adjacent_find(ForwardIt first, ForwardIt last)
{
	ForwardIt result = last;
	if (first != last)
	{
		ForwardIt next = first; ++next;
		while (next != last) 
		{
			if (*first == *next) 
			{
				result = first;
				break;
			}
			++first; ++next;
		}
	}
	return result;
}

template<class InputIt, class ForwardIt>
InputIt std_find_first_of(
	InputIt first1, InputIt last1, ForwardIt first2, ForwardIt last2)
{
	while (first1 != last1) 
	{
		for (ForwardIt it = first2; it != last2; ++it) 
		{
			if (*it == *first1)
			return first1;
		}
		++first1;
	}
	return last1;
}

template<class ForwardIt1, class ForwardIt2>
	ForwardIt1 std_search(
		ForwardIt1 first, ForwardIt1 last, 
		ForwardIt2 seq_first, ForwardIt2 seq_last)
{
	ForwardIt1 result = ForwardIt1();
	for(; !std_is_dereferenceable(result); ++first) 
	{
		ForwardIt1 it = first;
		for(ForwardIt2 seq_it = seq_first; ; ++it, ++seq_it) 
		{
			if(seq_it == seq_last) 
			{
				result = first;
				break;
			}
			if(it == last) 
			{
				result =  last;
				break;
			}
			if(!(*it == *seq_it)) break;
		}
	}
	return result;
}

#pragma endregion

#pragma region modifying_sequence_operations

template<class InputIt, class OutputIt>
OutputIt std_copy(InputIt first, InputIt last, OutputIt dest) 
{
	while(first != last) 
		*dest++ = *first++;
	return dest;
}

template<class InputIt, class OutputIt>
OutputIt* std_copy(InputIt* first, InputIt* last, OutputIt* dest)
{
	while (first != last)
		*(*dest++) = *(*first++);
	return dest;
}

template<class BidirIt1, class BidirIt2>
BidirIt2 std_copy_backward(BidirIt1 first, BidirIt1 last, BidirIt2 dest)
{
	while (last!=first) 
		*(--dest) = *(--last);
	return dest;
}

template<class InputIt, class OutputIt>
OutputIt std_move(InputIt first, InputIt last, OutputIt dest)
{
	while (first != last) 
	{
		*dest = std_move(*first);
		++dest; 
		++first;
	}
	return dest;
}

template<class BidirIt1, class BidirIt2>
BidirIt2 std_move_backward(BidirIt1 first, BidirIt1 last, BidirIt2 dest)
{
	while (last != first) *(--dest) = std_move(*(--last));
	return dest;
}

template<class ForwardIt, class T>
void std_fill(ForwardIt first, ForwardIt last, const T& value) 
{
	while(first != last) 
		*first++ = value;
}

template <class ForwardIt, class T>
ForwardIt std_remove(ForwardIt first, ForwardIt last, const T& val)
{
	ForwardIt result = first;
	while (first != last) 
	{
		if (!(*first == val)) 
		{
			*result = *first;
			++result;
		}
		++first;
	}
	return result;
}

template <class InputIt, class OutputIt, class T>
	OutputIt std_remove_copy(
		InputIt first, InputIt last, OutputIt result, const T& val)
{
	while (first != last) 
	{
		if (!(*first == val)) 
		{
			*result = *first;
			++result;
		}
		++first;
	}
	return result;
}

template <class ForwardIt, class T>
	void std_replace(
		ForwardIt first, ForwardIt last, 
		const T& old_value, const T& new_value)
{
	while (first != last) 
	{
		if (*first == old_value) 
			*first = new_value;
		++first;
	}
}

template <class InputIt, class OutputIt, class T>
	OutputIt std_replace_copy(
		InputIt first, InputIt last, OutputIt result, 
		const T& old_value, const T& new_value)
{
	while (first != last) 
	{
		*result = (*first == old_value) ? new_value : *first;
		++first; 
		++result;
	}
	return result;
}
template<class ForwardIt1, class ForwardIt2>
void std_iter_swap(ForwardIt1 a, ForwardIt2 b)
{
	std_swap(*a, *b);
}

template<class ForwardIt1, class ForwardIt2>
ForwardIt2 std_swap_ranges(ForwardIt1 first, ForwardIt1 last, ForwardIt2 dest)
{
	while(first != last) 
		std_iter_swap(first++, dest++);
	return dest;
}

template <class BidirIt>
void std_reverse(BidirIt first, BidirIt last)
{
	while ((first != last) && (first != --last)) 
	{
		std_iter_swap (first, last);
		++first;
	}
}

template <class BidirIt, class OutputIt>
OutputIt std_reverse_copy(BidirIt first, BidirIt last, OutputIt dest)
{
	while (first != last) 
	{
		--last;
		*dest = *last;
		++dest;
	}
	return dest;
}

template <class ForwardIt>
void std_rotate(ForwardIt first, ForwardIt middle, ForwardIt last)
{
	ForwardIt next = middle;
	while (first != next)
	{
		std_swap (*first++,*next++);
		if (next == last) 
			next = middle;
		else if (first == middle) 
			middle = next;
	}
}

template <class ForwardIt, class OutputIt>
	OutputIt std_rotate_copy(
		ForwardIt first, ForwardIt middle, ForwardIt last, OutputIt dest)
{
	dest = std_copy(middle,last,dest);
	return std_copy(first,middle,dest);
}

template <class ForwardIt>
ForwardIt std_unique(ForwardIt first, ForwardIt last)
{
	ForwardIt result = first == last ? last - 1 : first;
	if (result == first) 
	{
		while (++first != last)
		{
			if (!(*result == *first))
				*(++result) = *first;
		}
	}
	return ++result;
}

template <class InputIt, class OutputIt>
OutputIt std_unique_copy(InputIt first, InputIt last, OutputIt dest)
{
	dest = first == last ? last-1 : first;
	if (dest == first) 
	{
		*dest = *first;
		while (++first != last) 
		{
			typename std_iterator_traits<InputIt>::value_type val = *first;
			if (!(*dest == val))
			*(++dest) = val;
		}
	}
	return ++dest;
}
#pragma endregion

#pragma region sorting_operations

template<class ForwardIt>
bool std_is_sorted(ForwardIt first, ForwardIt last)
{
	return std_is_sorted_until(first, last) == last;
}

template<class ForwardIt>
ForwardIt std_is_sorted_until(ForwardIt first, ForwardIt last)
{
	return std_is_sorted_until(
		first, last, std_less<typename std_iterator_traits<ForwardIt>::value_type>());
}

template <class ForwardIt, class Compare>
ForwardIt std_is_sorted_until (ForwardIt first, ForwardIt last, Compare comp)
{
	ForwardIt next = last;
	if (first != last) 
	{
		next = first;
		while (++next != last) 
		{
			if (comp(*next, *first))
				break;
			first = next;
		}
	}
	return next;
}

struct std_heap_sort_tag {};
struct std_insertion_sort_tag{};
struct std_quick_sort_tag {};

namespace
{
	template <class RandomIt>
	RandomIt std_partition(RandomIt first, RandomIt last)
	{
		RandomIt i = first, j = first;

		while (j < last)
		{
			if (*j < *last)
			{
				std_iter_swap(i, j);
				++i;
			}
			++j;
		}
		std_iter_swap(i, last);

		return i;
	}

	template <class RandomIt>
	void std_max_heap(RandomIt first, RandomIt last, RandomIt root)
	{
		RandomIt largest = root;
		RandomIt left = first + 2 * std_distance(first, root) + 1;
		RandomIt right = left + 1;

		if (left < last && *left >* largest)
			largest = left;
		if (right < last && *right >* largest)
			largest = right;
		if (largest != root)
		{
			std_iter_swap(root, largest);
			std_max_heap(first, last, largest);
		}
	}
} // namespace

template <class RandomIt>
void std_make_heap(RandomIt first, RandomIt last)
{
	RandomIt it = first + std_distance(first, last) / 2 - 1;

	while (it >= first)
		std_max_heap(first, last, it--);
}

template <class RandomIt>
bool std_is_heap(RandomIt first, RandomIt last, RandomIt root)
{
	bool result = false;

	if (root > first + (std_distance(first, last) - 2) / 2)
		result = true;
	else
	{
		RandomIt left = first + 2 * std_distance(first, root) + 1;
		RandomIt right = left + 1;

		if (*root >= *left && *root >= *right &&
			std_is_heap(first, last, left) && std_is_heap(first, last, right))
			result = true;
	}

	return result;
}

template <class RandomIt>
void std_sort(RandomIt first, RandomIt last, std_heap_sort_tag)
{
	std_make_heap(first, last);
	while (--last >= first)
	{
		std_iter_swap(first, last);
		std_max_heap(first, last, first);
	}
}

template <class RandomIt>
void std_sort(RandomIt first, RandomIt last, std_insertion_sort_tag) 
{
	RandomIt i = first;
	while(++i < last) 
	{
		typename std_iterator_traits<RandomIt>::value_type tmp = *i;
		RandomIt j = i - 1;
		while(j >= first && *j > tmp) 
		{
			*(j + 1) = *j;
			j--;
		}
		*(j + 1) = tmp;
	}
}

//
// QuickSort is a recursive algorithm and limited by stack size!!!
//
template <class RandomIt>
void std_sort(RandomIt first, RandomIt last, std_quick_sort_tag)
{
	if (first < last)
	{
		RandomIt it = std_partition(first, last);

		std_sort(first, it - 1, std_quick_sort_tag());
		std_sort(it + 1, last, std_quick_sort_tag());
	}
}

#pragma endregion

#pragma region binary_search_operations

template <class ForwardIt, class T>
ForwardIt std_lower_bound(ForwardIt first, ForwardIt last, const T& val)
{
	ForwardIt it;
	typename std_iterator_traits<ForwardIt>::difference_type count, step;
	count = std_distance(first, last);
	while (count > 0)
	{
		it = first; 
		step = count / 2; 
		std_advance(it, step);
		if (*it < val) 
		{
			first = ++it;
			count -= step + 1;
		}
		else 
			count = step;
	}
	return first;
}

template <class ForwardIt, class T>
ForwardIt std_upper_bound(ForwardIt first, ForwardIt last, const T& val)
{
	ForwardIt it;
	typename std_iterator_traits<ForwardIt>::difference_type count, step;
	count = std_distance(first, last);
	while (count > 0)
	{
		it = first; 
		step = count / 2; 
		std_advance(it, step);
		if (!(val < *it))
		{ 
			first = ++it; 
			count -= step + 1;  
		}
		else count = step;
	}
	return first;
}

template <class ForwardIt, class T>
bool std_binary_search(ForwardIt first, ForwardIt last, const T& val)
{
	first = std_lower_bound(first, last, val);
	return (first != last && !(val < *first));
}

template <class ForwardIt, class T>
std_pair<ForwardIt, ForwardIt>
	std_equal_range(ForwardIt first, ForwardIt last, const T& val)
{
	ForwardIt it = std_lower_bound(first, last, val);
	return std_make_pair(it, std_upper_bound(it, last, val));
}

#pragma endregion

#pragma region min/max_operations

template <class T> const T& 
std_min (const T& a, const T& b) 
{
	return !(b < a) ? a : b;
}

template <class T> const T& 
std_max (const T& a, const T& b) 
{
	return !(a < b) ? b : a;
}

template <class T> 
std_pair <const T&, const T&> std_minmax(const T& a, const T& b) 
{
	return (b < a) ? std_make_pair(b, a) : std_make_pair(a, b);
}

template <class ForwardIt>
ForwardIt std_min_element(ForwardIt first, ForwardIt last)
{
	if (first == last) return last;
	ForwardIt smallest = first;

	while (++first != last)
		if (*first < *smallest) 
			smallest = first;
	return smallest;
}

template<class ForwardIt, class Compare>
ForwardIt std_min_element(ForwardIt first, ForwardIt last, Compare comp)
{
	if (first == last) return last;

	ForwardIt smallest = first;
	++first;
	for (; first != last; ++first) {
		if (comp(*first, *smallest)) {
			smallest = first;
		}
	}
	return smallest;
}

template <class ForwardIt>
ForwardIt std_max_element(ForwardIt first, ForwardIt last)
{
	if (first == last) return last;
	ForwardIt largest = first;

	while (++first != last)
		if (*largest < *first)
			largest = first;
	return largest;
}

template<class ForwardIt, class Compare>
ForwardIt std_max_element(ForwardIt first, ForwardIt last, Compare comp)
{
	if (first == last) return last;

	ForwardIt largest = first;
	++first;
	for (; first != last; ++first) {
		if (comp(*largest, *first)) {
			largest = first;
		}
	}
	return largest;
}
#pragma endregion

#pragma region set_operations

template <class InputIt1, class InputIt2>
	bool std_includes (
		InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2)
{
	bool result = true;
	for (; first2 != last2; ++first1)
	{
		if (first1 == last1 || *first2 < *first1)
		{
			result = false;
			break;
		}
		if (!(*first1 < *first2))
			++first2;
	}
	return true;
}

template <class InputIt1, class InputIt2, class OutputIt>
	OutputIt std_set_difference(
		InputIt1 first1, InputIt1 last1, InputIt2 first2, 
		InputIt2 last2, OutputIt dest)
{
	while (first1 != last1 && first2 != last2)
	{
		if (*first1 < *first2) 
		{ 
			*dest = *first1; 
			++dest; 
			++first1; 
		}
		else if (*first2 < *first1) 
			++first2;
		else 
		{ 
			++first1; 
			++first2; 
		}
	}
	return std_copy(first1, last1, dest);
}

template <class InputIt1, class InputIt2, class OutputIt>
	OutputIt std_set_intersection(
		InputIt1 first1, InputIt1 last1, InputIt2 first2, 
		InputIt2 last2, OutputIt dest)
{
	while (first1!=last1 && first2!=last2)
	{
		if (*first1<*first2) 
			++first1;
		else if (*first2<*first1) 
			++first2;
		else 
		{
			*dest = *first1;
			++dest; 
			++first1; 
			++first2;
		}
	}
	return dest;
}

template <class InputIt1, class InputI2, class OutputIt>
	OutputIt std_set_union(
		InputIt1 first1, InputIt1 last1, InputI2 first2, 
		InputI2 last2, OutputIt dest)
{
	while(true)
	{
		if (first1 == last1) return std_copy(first2, last2, dest);
		if (first2 == last2) return std_copy(first1, last1, dest);
		if (*first1 < *first2) 
		{ 
			*dest = *first1; 
			++first1; 
		}
		else if (*first2 < *first1) 
		{ 
			*dest = *first2; 
			++first2; 
		}
		else 
		{ 
			*dest = *first1; 
			++first1; 
			++first2; 
		}
		++dest;
	}
}

template <class InputIt1, class InputIt2, class OutputIt>
	OutputIt std_set_symmetric_difference(
		InputIt1 first1, InputIt1 last1, InputIt2 first2, 
		InputIt2 last2, OutputIt dest)
{
	while (true)
	{
		if (first1 == last1) return std_copy(first2, last2, dest);
		if (first2 == last2) return std_copy(first1, last1, dest);
		if (*first1 < *first2) 
		{ 
			*dest = *first1; 
			++dest; 
			++first1; 
		}
		else if (*first2 < *first1) 
		{ 
			*dest = *first2; 
			++dest; 
			++first2; 
		}
		else 
		{
			++first1; 
			++first2; 
		}
	}
}

#pragma endregion

#pragma region numeric_operations

#pragma endregion

#pragma region compare_operations

template<class InputIt1, class InputIt2>
	bool std_lexicographical_compare(
		InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2)
{
	bool result = false;
	for(; (first1 != last1) && (first2 != last2); ++first1, (void)++first2)
	{
		if(*first1 < *first2) 
		{
			result = true;
			break;
		}
		if(*first2 < *first1) 
			break;
	}
	result = first1 == last1 ? first2 != last2 : result;
	return result;
}

template<class InputIt1, class InputIt2>
bool std_equal(InputIt1 first1, InputIt1 last1, InputIt2 first2)
{
	for(; first1 != last1; ++first1, ++first2)
		if(!(*first1 == *first2))
			break;
	return first1 == last1;
}

#pragma endregion

#endif // !defined ALGORITHM_H__