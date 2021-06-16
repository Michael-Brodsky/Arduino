/*
 *	This files declares an asynchronous character display manager component.
 *
 *	***************************************************************************
 *
 *	File: Display.h
 *	Date: June 15, 2021
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
 *	**************************************************************************/

#if !defined DISPLAY_H__
# define DISPLAY_H__ 20210615L

# include <stdio.h>			// `sprintf()'
# include "LiquidCrystal.h"	// Arduino `LiquidCrystal' API.
# include "array.h"			// `ArrayWrapper' type.
# include "IComponent.h"	// `IComponent' interface.
# include "IClockable.h"	// `IClockable' interface.
# include "Timer.h"			// `Timer' type.

class Display : public IComponent, public IClockable
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
		const_iterator	begin() const;
		const_iterator	end() const;
		size_type		size() const;
		const char*		label() const;

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
		explicit Event(Update u = Update::None) : val_(static_cast<uint8_t>(u)) {}

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
	explicit Display(LiquidCrystal& lcd, Callback, const Screen* screen = nullptr);

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
	void			refresh();

private:
	// Sets the Update::Cursor event.
	void	updateCursor();
	// Sets the Update::Field event.
	void	updateField();
	// Sets the Update::Display event.
	void	updateDisplay(bool);
	// Sets the Update::Clear event.
	void	clearDisplay();
	// Calls the `refresh()' method.
	void	clock() override;

private:
	LiquidCrystal&	lcd_;
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
	Spinner(const char[], size_t, uint8_t);
	Spinner(const char*, const char*, uint8_t);

public:
	// Returns the next character in the animation.
	const char	spin();
	bool&		visible();
	const bool& visible() const;

private:
	const char	getNext();

private:
	container_type	chars_;		// The collection of animation characters.
	const_iterator	it_;		// Character collection iterator.
	uint8_t			div_;		// Divisor used to set the animation rate.
	uint8_t			rate_;		// The current animation rate.
	bool			visible_;	// Flag indicating whether the spinner is currently visible.
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