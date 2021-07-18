/*
 *	This file declares an asynchronous serial command execution class.
 *
 *	***************************************************************************
 *
 *	File: SerialRemote.h
 *	Date: July 18, 2021
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

#if !defined SERIALREMOTE_H__ 
# define SERIALREMOTE_H__ 20210718L

# include <string.h>		// C-stdlib string functions.
# include "array.h"			// STL fixed-size array types.
# include "IClockable.h"	// `IClockable' interface class.
# include "IComponent.h"	// `IComponent' interface class.

enum class CommandTag;	// Client-defined command tags.

// Asynchronous serial command execution type.
class SerialRemote : public IClockable, public IComponent
{
public:
	// `SerialRemote' command type.
	class Command
	{
	public:
		// Command constructor.
		Command(CommandTag tag, const char* key, ICommand* program) : 
			tag_(tag), key_(key), program_(program) 
		{
			assert(program);
		}

	public:
		// Returns an immutable iterator to the command object.
		ICommand* program() const
		{
			return program_;
		}

		// Compares a command's key string to another string.
		bool operator==(const char* key) const
		{
			return !strncmp(key_, key, strlen(key_));
		}

		// Compares a command's key string to another command's key string.
		bool operator==(const Command& other) const
		{
			return !strncmp(key_, other.key_, strlen(key_));
		}

	private:
		CommandTag	tag_;		// The command's identifying tag.
		const char* key_;		// The command's key string.
		ICommand*	program_;	// The command object to execute.
	};

	static const char EndOfTextChar = '\n';	// Character indicating end of text.

	using commands_type = ArrayWrapper<const Command>;		// Command collection container type.
	using commands_iter = commands_type::const_iterator;	// Command container immutable iterator type.
	using buf_type = ArrayWrapper<char>;					// Read/write buffer container type.
	using buf_iter = buf_type::iterator;					// Read/write buffer container mutable iterator type.

public:
	// Unsized array constructor.
	template<size_t SizeBuf, size_t SizeCmds>
	SerialRemote(char (&)[SizeBuf], const Command(&)[SizeCmds]);
	// Sized array constructor.
	SerialRemote(char*, size_t, const Command[], size_t);
	// Range constructor.
	SerialRemote(char*, char*, const Command*, const Command*);

public:
	// Polls the serial port buffer for commands.
	void		poll();
	// Returns a mutable iterator to the read/write buffer.
	char*		buf();
	// Returns an immutable iterator to the read/write buffer.
	const char* buf() const;
	// Returns a mutable reference to the echo flag.
	bool&		echo();
	// Returns an immutable reference to the echo flag.
	const bool& echo() const;

private:
	// IClockable clock method implementation.
	void clock() override;

private:
	commands_type	commands_;	// The current command collection.
	commands_iter	current_;	// Points to the currently matched command, if any. 
	buf_type		buf_;		// Serial read/write buffer.
	buf_iter		data_;		// Current read/write position.
	bool			echo_;		// Flag indicating whether to echo the buffer after command execution.
};

template<size_t SizeBuf, size_t SizeCmds>
SerialRemote::SerialRemote(char (&buf)[SizeBuf], const Command(&commands)[SizeCmds]) :
	commands_(commands), current_(std_end(commands_)), buf_(buf), data_(buf_.begin()) 
{
	
}

#endif // !defined SERIALREMOTE_H__