/*
 *	This files declares an asynchronous LCD display management class.
 *
 *	***************************************************************************
 *
 *	File: Display.h
 *	Date: April 17, 2021
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
 *		The `Display' class template can be used to asynchronously manage a 
 *		character display by refreshing it as needed, changing the appearance 
 *		of the display cursor, automatically positioning it at specified
 *		locations and formatting text for printing to the display hardware. 
 *		This makes it useful for implementing user interfaces where the 
 *		display behaviors or appearance need to change in response to external
 *		inputs, such as when a user edits application settings. 
 *
 *		The template parameters indicate the number of columns and rows 
 *		supported by the display hardware.
 * 
 *			using DisplayType = Display<16, 2>; // Type alias for `Display<16, 2>'.
 *			DisplayType display;				// Instantiates a 16x2 display object.
 *
 *		Behaviors are implemented using a "screen" and "field" pattern where a
 *		field is a particular position (col/row) on the display, and a screen
 *		is a collection of such fields. They are represented by the two nested 
 *		types: `Screen' and `Field'. The `Screen' type also encapsulates print 
 *		formatting and allow clients to print an entire formatted screen to 
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
 *		Clients are responsible for managing the actual display hardware, i.e 
 *		through the Arduino `LiquidCrystal' API or other mechanism. Client 
 *		communication is handled via callbacks and "event messages". Clients 
 *		must implement a callback function that is passed to the `Display' 
 *		class constructor and which `Display' objects call and pass messages 
 *		to whenever the display hardware needs attention.
 * 
 *			// Client callback function.
 *			void displayCallback(DisplayType::Event);
 * 
 *			// Client instantiates a display object with the callback and 
 *			// optionally, the default display screen.
 *			DisplayType display(&displayCallback, &screen);      
 * 
 *			// Client instantiates the display hardware API/mechanism:
 *			LiquidCrystal lcd( ... );
 * 
 *			// Display hardware can be updated synchronously with one line of code:
 *			void loop() { 
 *				display.refresh();
 *				... 
 *			}
 * 
 *			// or asynchronously with the `clock()' method.
 * 
 *			// Callback function implements display event handling logic:
 *			void displayCallback(DisplayType::Event event) {   
 *				... 
 *				if (event == DisplayType::Update::Print)
 *					printDisplay(); 
 *				...
 *			}
 * 
 *			// Client implements functions to handle printing and other events.
 *			void printDisplay() { 
 *				char buf[17];	 // Print formatting buffer.
 *				lcd.print(screen(buf, 0, hour, minute, second); // Prints screen from client-supplied parameters.
 *				...
 *			}
 * 
 *		PROTIP ON PRINTING AND LIMITATIONS OF THE ARDUINO `LiquidCrystal' API.
 * 
 *		The Arduino API can only print a single row of chars to the hardware 
 *		at a time. Therefore, the `Screen' class and its associated methods 
 *		are fashioned on a "per row" basis. `Screen' objects have a member 
 *		array whose size is determined by the `Display' `Rows' template 
 *		parameter at time of construction and initialized with values passed 
 *		to the constructor. The values are the print format specifiers for each 
 *		row on the display hardware. So for a two row display, upto two format 
 *		specifiers are passed: 
 * 
 *			const char row1[] = "%02u:%02u:%02u";
 *			const char row2[] = "%s %s";
 *			DisplayType::Screen screen(fields, row1, row2);  
 * 
 *		In the client print event handling function, the entire display can be 
 *		printed thus:
 * 
 *		void printDisplay()
 *		{ 
 *			char buf[Cols + 1];	   // Buffer size set from `Display' `Cols' template parameter.
 *			uint8_t row = 0;	   // Keeps track of current row index. 
			lcd.setCursor(0, row); // Cursor must be positioned for each row.
			lcd.print(screen(buf, row, ... ); // Row formatting handled by `Screen' object.
			lcd.setCursor(0, ++row);		  // Position, ...
			lcd.print(screen(buf, row, ... ); // ... format and print next row(s).
 *		}
 * 
 *		The `Screen::format' method can be used to assign print format 
 *		specifiers at runtime:
 * 
 *			screen.format(row1, row2);
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
 *		and implement the functionality in the callback function.
 * 
 *			void displayCallback(DisplayType::Event event) {   
 *				... 
 *				if (event == DisplayType::Update::Cursor)
 *					switch (display.cursor())
 *					{
 *					case DisplayType::Cursor::Normal:	// Hide the cursor
 *						lcd.noCursor();
 *						lcd.noBlink();
 *						break;
 *					case DisplayType::Cursor::Edit:		// Show the blinking block cursor.
 *						lcd.cursor();
 *						lcd.blink();
 *						break;
 *					default:
 *						break;
 *					}
 *				}
 *				...
 *			}
 *
 *		Clients can assign the current screen and navigate its fields using 
 *		the `screen()', `next()' and `prev()' methods. Using the above 
 *		example:
 * 
 *			display.screen(screen); // Set the current screen object, field initialized to "Hour".
 *			display.next(); // field advances to "Minute", ... 
 *			display.next(); // ... then to "Second". 
 * 
 *		Again, clients must implement the actual API calls:
 * 
 *			void displayCallback(DisplayType::Event event) 
 *			{
 *				... 
 *				if (event == DisplayType::Update::Field) 
 *				{
 *					const DisplayType::Field& field = *display.field();
 *					lcd.setCursor(field.col_, field.row_);
 *				}
 *			}
 * 
 *		When the cursor is set to `Normal' mode, the `Display' object always 
 *		issues the `Update::Print' event and the display hardware is updated 
 *		on each call to `refresh()'. In `Edit' mode the `Update::Print' event 
 *		is not issued unless specifically requested by the client. This is to 
 *		ease processor load and display management when users are editting 
 *		settings, which occur much less frequently and at slower rates than 
 *		when an application is "running". The `Display::print' method is 
 *		used to force a `Update::Print' event, usually in response to changes 
 *		in settings made by the user. 
 * 
 *			// Display value changes in edit mode won't be reflected unless:
 *			display.print();
 * 
 *		Two member methods, `clear()' and `blink()' are also implemented to 
 *		facilitate the common tasks of clearing the display or making it blink 
 *		at a specified rate, i.e. to indicate that something needs the user's 
 *		attention. The blink timing is handled by the `Display' class and API 
 *		calls implemented by the client:
 * 
 *			void displayCallback(DisplayType::Event event) 
 *			{ 
 *				... 
 *				if (event == DisplayType::Update::Display) 
 *					display.display() ? lcd.display() : lcd.noDisplay(); 
 *				if (event == DisplayType::Update::Clear) 
 *					lcd.clear(); 
 *			}
 * 
 *	EVENT HANDLING ORDER:
 * 
 *		In order to make the display management work properly, two 
 *		requirements have been imposed on the order in which events must be 
 *		handled: 
 * 
 *			1) The `DisplayType::Update::Clear' event must be handled first, 
 *			2) The `DisplayType::Update::Field' event must be handled last.
 * 
 *		Other events can be handled in any order between `Clear' and `Field'.
 *		This is obvious because clearing the display after updating it would 
 *		erase all the updates and, updates such as printing move the cursor 
 *		past the last character printed. So, in the case where settings are 
 *		being adjusted by the user in a particular field, the cursor must be 
 *		repositioned to the field coordinates after each time the client 
 *		updates the display with the adjusted values. 
 * 
 *	**************************************************************************/

#if !defined DISPLAY_H__
#define DISPLAY_H__ 20210417L

#include <stdio.h>		// `sprintf()'
#include <array.h>		// `std_array' and `ArrayWrapper' types.
#include <IComponent.h>	// `IComponent' interface.
#include <IClockable.h>	// `IClockable' interface.
#include <Timer.h>		// `Timer' type.

template<size_t Cols, size_t Rows>
class Display : public IClockable, public IComponent 
{
public:
	// Type that encapsulates information about a display field.
	struct Field
	{
		uint8_t		col_;	// Column specifier.
		uint8_t		row_;	// Row specifier.
		const char* fmt_;	// Optional print formatting specifier.

		explicit Field(uint8_t col = 0, uint8_t	row = 0, const char* fmt = nullptr) : 
			col_(col), row_(row), fmt_(fmt) 
		{}
	}; 

	// Type that encapsulates a collection of display fields and print format specifiers.
	class Screen : public ArrayWrapper<Field>
	{
	public:
		using container_type = ArrayWrapper<Field>; // `Field' container type alias.
		using format_type = std_array<const char*, Rows>; // Print format specifier container type alias.
		using value_type = typename container_type::value_type;
		using iterator = typename container_type::iterator;

	public:
		template <size_t Size>
		explicit Screen(value_type(&fields)[Size]); 

		template <size_t Size, class ... Args>
		Screen(value_type(&fields)[Size], Args ... args); 

		Screen(value_type fields[], size_t size); 

		template <class ... Args>
		Screen(value_type fields[], size_t size, Args ... args); 

		Screen(iterator begin, iterator end); 

		template <class ... Args>
		Screen(iterator begin, iterator end, Args ... args); 

	public:
		// Sets the print formatting specifiers collection. 
		template<class ... Args>
		void		format(Args ... args);

		// Prints to and returns a buffer of formatted text from the supplied parameters.
		template<class ... Args>
		const char* operator()(char* buf, size_t row, Args ... args) const;

	private: 
		format_type fmt_;	// The current print formatting specifiers collection.
	};

	// Enumerates the valid event message types.
	enum class Update : uint8_t
	{
		None = 0x00,	// No event.
		Cursor = 0x01,	// Cursor change event.
		Field = 0x02,	// Field change event.
		Display = 0x04,	// Display/noDisplay event.
		Print = 0x08,	// Print to display event.
		Clear = 0x10	// Clear display event.
	};

	// Type that encapsulates display events encoded as a bitfield.
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

	// Enumerates the valid display "modes".
	enum class Cursor
	{
		Normal = 0, // Normal operating mode.
		Edit		// Editing mode.
	};

	// Client callback type.
	using Callback = void(*)(Event);

public:
	Display() = default;
	explicit Display(Callback, Screen* screen = nullptr);

public:
	// Sets the screen object.
	void	screen(Screen*);
	// Returns the current screen object.
	Screen* screen() const; 
	// Returns the current field object.
	Field*	field() const;
	// Sets the display cursor.
	void	cursor(Cursor);
	// Returns the current display cursor.
	Cursor	cursor() const;
	// Sets the display/nodisplay mode.
	bool	display() const;
	// Sets the clear display event.
	void	clear();
	// Sets the display blink mode.
	void	blink(msecs_t interval = 0);
	// Sets the Update::Print event.
	void	print();
	// Advances to the next field in the current screen object.
	void	next();
	// Advances to the previous field in the current screen object.
	void	prev();
	// Calls the client callback function with the current event messages.
	void	refresh();

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
	Screen*		screen_;		// The current `Screen' object.
	Field*		field_;			// The current `Field' object.
	Cursor		cursor_;		// The current display cursor setting.
	bool		display_;		// The current display/nodisplay setting.
	Callback	callback_;		// Client callback object.
	Event		event_;			// The current events object.
	Timer		blink_timer_;	// Display blink timer.
};

#pragma region Display Impl

template<size_t Cols, size_t Rows>
Display<Cols, Rows>::Display(Callback callback, Screen* screen) : 
	screen_(screen), field_(screen->begin()), cursor_(), 
	callback_(callback), event_(), blink_timer_(), display_(true) 
{

}

template<size_t Cols, size_t Rows>
void Display<Cols, Rows>::screen(Screen* screen) 
{ 
	field_ = screen_->begin();
	print(); // Always print the display on a new screen.
	screen_ = screen; 
}

template<size_t Cols, size_t Rows>
	typename Display<Cols, Rows>::Screen* 
		Display<Cols, Rows>::screen() const 
{ 
	return screen_; 
}

template<size_t Cols, size_t Rows>
	typename Display<Cols, Rows>::Field* 
		Display<Cols, Rows>::field() const 
{ 
	return field_; 
}

template<size_t Cols, size_t Rows>
void Display<Cols, Rows>::cursor(Cursor cursor)
{
	field_ = screen_->begin();
	cursor_ = cursor;
	updateCursor();
	updateField();
}

template<size_t Cols, size_t Rows>
	typename Display<Cols, Rows>::Cursor 
		Display<Cols, Rows>::cursor() const
{
	return cursor_;
}

template<size_t Cols, size_t Rows>
bool Display<Cols, Rows>::display() const
{
	return display_;
}

template<size_t Cols, size_t Rows>
void Display<Cols, Rows>::clear()
{
	clearDisplay();
}

template<size_t Cols, size_t Rows>
void Display<Cols, Rows>::blink(msecs_t interval)
{
	if (interval)
		blink_timer_.start(interval);	// Blink display on interval != 0, ...
	else
	{
		blink_timer_.stop();			// ... otherwise stop blinking.
		updateDisplay(true);
	}
}

template<size_t Cols, size_t Rows>
void Display<Cols, Rows>::print()
{
	event_.set(Update::Print);
	updateField();
}

template<size_t Cols, size_t Rows>
void Display<Cols, Rows>::next()
{
	if (++field_ == screen_->end())
		field_ = screen_->begin();
	updateField();
}

template<size_t Cols, size_t Rows>
void Display<Cols, Rows>::prev()
{
	if (field_ == screen_->begin())
		field_ = screen_->end();
	--field_;
	updateField();
}

template<size_t Cols, size_t Rows>
void Display<Cols, Rows>::refresh()
{
	if (cursor_ == Display::Cursor::Normal)
		event_.set(Update::Print); // Always print in Normal mode.
	if (blink_timer_.expired()) // Handle display blinking and timer.
	{
		updateDisplay(!display_);
		blink_timer_.reset();
	}
	(*callback_)(event_);
	event_ = Update::None;	// Reset the Event flags.

}

template<size_t Cols, size_t Rows>
void Display<Cols, Rows>::updateCursor()
{
	event_.set(Update::Cursor);
}

template<size_t Cols, size_t Rows>
void Display<Cols, Rows>::updateField()
{
	event_.set(Update::Field);
}

template<size_t Cols, size_t Rows>
void Display<Cols, Rows>::updateDisplay(bool display)
{
	if (!display)
		event_ = Update::None; // Clear all other events when blanking the display.
	event_.set(Update::Display);
	display_ = display;
}

template<size_t Cols, size_t Rows>
void Display<Cols, Rows>::clearDisplay()
{
	event_.set(Update::Clear);
}

template<size_t Cols, size_t Rows>
void Display<Cols, Rows>::clock()
{
	refresh();
}
#pragma endregion

#pragma region Screen Impl

template<size_t Cols, size_t Rows>
template <size_t Size>
Display<Cols, Rows>::Screen::Screen(
	typename Display<Cols, Rows>::Screen::value_type (&fields)[Size]) :
		container_type(fields), fmt_() 
{

}

template<size_t Cols, size_t Rows>
template <size_t Size, class ... Args>
Display<Cols, Rows>::Screen::Screen(
	typename Display<Cols, Rows>::Screen::value_type(&fields)[Size], Args ... args) :
		container_type(fields), fmt_({ args ... }) 
{

}

template<size_t Cols, size_t Rows>
Display<Cols, Rows>::Screen::Screen(
	typename Display<Cols, Rows>::Screen::value_type fields[], size_t size) : 
		container_type(fields, size), fmt_() 
{

}

template<size_t Cols, size_t Rows>
template <class ... Args>
Display<Cols, Rows>::Screen::Screen(
	typename Display<Cols, Rows>::Screen::value_type fields[], size_t size, Args ... args) : 
		container_type(fields, size), fmt_({ args ... }) 
{

}

template<size_t Cols, size_t Rows>
Display<Cols, Rows>::Screen::Screen(
	typename Display<Cols, Rows>::Screen::iterator begin, 
	typename Display<Cols, Rows>::Screen::iterator end) : 
		container_type(begin, end), fmt_() 
{

}

template<size_t Cols, size_t Rows>
template <class ... Args>
Display<Cols, Rows>::Screen::Screen(
	typename Display<Cols, Rows>::Screen::iterator begin, 
	typename Display<Cols, Rows>::Screen::iterator end, Args ... args) :
		container_type(begin, end), fmt_({ args ... }) 
{

}

template<size_t Cols, size_t Rows>
template<class ... Args>
void Display<Cols, Rows>::Screen::format(Args ... args)
{ 
	fmt_ = { args ... }; 
}

template<size_t Cols, size_t Rows>
template<class ... Args>
const char* Display<Cols, Rows>::Screen::operator()(char* buf, size_t row, Args ... args) const
{
	(void)sprintf(buf, fmt_[row], args ...);

	return buf;
}

#pragma endregion

#endif // !defined DISPLAY_H__