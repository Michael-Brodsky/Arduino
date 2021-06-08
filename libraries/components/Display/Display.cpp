#include "Display.h"

#pragma region Display Impl

Display::Display(Callback callback, const Screen* screen) :
	screen_(screen), field_(screen->begin()), cursor_(),
	callback_(callback), event_(), blink_timer_(), display_(true)
{

}

void Display::screen(const Screen* screen)
{
	print(); // Always print the display on a new screen.
	screen_ = screen;
	field_ = screen_->begin();
}


const Display::Screen* Display::screen() const
{
	return screen_;
}

const Display::Field* Display::field() const
{
	return field_;
}

void Display::cursor(Cursor cursor)
{
	cursor_ = cursor;
	updateCursor();
}

Display::Cursor Display::cursor() const
{
	return cursor_;
}

bool Display::display() const
{
	return display_;
}

void Display::clear()
{
	clearDisplay();
}

void Display::blink(msecs_t interval)
{
	if (interval)
		blink_timer_.start(interval);	// Blink display on interval != 0, ...
	else
	{
		blink_timer_.stop();			// ... otherwise stop blinking.
		updateDisplay(true);
	}
}

void Display::print()
{
	event_.set(Update::Print);
	updateField();
}

void Display::next()
{
	if (++field_ == screen_->end())
		field_ = screen_->begin();
	updateField();
}

void Display::prev()
{
	if (field_ == screen_->begin())
		field_ = screen_->end();
	--field_;
	updateField();
}

void Display::refresh()
{
	if (cursor_ == Display::Cursor::Normal)
		event_.set(Update::Print); // Always print in Normal mode.
	if (blink_timer_.expired()) // Handle display blinking and timer.
	{
		updateDisplay(!display_);
		blink_timer_.reset();
	}
	if (event_ == Update::Display)
		display_enable(display_);
	if (event_ == Update::Clear)
		display_clear();
	if (event_ == Update::Cursor)
		display_cursor(cursor_);
	if(event_ == Update::Print && callback_)
		(*callback_)();
	if (event_ == Update::Field)
		display_field(field_);
	event_ = Update::None;	// Reset the Event flags.

}

void Display::updateCursor()
{
	event_.set(Update::Cursor);
}

void Display::updateField()
{
	event_.set(Update::Field);
}

void Display::updateDisplay(bool display)
{
	if (!display)
		event_ = Update::None; // Clear all other events when blanking the display.
	event_.set(Update::Display);
	display_ = display;
}

void Display::clearDisplay()
{
	event_.set(Update::Clear);
}

void Display::clock()
{
	refresh();
}
#pragma endregion

#pragma region Field Impl
Display::Field::Field(uint8_t col, uint8_t row) :
	col_(col), row_(row)
{}

bool Display::Field::operator==(const Field& other) const
{
	return row_ == other.row_ && col_ == other.col_;
}

bool Display::Field::operator<(const Field& other) const
{
	return row_ < other.row_ || (row_ == other.row_ && col_ < other.col_);
}
#pragma endregion

#pragma region Screen Impl
Display::Screen::Screen(const char* label, const Field fields[], size_t sz_fields, const char* rows[], size_t sz_rows) :
	fields_(fields, sz_fields), rows_(rows, sz_rows), label_(label) 
{

}

Display::Screen::Screen(const char* label, const Field* first, const Field* last, const char** begin, const char** end) :
	fields_(first, last), rows_(begin, end), label_(label)
{

}
Display::Screen::const_iterator Display::Screen::begin() const 
{ 
	return fields_.begin(); 
}

Display::Screen::const_iterator Display::Screen::end() const 
{ 
	return fields_.end(); 
}

Display::Screen::size_type Display::Screen::size() const 
{ 
	return fields_.size(); 
}

const char* Display::Screen::label() const
{
	return label_;
}
#pragma endregion

#pragma region Spinner Impl
Spinner::Spinner(const char c[], size_t size, uint8_t div) :
	chars_(c, size), it_(chars_.begin()), div_(div), rate_(div), visible_()
{

}

Spinner::Spinner(const char* first, const char* last, uint8_t div) :
	chars_(first, last), it_(chars_.begin()), div_(div), rate_(div), visible_()
{

}

const char Spinner::spin()
{
	const char invisible = ' ';

	return visible_ ? getNext() : invisible;
}

bool& Spinner::visible()
{
	return visible_;
}

const bool& Spinner::visible() const
{
	return visible_;
}

const char Spinner::getNext()
{
	if (!(div_--))
	{
		if (++it_ == chars_.end())
			it_ = chars_.begin();
		div_ = rate_;
	}

	return *it_;
}
#pragma endregion

