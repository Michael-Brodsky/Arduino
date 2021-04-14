/*
 *	This files declares an asynchronous LCD display management class.
 *
 *	***************************************************************************
 *
 *	File: Display.h
 *	Date: April 6, 2021
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
 *		The `Display' class can be used to asynchronously manage an LCD 
 *		display device by refreshing it as needed, changing the appearance of 
 *		the display cursor and automatically positioning it at specified 
 *		locations. This makes it useful for implementing user interfaces where 
 *		the display behaviors or appearance change in response to external 
 *		inputs, such as when a user edits application settings.
 *
 *		Behaviors are implemented using a "screen" and "field" pattern where a 
 *		field is a particular position (col/row) on the display, and a screen 
 *		is a collection of such fields. Clients can iterate (scroll) through 
 *		a screen's fields or change the screen to use a different set of 
 *		fields. Actual display device updates are handled by the client since 
 *		they are application-specific. Therefore, the `Display' class executes 
 *		a client-supplied callback that handles device updates.
 * 
 *		Consider a digital clock that displays a formatted time and date on an 
 *		LCD display:
 * 
 *			06 Apr 2021
 *			Tue 12:15:59
 * 
 *		The date has three fields, day, month and year, while the time has 
 *		four, day of the week, hour, minute and second, each displayed at a 
 *		particular position. If, for instance, the clock is part of a 
 *		digital thermostat application that allows users to set the 
 *		temperature for a particular time of day, the client can switch to a
 *		to a screen containing the temperature and time of day fields:
 * 
 *			72F   11:00:00
 * 
 *		and use the same methods to scroll through the fields and display  
 *		or change their current value.
 * 
 *		The `Display' class can be set to one of two operating modes: `Run' or 
 *		`Set'. In `Run' mode, the display cursor is not visible, in `Set' mode 
 *		it's a flashing block. The cursor is automatically positioned in the 
 *		active field and advanced using the `nextField()' and `prevField()' 
 *		methods or set manually with the `field()' method. The operating mode 
 *		is set with the `mode()' method. The screen is changed using the 
 *		`screen()' method.
 * 
 *		`Display' objects can be operated asynchronously with the `clock()' 
 *		method (see IClockable.h). This allows clients to refresh the display 
 *		device periodically without blocking the rest of the application. 
 *		Consider, for instance, that a digital clock still needs to run even 
 *		while it's being set by the user.
 * 
 *		In `Set' mode, the display device is not refreshed automatically on 
 *		each call to the `clock()' method. This is to reduce overhead and 
 *		processor load, for instance, while users edit application settings.
 *		Clients can force a refresh with the `needsRefresh()' method which 
 *		only has to be called in response to events that actually modify 
 *		the display. In `Run' mode the display device is always refreshed.
 *		The display can be refreshed synchronously with the `refresh()' 
 *		method, which is what the `clock()' method calls.
 *		
 *	**************************************************************************/

#if !defined DISPLAY_H__
#define DISPLAY_H__ 20210406L

#include "iterator.h"		// std_begin(), std_end() & std_distance()
#include "IClockable.h"	// `IClockable' and `ICommand' interfaces.
#include "Curses.h"		// `Cursor' type.
#include "Timer.h"			// `Timer' type.

// Asynchronous LCD display management type.
class Display : public IClockable
{
public:
	// Enumerates the valid display modes.
	enum class Mode 
	{
		Run,	// Display run mode.
		Set		// Display set mode.
	};

	// Type that encapsulates a collection of display fields.
	class Screen
	{
	public:
		// Display field type.
		struct Field
		{
			uint8_t		col_;	// Field display column.
			uint8_t		row_;	// Field display row.

			Field(uint8_t col = 0, uint8_t row = 0) : col_(col), row_(row) {}
		};

		using iterator = Field*;	// Screen iterator type.

	public:
		template <size_t Size>
		explicit Screen(Field (&fields)[Size]) : begin_(fields), end_(fields + Size) {}
		Screen(Field fields[], size_t size) : begin_(fields), end_(fields + size) {}
		Screen(iterator begin, iterator end) : begin_(begin), end_(end) {}

	public:
		// Returns an iterator to the beginning of the current Fields collection.
		iterator begin() const { return begin_; }
		// Returns an iterator to the end of the current Fields collection.
		iterator end() const { return end_; }
		// Returns the number of elements in the current Fields collection.
		size_t	size() const { return std_distance(begin_, end_); }

	private:
		iterator begin_;	// Points to the beginning of the current Fields collection.
		iterator end_;		// Points to the end of the current Fields collection.
	};

	using iterator = Screen::iterator;	// Display iterator type.
	using Callback = void(*)();

public:
	Display(LiquidCrystal&, Callback, Screen* screen = nullptr);

public:
	// Refreshes the LCD display device.
	void refresh();
	// Sets the display mode.
	void mode(Mode);
	// Sets the current field.
	void field(iterator);
	// Moves the cursor to the next field of the current screen.
	void nextField();
	// Moves the cursor to the previous field of the current screen.
	void prevField();
	// Sets the current screen.
	void screen(Screen*); 
	// Notifies the display that it requires a refresh.
	void needsRefresh();
	// Starts/stops display blinking.
	void blink(msecs_t interval = 0);

private:
	// Sets the display cursor.
	void setCursor();
	// Moves the cursor to the current field.
	void setField();
	// Displays/blanks the LCD device.
	void setDisplay(bool);
	// Calls the `refresh()' method.
	void clock() override;

private:
	LiquidCrystal&		lcd_;			// References the LCD display device.
	Mode				mode_;			// The current display mode.
	Screen*				screen_;		// The current screen.
	Screen::iterator	field_;			// The current field on the current screen.
	bool				set_field_;		// Flag indicating whether to refresh the current field.
	bool				print_display_;	// Flag indicating whether to refresh the entire display.
	Callback			callback_;		// Client callback.
	bool				display_;		// Flag indicating the current display state when blinking.
	Timer				blink_timer_;	// Display blink timer.
};

#endif // !defined DISPLAY_H__