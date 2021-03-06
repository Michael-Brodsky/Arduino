/*
 *	This file defines several objects in the C++ Standard Template Library 
 *	(STL) type support library.
 *
 *	***************************************************************************
 *
 *	File: type_traits.h
 *	Date: July 17, 2021
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
 *		This file defines some of the objects in the <type_traits> header of
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
 *		and so forth. Otherwise object names are identical to those defined
 *		by the Standard.
 * 
 *	**************************************************************************/

#if !defined TYPE_TRAITS_H__
#define TYPE_TRAITS_H__ 20210717L

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

template<bool B>
using std_bool_constant = std_integral_constant<bool, B>;

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
struct std_is_integral<wchar_t> : public std_true_type {};

template<>
struct std_is_integral<char16_t> : public std_true_type {};

template<>
struct std_is_integral<char32_t> : public std_true_type {};

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
struct std_is_signed<wchar_t> : public std_true_type {};

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
struct std_make_signed { typedef T type; };

template<>
struct std_make_signed<char> { typedef signed char type; };

template<>
struct std_make_signed<signed char> { typedef signed char type; };

template<>
struct std_make_signed<unsigned char> { typedef signed char type; };

template<>
struct std_make_signed<wchar_t> { typedef wchar_t type; };

template<>
struct std_make_signed<char16_t> { typedef signed short type; };

template<>
struct std_make_signed<char32_t> { typedef signed long type; };

template<>
struct std_make_signed<signed short> { typedef signed short type; };

template<>
struct std_make_signed<unsigned short> { typedef signed short type; };

template<>
struct std_make_signed<signed int> { typedef signed int type; };

template<>
struct std_make_signed<unsigned int> { typedef signed int type; };

template<>
struct std_make_signed<signed long> { typedef signed long type; };

template<>
struct std_make_signed<unsigned long> { typedef signed long type; };

#if defined LLONG_MAX
template<>
struct std_make_signed<signed long long> { typedef signed long long type; };

template<>
struct std_make_signed<unsigned long long> { typedef signed long long type; };
#endif // defined LLONG_MAX

template<class T>
struct std_make_unsigned { typedef T type; };

template<>
struct std_make_unsigned<char> { typedef unsigned char type; };

template<>
struct std_make_unsigned<signed char> { typedef unsigned char type; };

template<>
struct std_make_unsigned<unsigned char> { typedef unsigned char type; };

template<>
struct std_make_unsigned<wchar_t> { typedef unsigned short type; };

template<>
struct std_make_unsigned<char16_t> { typedef char16_t type; };

template<>
struct std_make_unsigned<char32_t> { typedef char32_t type; };

template<>
struct std_make_unsigned<signed short> { typedef unsigned short type; };

template<>
struct std_make_unsigned<unsigned short> { typedef unsigned short type; };

template<>
struct std_make_unsigned<signed int> { typedef unsigned int type; };

template<>
struct std_make_unsigned<unsigned int> { typedef unsigned int type; };

template<>
struct std_make_unsigned<signed long> { typedef unsigned long type; };

template<>
struct std_make_unsigned<unsigned long> { typedef unsigned long type; };

#if defined LLONG_MAX
template<>
struct std_make_unsigned<signed long long> { typedef unsigned long long type; };

template<>
struct std_make_unsigned<unsigned long long> { typedef unsigned long long type; };
#endif // defined LLONG_MAX

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

template<class B>
struct std_negation : std_bool_constant<!bool(B::value)> { };

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

namespace 
{
	template<class T>
	struct is_pointer_helper : std_false_type {};

	template<class T>
	struct is_pointer_helper<T*> : std_true_type {};
}
template<class T>
struct std_is_pointer : is_pointer_helper< typename std_remove_cv<T>::type > {};

template< class T > struct std_remove_pointer { typedef T type; };
template< class T > struct std_remove_pointer<T*> { typedef T type; };
template< class T > struct std_remove_pointer<T* const> { typedef T type; };
template< class T > struct std_remove_pointer<T* volatile> { typedef T type; };
template< class T > struct std_remove_pointer<T* const volatile> { typedef T type; };

#endif // !defined TYPE_TRAITS_H__