#include "Display.h"

Display::Display(LiquidCrystal& lcd, Callback callback, Screen* screen) :
	lcd_(lcd), mode_(), screen_(screen), field_(), set_field_(), 
	print_display_(), callback_(callback), display_(true), blink_timer_()
{
	
}

void Display::refresh()
{
	switch (mode_)
	{
	case Display::Mode::Run:		// Always refresh in Run mode.
		(*callback_)();
		if (blink_timer_.expired()) // Handle display blinking and timer.
		{
			setDisplay(!display_);
			blink_timer_.reset();
		}
		break;
	case Display::Mode::Set:		// Only refresh in Set mode if a value changed.
		if (print_display_)
		{
			(*callback_)();
			setField();				// Writing to LiquidCrystal moves cursor past last char, ... 
			print_display_ = false;	// ... so reposition it back to current field.
		}
		else if (set_field_)
		{
			setField();
			set_field_ = false;
		}
		break;
	default:
		break;
	}
}

void Display::mode(Mode mode)
{
	field_ = screen_->begin();
	mode_ = mode;
	setCursor();
	setField();
}

void Display::field(iterator field)
{
	field_ = field;
	set_field_ = true;
}

void Display::nextField()
{
	if (++field_ == screen_->end())
		field_ = screen_->begin();
	set_field_ = true;
}

void Display::prevField()
{
	if (field_ == screen_->begin())
		field_ = screen_->end();
	--field_;
	set_field_ = true;
}

void Display::screen(Screen* screen)
{
	screen_ = screen;
	field_ = screen_->begin();	// Activate the screen's first field and ...
	print_display_ = true;		// ... always refresh when the screen changes.
}

void Display::needsRefresh()
{
	print_display_ = true;
}

void Display::setCursor()
{
	switch (mode_)
	{
	case Display::Mode::Run:	// `Run' mode cursor is not visible.
		lcd_.noCursor();
		lcd_.noBlink();
		break;
	case Display::Mode::Set:	// `Set' mode cursor is flashing block.
		lcd_.cursor();
		lcd_.blink();
		break;
	default:
		break;
	}
}

void Display::setField()
{
	lcd_.setCursor((*field_).col_, (*field_).row_);
}

void Display::blink(msecs_t interval)
{
	if (interval)
		blink_timer_.start(interval);	// Blink display on interval != 0, ...
	else
	{
		blink_timer_.stop();			// ... otherwise stop blinking.
		setDisplay(true);
	}
}

void Display::setDisplay(bool display)
{
	if (display)
		lcd_.display();
	else
		lcd_.noDisplay();
	display_ = display;
}

void Display::clock()
{
	refresh();
}