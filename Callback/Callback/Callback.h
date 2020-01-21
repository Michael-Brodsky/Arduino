/*
 *	This file defines several template typedefs for implementing callbacks. 

 *	***************************************************************************/

/*	File: Callback.h
 *	Date: January 16, 2020
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
 *	This file includes several templated typedefs for defining callback 
 *	function signatures with template parameters. They can be templated as
 *	memeber method or free-standing function callbacks, with or without 
 *	arguments.
 *
 *	Examples:
 *
 *		struct Object { int doSomething(int i) { ... } };
 *		void doSomething() { ... }
 *		Object obj;
 *
 *		...
 *
 *		Callback<int, Object, int>::callback_t cb1 = &Object::doSomething;
 *		Callback<void>::callback_t cb2 = &doSomething;
 *
 *		(obj.*cb1)(42);	// Calls obj.doSomething(42);
 *		cb2();			// Calls doSomething();
 */

#if !defined CALLBACK_H__
#define CALLBACK_H__ 20200116L

#include <types.h>	// Check for compiler C++ Standard.

#if defined __HAS_VARIADIC_TEMPLATES	// Requires C++11 or later.

template <class Ret, class Obj = void, class ... Args>
struct Callback 
{
	typedef Ret(Obj::*callback_t)(Args ...);
};

template <class Ret, class ... Args>
struct Callback<Ret, void, Args ...>
{
	typedef Ret(*callback_t)(Args ...);
};

#else // !defined __HAS_VARIADIC_TEMPLATES

template <class Ret, class Obj = void, class Arg = void>
struct Callback 
{
	typedef Ret(Obj::*callback_t)(Arg);
}; 

template <class Ret, class Arg>
struct Callback<Ret, void, Arg>
{
	typedef Ret(*callback_t)(Arg);
};

#endif // defined __HAS_VARIADIC_TEMPLATES 

template <class Ret, class Obj>
struct Callback<Ret, Obj, void> 
{
	typedef Ret(Obj::*callback_t)();
};

template <class Ret>
struct Callback<Ret, void, void> 
{
	typedef Ret(*callback_t)();
};

#endif // !defined CALLBACK_H__
