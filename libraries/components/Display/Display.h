/*
 *	This files declares an abstract character display management class.
 *
 *	***************************************************************************
 *
 *	File: Display.h
 *	Date: April 26, 2021
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
 *		The `Display' class can be used to asynchronously manage a character
 *		display by refreshing it as needed, changing the appearance of the 
 *		display cursor, automatically positioning it at specified locations 
 *		and formatting text for printing to the display hardware. This makes 
 *		it useful for implementing user interfaces where the display 
 *		behaviors or appearance need to change in response to external
 *		inputs, such as when a user edits application settings. 
 *
 *		`Display' is an abstract class that declares four purely virtual 
 *		methods that must be implemented by derived types: `display_clear(),' 
 *		`display_enable()', `display_cursor()' and `display_field()'. These   
 *		methods handle non-printing functions and are declared virtual so that 
 *		clients can interface with implementation-specific hardware APIs, such 
 *		as the Arduino `LiquidCrystal' API. Printing functions are handled by 
 *		the client and implemented via callbacks.
 *
 *		`Display' handles the common task encountered in display management. 
 *		Behaviors are implemented using a "screen" and "field" pattern where a
 *		field is a particular position (col/row) on the display, and a screen
 *		is a collection of such fields. They are represented by the two nested 
 *		types: `Screen' and `Field'. The `Screen' type also encapsulates print 
 *		formatting and allows clients to print an entire formatted screen to 
 *		the display hardware all at once. 
 *		
 *			// A collection of display fields with their col/row coordinates:
 *			DisplayType::Field fields[] = 
 *			{ 
 *				DisplayType::Field(0, 0),	// Hour
 *				DisplayType::Field(3, 0),	// Minute
 *				DisplayType::Field(6, 0),	// Second
 *			};
 *			
 *			// Fields assigned to a screen object along with their print format.
 *			DisplayType::Screen screen(fields, "%02u:%02u:%02u"); 
 * 
 *		Clients must implement a callback function to handle printing to the  
 *		display hardware. 
 * 
 *			// Client callback function handles printing.
 *			void displayCallback();
 * 
 *		PROTIP ON PRINTING AND LIMITATIONS OF THE ARDUINO `LiquidCrystal' API.
 * 
 *		The Arduino API can only print a single row of chars to the hardware 
 *		at a time. Therefore, the `Screen' class and its associated methods 
 *		are fashioned on a "per row" basis. `Screen' objects take an array 
 *		parameter that contains print format specifiers for each row in the 
 *		display hardware. So for a two row display, upto two format specifiers 
 *		are passed: 
 * 
 *			const char* rows[] = { "%02u:%02u:%02u", "%s %s" };
 *			DisplayType::Screen screen(fields, rows);  
 * 
 *		In the client callback function, the entire display can be printed 
 *		in the following fashion:
 * 
 *		void displayCallback()
 *		{ 
 *			char buf[Cols + 1];	   // Buffer size set from client-supplied `Cols' value.
 *			uint8_t row = 0;	   // Keeps track of current row index. 
 *			lcd.setCursor(0, row); // Cursor must be positioned for each row.
 *			lcd.print(screen(buf, row, ... ); // Row formatting handled by `Screen' object.
 *			lcd.setCursor(0, ++row);		  // Position, ...
 *			lcd.print(screen(buf, row, ... ); // ... format and print next row(s).
 *		}
 * 
 *		Most applications use displays in one of two ways, to display their   
 *		current "state" or to edit settings. Consider a digital clock that can 
 *		display the current time, or allow users to set the time and the alarm. 
 *		In each case, the display behaviors and appearance are different. This  
 *		is facilitated by the `Cursor' nested type, which can take on one of 
 *		two values, `Normal' or `Edit', and is used to indicate how the display 
 *		should behave. Clients set the value with the `Display::cursor' method 
 * 
 *			DisplayType.cursor(DisplayType::Cursor::Edit); 
 * 
 *		and implement the functionality in a concrete derived type..
 * 
 *		Clients can assign the current screen and navigate its fields using 
 *		the `screen()', `next()' and `prev()' methods. Using the above 
 *		example:
 * 
 *			display.screen(screen); // Set the current screen object, field initialized to "Hour".
 *			display.next(); // field advances to "Minute", ... 
 *			display.next(); // ... then to "Second". 
 * 
 *		When the cursor is set to `Normal' mode, the `Display' object always 
 *		refreshes the display hardware on each call to `refresh()'. In `Edit' 
 *		mode the the display hardware is only refreshed if specifically 
 *		requested by the client. This is to ease processor load and display 
 *		management when, for instance, users are editting settings, which 
 *		occur much less frequently and at slower rates than when an 
 *		application is running. The `Display::print' method is used to force 
 *		a display refresh, usually in response to changes in settings made by 
 *		the user.
 * 
 *			// Display value changes in edit mode won't be reflected unless:
 *			display.print();
 * 
 *		Two member methods, `clear()' and `blink()' are also implemented to 
 *		facilitate the common tasks of clearing the display or making it blink 
 *		at a specified rate, i.e. to indicate that something needs the user's 
 *		attention. The blink timing is handled by the `Display' class and API 
 *		calls implemented by a concrete derived class.
 * 
 *	**************************************************************************/

#if !defined DISPLAY_H__
#define DISPLAY_H__ 20210426L

#include <stdio.h>		// `sprintf()'
#include "array.h"		// `ArrayWrapper' type.
#include "IDisplay.h"	// `IDisplay' interface.
#include "IComponent.h"	// `IComponent' interface.
#include "IClockable.h"	// `IClockable' interface.
#include "Timer.h"		// `Timer' type.

class Display : public IDisplay, public IComponent, public IClockable
{
public:
	// Type that encapsulates information about a display field.
	struct Field
	{
		uint8_t		col_;	// Column specifier.
		uint8_t		row_;	// Row specifier.

		explicit Field(uint8_t col = 0, uint8_t	row = 0);

		bool operator==(const Field& other) const;

		bool operator<(const Field& other) const;
	}; 
	// Type that encapsulates a collection of display fields and print format specifiers.
	class Screen
	{
	public:
		using container_type = ArrayWrapper<const Field>;
		using const_iterator = container_type::const_iterator;
		using size_type = container_type::size_type;

	public:
		template <size_t SzFields, size_t SzRows>
		Screen(const char* label, const Field(&)[SzFields], const char* (&)[SzRows]);
		Screen(const char* label, const Field[], size_t sz_fields, const char* [], size_t sz_rows);
		Screen(const char* label, const Field*, const Field*, const char**, const char**);

	public:
		template<class ... Args>
		const char* operator()(char*, size_t, Args ...) const;

	public:
		const_iterator begin() const;
		const_iterator end() const;
		size_type size() const;
		const char* label() const;

	private:
		container_type fields_;
		ArrayWrapper<const char*> rows_;
		const char* label_;
	};
	// Enumerates the valid display "modes".
	enum class Cursor
	{
		Normal = 0, // Normal operating mode.
		Block,		// Block mode.
		Edit		// Editing mode.
	};
	// Client callback type.
	using Callback = void(*)();

private:
	// Enumerates the valid update event types.
	enum class Update : uint8_t
	{
		None = 0x00,	// No event.
		Cursor = 0x01,	// Cursor change event.
		Field = 0x02,	// Field change event.
		Display = 0x04,	// Display/noDisplay event.
		Print = 0x08,	// Print to display event.
		Clear = 0x10	// Clear display event.
	};
	// Type that encapsulates update events encoded as a bitfield.
	class Event
	{
	public:
		Event(Update u = Update::None) : val_(static_cast<uint8_t>(u)) {}

	public:
		// Sets the specified event in the bitfield.
		void set(Update u) { val_ |= static_cast<uint8_t>(u); }
		// Clears the specified event in the bitfield.
		void clr(Update u) { val_ &= ~(static_cast<uint8_t>(u)); }
		// Checks whether the specified event is currently set.
		uint8_t operator==(Update u) { return val_ & static_cast<uint8_t>(u); }
		// Assigns the specified event, overwriting the previous value in the bitfield.
		void operator=(Update u) { val_ = static_cast<uint8_t>(u); }

	private:
		uint8_t val_;	// The current events, encoded as a bitfield.
	};

public:
	explicit Display(Callback, const Screen* screen = nullptr);

public:
	// Sets the screen object.
	void			screen(const Screen*);
	// Returns the current screen object.
	const Screen*	screen() const; 
	// Returns the current field object.
	const Field*	field() const;
	// Sets the display cursor.
	void			cursor(Cursor);
	// Returns the current display cursor.
	Cursor			cursor() const;
	// Sets the display/nodisplay mode.
	bool			display() const;
	// Sets the clear display event.
	void			clear();
	// Sets the display blink mode.
	void			blink(msecs_t interval = 0);
	// Sets the Update::Print event.
	void			print();
	// Advances to the next field in the current screen object.
	void			next();
	// Advances to the previous field in the current screen object.
	void			prev();
	// Refreshes the display device.
	void			refresh() override;

private:
	// Sets the Update::Cursor event.
	void			updateCursor();
	// Sets the Update::Field event.
	void			updateField();
	// Sets the Update::Display event.
	void			updateDisplay(bool);
	// Sets the Update::Clear event.
	void			clearDisplay();
	// Calls the `refresh()' method.
	void			clock() override;
	// Clears the display.
	virtual void	display_clear() = 0;
	// Enables/disables the display.
	virtual void	display_enable(bool) = 0;
	// Sets the display cursor.
	virtual void	display_cursor(Cursor) = 0;
	// Sets the display field.
	virtual void	display_field(const Field*) = 0;

private:
	const Screen*	screen_;		// The current `Screen' object.
	const Field*	field_;			// The current `Field' object.
	Cursor			cursor_;		// The current display cursor setting.
	bool			display_;		// The current display/nodisplay setting.
	Callback		callback_;		// Client callback object.
	Event			event_;			// The current events object.
	Timer			blink_timer_;	// Display blink timer.
};

// Type used to draw an animated symbol on the display.
class Spinner
{
private:
	using container_type = ArrayWrapper<const char>;
	using const_iterator = container_type::const_iterator;

public:
	template<size_t Size>
	Spinner(const char(&)[Size], uint8_t);
	Spinner(const char [], size_t, uint8_t);
	Spinner(const char*, const char*, uint8_t);

public:
	// Returns the next character in the animation.
	const char spin();
	bool& visible();
	const bool& visible() const;

private:
	const char getNext();

private:
	container_type chars_;	// The collection of animation characters.
	const_iterator it_;	// Character collection iterator.
	uint8_t div_;	// Divisor used to set the animation rate.
	uint8_t rate_;	// The current animation rate.
	bool visible_;
};

#pragma region Spinner Impl
template<size_t Size>
Spinner::Spinner(const char(&c)[Size], uint8_t div) :
	chars_(c, Size), it_(chars_.begin()), div_(div), rate_(div), visible_()
{

}
#pragma endregion

#pragma region Screen Impl
template <size_t SzFields, size_t SzRows>
Display::Screen::Screen(const char* label, const Field(&fields)[SzFields], const char* (&rows)[SzRows]) :
	fields_(fields), rows_(rows), label_(label)
{

}

template<class ... Args>
const char* Display::Screen::operator()(char* buf, size_t row, Args ... args) const
{
	(void)sprintf(buf, rows_[row], args ...);

	return buf;
}
#pragma endregion

#endif // !defined DISPLAY_H__
