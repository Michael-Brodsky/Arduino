/*
 *	This file defines several types for implementing the C++ "Command" design 
 *	pattern. 
 *
 *	***************************************************************************
 *
 *	File: ICommand.h
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
 *  You should have received a copy of the GNU General Public License 
 *	along with this file. If not, see <http://www.gnu.org/licenses/>.
 *
 *	**************************************************************************
 *
 *	Description:
 *
 *	The "Command" pattern is an object behavioral pattern that decouples 
 *	the sender from the receiver by enapsulating a request as an object. It 
 *	can be thought of as an object-oriented equivalent to a callback method.
 *	State information encapsulated within the command object can also be 
 *	preserved between successive calls.
 *
 *	This file defines an abstract interface class `ICommand' which all 
 *	command objects should derive from. ICommand declares one virtual method: 
 *
 *		void execute();
 *
 *	Code placed in a derived type's `execute()' method will run each time it 
 *	is called, and can be anything valid.
 *
 *	Several useful concrete types are also defined:
 *
 *		NullCommand:	a command object with an empty `execute()' method, 
 *						useful in cases where a function argument of type 
 *						`ICommand' cannot be NULL, such as when the function 
 *						takes a reference rather than a pointer to `ICommand'.
 *
 *		Command:		The `Command' types are class templates for  
 *						implementing callbacks as command objects and 
 *						specifying the callback signature with template 
 *						parameters. The templates support both member method 
 *						or free-standing function callbacks, with or without 
 *						arguments (see <Callback.h>).
 *
 *	The concrete Command templates take three parameters: 
 *
 *		Ret: The return type of the callback method or function, 
 *		Obj: The object type (class) of a member callback method, 
 *		Arg: The argument type passed to a callback method or function, if any.
 * 
 * The Obj and Arg template parameters default to `void'. The Ret parameter 
 * must be specified even if it is of type `void'. For example: 
 * 
 *		void handler();
 * 
 *		Command<void> command(&handler);
 * 
 *		void anotherHandler(int);
 * 
 *		Command<void, void, int> command(&anotherHandler, 42);
 * 
 *		class Foo {
 *		public:
 *			Foo() = default;
 *			void bar();
 *		};
 * 
 *		Foo foo;
 *		Command<void, Foo> command(&foo, &Foo::bar);
 * 
 *		...
 * 
 *		command.execute();
 */

#if !defined ICOMMAND_H__
# define ICOMMAND_H__ 20210717L

# include "Callback.h"			// `Callback' types.
# include "ICloneable.h"		// `ICloneable' interface.
# include "ISerializeable.h"	// `ISerializeable' interface.

// "Command" design pattern abstract interface class.
struct ICommand
{
	virtual ~ICommand() = default;

	virtual void execute() = 0;
};

// Cloneable command interface.
struct ICloneCommand : public ICommand, public ICloneable
{
	virtual ~ICloneCommand() = default;
};

struct ISerializableCommand : public ICommand, public ISerializeable
{
	virtual ~ISerializableCommand() = default;
};

// Serializable command interface.
struct ISerialCommand : public ICloneCommand, public ISerializeable
{
	virtual ~ISerialCommand() = default;
};

// Constant indicating no command specified.
const nullptr_t NoCommand = nullptr;

// Null command type.
struct NullCommand : public ICommand 
{
	NullCommand() = default;
	~NullCommand() = default;

	void execute() override {}
};

// Command type for member function callbacks with arguments.
template <class Ret, class Obj = void, class Arg = void>
class Command : public ICommand	
{
public:
	// Command callback type.
	using callback_type = typename Callback<Ret, Obj, Arg>::callback_t;

public:
	Command(Obj* receiver, _typename callback_type callback, Arg arg) : 
		ICommand(), arg_(arg), receiver_(receiver), callback_(callback) {}
	~Command() {}

public:
	// Command execute method.
	void execute() override { (void)(receiver_->*callback_)(arg_); }

protected:
	Obj* receiver_;						// Command receiver.
	_typename callback_type callback_;	// Receiver::method.
	Arg arg_;							// Method argument.
};

// Command type for free-standing function callbacks with arguments.
template <class Ret, class Arg>
class Command<Ret, void, Arg> : public ICommand	
{
public:
	// Command callback type.
	using callback_type = typename Callback<Ret, void, Arg>::callback_t;

public:
	Command(_typename callback_type callback, Arg arg) : 
		ICommand(), arg_(arg), callback_(callback) {}
	~Command() {}

public:
	// Command execute method.
	void execute() override { (void)(*callback_)(arg_); }

protected:
	_typename callback_type callback_;	// Command callback.
	Arg arg_;							// Command argument.
};

// Command type for member function callbacks without arguments.
template <class Ret, class Obj>
class Command<Ret, Obj, void> : public ICommand	
{
public:
	// Command callback type.
	using callback_type = typename Callback<Ret, Obj, void>::callback_t;

public:
	Command(Obj* object, _typename callback_type callback) : 
		ICommand(), receiver_(object), callback_(callback) {}
	~Command() {}

public:
	// Command execute method.
	void execute() override { (void)(receiver_->*callback_)(); }

protected:
	Obj* receiver_;						// Command receiver.
	_typename callback_type callback_;	// Receiver::method.
};

// Command type for free-standing function callbacks without arguments.
template <class Ret>
class Command<Ret, void, void> : public ICommand 
{
public:
	// Command callback type.
	using callback_type = typename Callback<Ret, void, void>::callback_t ;

public:
	Command(_typename callback_type callback) : 
		ICommand(), callback_(callback) {}
	~Command() {}

public:
	// Command execute method.
	void execute() override { (void)(*callback_)(); }

protected:
	_typename callback_type callback_;	// Command callback.
};

#endif // !defined ICOMMAND_H__
