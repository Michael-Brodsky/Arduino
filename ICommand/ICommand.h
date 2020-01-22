/*
 *	This file defines several types for implementing the C++ "Command" design 
 *	pattern. 

 *	***************************************************************************/

/*	File: ICommand.h
 *	Date: January 18, 2020
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
 *	The "Command" pattern is an object behavioral pattern that decouples 
 *	the sender from the receiver by enapsulating a request as an object. It 
 *	can be thought of as an object-oriented equivalent to a callback method.
 *	State information enccapsulated within the command object can also be 
 *	preserved between successive calls.
 *
 *	This file defines an abstract interface class `ICommand' which all 
 *	command objects should derive from. ICommand declares one virtual method: 
 *
 *		void execute();
 *
 *	Code placed in a derived type's `execute()' method will run each time it 
 *	is called, and can be anything from a simple function call to an entire 
 *	application.
 *
 *	Several useful concrete types are also defined:
 *
 *		NullCommand:	a command object with an empty `execute()' method, 
 *						useful in cases where a function argument of type 
 *						`ICommand' cannot be NULL, such as when the function 
 *						takes a reference rather than a pointer to `ICommand'.
 *
 *		Command:		The `Command' types are templated callback types that 
 *						allow users to customize the callback signature with 
 *						template parameters. They can be templated as memeber 
 *						method or free-standing function callbacks, with or 
 *						without arguments (see <Callback.h>).
 *
 *	All of the concrete types defined here derive from `ICommand', which is 
 *	non-templated and can thus be placed into a collection of type `ICommand'.
 *
 *		e.g.	ICommand* commands[] = { ... };
 *
 *	Examples:
 *
 *		struct Object { void doSomething(int i) { ... } };
 *		Object obj;
 *		
 *		class MyCommand : public ICommand 
 *		{
 *		public:
 *			MyCommand(Object& object, int arg) : object_(object), arg_(arg) { }
 *			void execute() override { object_.doSomething(arg_); }
 *		private:
 *			Object& object_;
 *			int arg_;
 *		};
 *
 *		MyCommand cmd(obj, 42);
 *		cmd.execute()	// Calls obj.doSomething(42).
 */

#if !defined ICOMMAND_H__
#define ICOMMAND_H__ 20200118L

#include <utils.h>		// _typename macro.
#include <Callback.h>	// Callback types.

struct ICommand	// "Command" design pattern abstract interface class.
{
	virtual ~ICommand() { }

	virtual void execute() = 0;
};

struct NullCommand : public ICommand	// Null command type.
{
	NullCommand() : ICommand() { }
	virtual ~NullCommand() { }

	void execute() override { }
};

template <class Ret, class Obj = void, class Arg = void>
class Command : public ICommand	// Command type for member function callbacks with arguments.
{
	typedef typename Callback<Ret, Obj, Arg>::callback_t callback_type;
public:
	Command(Arg arg, Obj* receiver = nullptr, _typename callback_type callback = nullptr) : 
		ICommand(), arg_(arg), receiver_(receiver), callback_(callback) { }
	virtual ~Command() { }
public:
	void execute() override { (receiver_->*callback_)(arg_); }
	Arg	arg() const { return arg_; }
private:
	Obj* receiver_;
	_typename callback_type callback_;
	Arg arg_;
};

template <class Ret, class Arg>
class Command<Ret, void, Arg> : public ICommand	// Command type for free-standing function callbacks with arguments.
{
	typedef typename Callback<Ret, void, Arg>::callback_t callback_type;
public:
	Command(Arg arg, _typename callback_type callback = nullptr) : 
		ICommand(), arg_(arg), callback_(callback) { }
	virtual ~Command() { }
public:
	void execute() override { (*callback_)(arg_); }
	Arg	arg() const { return arg_; }
private:
	_typename callback_type callback_;
	Arg arg_;
};

template <class Ret, class Obj>
class Command<Ret, Obj, void> : public ICommand	// Command type for member function callbacks without arguments.
{
	typedef typename Callback<Ret, Obj, void>::callback_t callback_type;
public:
	Command(Obj* object = nullptr, _typename callback_type callback = nullptr) : 
		ICommand(), object_(object), callback_(callback) { }
	virtual ~Command() { }
public:
	void execute() override { (object_->*callback_)(); }
private:
	Obj* object_;
	_typename callback_type callback_;
};

template <class Ret>
class Command<Ret, void, void> : public ICommand	// Command type for free-standing function callbacks without arguments.
{
	typedef typename Callback<Ret, void, void>::callback_t callback_type;
public:
	Command(_typename callback_type callback = nullptr) : 
		ICommand(), callback_(callback) { }
	virtual ~Command() { }
public:
	void execute() override { (*callback_)(); }
private:
	_typename callback_type callback_;
};

#endif // !defined ICOMMAND_H__
