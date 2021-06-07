/*
 *	This file is part of the STL-style type support library.
 *
 *	***************************************************************************
 *
 *	File: type_traits.h
 *	Date: April 8, 2021
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
 *		This file is still under development.
 * 
 *	**************************************************************************/

#if !defined TYPE_TRAITS_H__
#define TYPE_TRAITS_H__ 20210408L

#include <stddef.h>
#include <limits.h>

template<class T, T val>
struct std_integral_constant						
{
	static const T value = val;

	typedef T value_type;
	typedef std_integral_constant<T, val> type;

	operator value_type() const
	{
		return value;
	}
};

template<bool B, class T, class F>
struct std_conditional { typedef T type; };

template<class T, class F>
struct std_conditional<false, T, F> { typedef F type; };

template< bool B, class T, class F >
using std_conditional_t = typename std_conditional<B, T, F>::type;

typedef std_integral_constant<bool, true> std_true_type;

typedef std_integral_constant<bool, false> std_false_type;

template<class T>
struct std_is_void : public std_false_type {};

template<>
struct std_is_void<void> : public std_true_type	{};

template<bool B, class T = void>
struct std_enable_if {};

template<class T>
struct std_enable_if<true, T> { typedef T type; };

template<class T1, class T2>
struct std_is_same : public std_false_type {};

template<class T>
struct std_is_same<T, T> : public std_true_type {};

template<class T> 
struct std_is_const : public std_false_type {};

template<class T> 
struct std_is_const<const T> : public std_true_type {};

// COMPILER WARNING: -std=c++1z or -std=gnu++1z
//template< class T >
//inline constexpr bool std_is_const_v = std_is_const<T>::value;

template <class T> 
struct std_is_reference : public std_false_type {};

template <class T> 
struct std_is_reference<T&> : public std_true_type {};

template <class T> 
struct std_is_reference<T&&> : public std_true_type {};

// COMPILER WARNING: -std=c++1z or -std=gnu++1z
//template< class T >
//inline constexpr bool std_is_reference_v = std_is_reference<T>::value;

template<class T>
struct std_is_array	: public std_false_type {};

template<class T, size_t N>
struct std_is_array<T[N]> : public std_true_type {};

template<class T>
struct std_is_array<T[]> : public std_true_type {};

// COMPILER WARNING: -std=c++1z or -std=gnu++1z
//template<class T>
//inline constexpr bool std_is_array_v = std_is_array<T>::value;

template<class T>
struct std_is_function : std_integral_constant<
	bool, 
	!std_is_const<const T>::value && !std_is_reference<T>::value> {};

template<class T>
struct std_is_integral : public std_false_type {};

template<>
struct std_is_integral<bool> : public std_true_type {};

template<>
struct std_is_integral<char> : public std_true_type {};

template<>
struct std_is_integral<unsigned char> : public std_true_type {};

template<>
struct std_is_integral<signed char> : public std_true_type {};

template<>
struct std_is_integral<unsigned short> : public std_true_type {};

template<>
struct std_is_integral<signed short> : public std_true_type {};

template<>
struct std_is_integral<unsigned int> : public std_true_type	{};

template<>
struct std_is_integral<signed int> : public std_true_type {};

template<>
struct std_is_integral<unsigned long> : public std_true_type {};

template<>
struct std_is_integral<signed long> : public std_true_type {};

#if defined LLONG_MAX
template<>
struct std_is_integral<signed long long> : public std_true_type {};

template<>
struct std_is_integral<unsigned long long> : public std_true_type {};
#endif // if defined LLONG_MAX

template<class T>
struct std_is_floating_point : public std_false_type {};

template<>
struct std_is_floating_point<float> : public std_true_type {};

template<>
struct std_is_floating_point<double> : public std_true_type	{};

template<>
struct std_is_floating_point<long double> : public std_true_type {};

template<class T>
struct std_is_signed : public std_false_type {};

template<>
struct std_is_signed<signed char> : public std_true_type {};

template<>
struct std_is_signed<signed short> : public std_true_type {};

template<>
struct std_is_signed<signed int> : public std_true_type	{};

template<>
struct std_is_signed<signed long> : public std_true_type {};

#if defined LLONG_MAX
template<>
struct std_is_signed<signed long long> : public std_true_type {};
#endif // defined LLONG_MAX

template<>
struct std_is_signed<float> : public std_true_type {};

template<>
struct std_is_signed<double> : public std_true_type	{};

template<>
struct std_is_signed<long double> : public std_true_type {};

template<class T>
struct std_remove_reference { typedef T type; };

template<class T>
struct std_remove_reference<T&>	{ typedef T type; };

template<class T>
struct std_remove_reference<T&&> { typedef T type; };

/************************************************************************************
 * `std_add_pointer' only works if defined in the translation unit where it's used. *
 ************************************************************************************/

namespace {

	template <class T>
	struct ptr_type_identity { using type = T; }; 

	template <class T>
	auto try_add_pointer(int)->ptr_type_identity<typename std_remove_reference<T>::type*> {};
	template <class T>
	auto try_add_pointer(...)->ptr_type_identity<T> {};

} // namespace 

template <class T>
struct std_add_pointer : decltype(try_add_pointer<T>(0)) {};

/********************************************************************
 * `std_add_lvalue_reference' and `std_add_rvalue_reference'        *
 * only work if defined in the translation unit where they're used. *
 ********************************************************************/

namespace {

	template <class T>
	struct ref_type_identity { using type = T; }; 
 
	template <class T>
	auto try_add_lvalue_reference(int)->ref_type_identity<T&>;
	template <class T>
	auto try_add_lvalue_reference(...)->ref_type_identity<T>;

	template <class T>
	auto try_add_rvalue_reference(int)->ref_type_identity<T&&>;
	template <class T>
	auto try_add_rvalue_reference(...)->ref_type_identity<T>;

} // namespace 

template <class T>
struct std_add_lvalue_reference : decltype(try_add_lvalue_reference<T>(0)) {};

template <class T>
struct std_add_rvalue_reference : decltype(try_add_rvalue_reference<T>(0)) {};

template<class T> 
struct std_remove_cv { typedef T type; };

template<class T> 
struct std_remove_cv<const T> { typedef T type; };

template<class T> 
struct std_remove_cv<volatile T> { typedef T type; };

template<class T> 
struct std_remove_cv<const volatile T> { typedef T type; };

template<class T> 
struct std_remove_const { typedef T type; };

template<class T> 
struct std_remove_const<const T> { typedef T type; };

template<class T> 
struct std_remove_volatile { typedef T type; };

template<class T> 
struct std_remove_volatile<volatile T> { typedef T type; };

template<class T>
struct std_remove_extent { typedef T type; };

template<class T>
struct std_remove_extent<T[]> { typedef T type; };

template<class T, size_t N>
struct std_remove_extent<T[N]> { typedef T type; };

template< class T >
struct std_decay {
private:
	typedef typename std_remove_reference<T>::type U;
public:
	typedef typename std_conditional<
		std_is_array<U>::value,
		typename std_remove_extent<U>::type*,
		typename std_conditional<
		std_is_function<U>::value,
		typename std_add_pointer<U>::type,
		typename std_remove_cv<U>::type
		>::type
	>::type type;
};

template< class T >
using std_decay_t = typename std_decay<T>::type;

template <typename...>
using std_void_t = void;

template<class T>
typename std_add_rvalue_reference<T>::type std_declval();
//typename std_add_rvalue_reference<T>::type std_declval() { return typename std_add_rvalue_reference<T>::type(); };

// primary template (used for zero types)
template <class...>
struct std_common_type {};

//////// one type
template <class T>
struct std_common_type<T> : std_common_type<T, T> {};

//////// two types
template <class T1, class T2>
using std_cond_t = decltype(false ? std_declval<T1>() : std_declval<T2>());

template <class T1, class T2, class = void>
struct std_common_type_2_impl {};

template <class T1, class T2>
struct std_common_type_2_impl<T1, T2, std_void_t<std_cond_t<T1, T2>>> {
	using type = typename std_decay<std_cond_t<T1, T2>>::type;
};

template <class T1, class T2>
struct std_common_type<T1, T2>
	: std_common_type_2_impl<typename std_decay<T1>::type,
	typename std_decay<T2>::type> {};

////// 3+ types
template <class AlwaysVoid, class T1, class T2, class...R>
struct std_common_type_multi_impl {};

template <class T1, class T2, class...R>
struct std_common_type_multi_impl<
	std_void_t<typename std_common_type<T1, T2>::type>, T1, T2, R...>
	: std_common_type<typename std_common_type<T1, T2>::type, R...> {};

template <class T1, class T2, class... R>
struct std_common_type<T1, T2, R...>
	: std_common_type_multi_impl<void, T1, T2, R...> {};

template< class... T >
using std_common_type_t = typename std_common_type<T...>::type;

#endif // !defined TYPE_TRAITS_H__