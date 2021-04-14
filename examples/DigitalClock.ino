/*
 *	This file demonstrates an object-oriented solution to implementing a
 *	digital clock on an Arduino Uno attached to an LCD/Keypad shield.
 *
 *	***************************************************************************
 *
 *	File: DigitalClock.ino
 *	Date: April 9, 2021
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
 *		The clock implementation behaves like a typical digital clock,
 *		displaying the current time and date in a particular format and also
 *		features an alarm function. Both the current and alarm time are
 *		adjustable with the keypad and the alarm time is always stored in the
 *		onboard EEPROM and recalled on power up.
 *
 *			01 Jan 1970
 *			Thu 00:00:00
 *
 *		On power up or after a hardware reset, the display shows the current
 *		date and time (if automatically synced) or flashes to indicate it
 *		needs to be manually set. Once set, it displays the current date and
 *		time steadily. The clock is toggled between "set" and "run" modes
 *		by the Select key. Short pressing the Select key sets the current
 *		date and time, longpressing it sets the alarm. When in "set" mode,
 *		the display cursor changes to a flashing block and highlights the
 *		"field" being adjusted.
 *
 *			█1 Jan 1970
 *			Thu 00:00:00
 *
 *		The field value can be incremented or decremented with the Up or Down
 *		keys respectively. Holding down the Up or Down keys causes them to
 *		"repeat" at a rate of 10 Hz and allows values to be adjusted quickly.
 *
 *		The Right and Left keys are used to scroll forward and backwards
 *		through the adjustable fields, one at a time. The cursor moves to the
 *		next or previous field and wraps around at the end or beginning field.
 *		The "day of week" field (Mon-Sun) is automatically updated by
 *		adjustments to the date fields, and is skipped when scrolling.
 *
 *			█1 Jan 1970				 01 █an 1970			  01 Jan █970
 *			Thu 00:00:00  ->Right->  Thu 00:00:00  ->Right->  Thu 00:00:00 ->Right->
 *
 *			01 Jan 1970				 01 Jan 1970			  01 Jan 1970
 *			Thu █0:00:00  ->Right->  Thu 00:█0:00  ->Right->  Thu 00:00:█0
 *
 *			(The Left key scrolls in the opposite direction.)
 *
 *		Once adjusted, the time is set by pressing the Select key again,
 *		which puts the display back to "run" mode.
 *
 *		The alarm is set by pressing and holding down the Select key until the
 *		display changes to "set" mode. The alarm is adjusted in the same way as
 *		the time, above. The alarm screen also has one additional field to
 *		indicate the alarm is being adjusted and toggle the alarm on and off.
 *
 *			█1 Jan 1970 ALRM
 *			Thu 00:00:00
 *
 *		The alarm is toggled on and off by scrolling to the ALRM field and
 *		changing the setting with the Up or Down keys. Either key toggles the
 *		setting. An asterisk (*) below ALRM on the display indicates the alarm
 *		is on and no asterisks indicates it's off.
 *
 *			01 Jan 1970 █LRM
 *			Thu 00:00:00   * <--- alarm is on.
 *
 *		Once adjusted, the alarm is set by pressing the Select key again,
 *		which puts the display back into "run" mode. If the alarm is on, the
 *		asterisk remains displayed.
 *
 *			01 Jan 1970
 *			Thu 00:00:00   * <--- alarm is on.
 *
 *	**************************************************************************
 *
 *	The purpose of this project is to demostrate the use of object-oriented
 *	methodologies, modern C++ design patterns and asynchronous (multi-tasking)
 *	operation, implemented with generic, reusable "components" that can be
 *	parameterized for different applications. The components are part of a
 *	larger library that forms a foundation for modern object-oriented C++
 *	development, provide basic services and enforces const-correctness and use
 *	of appropriate data types.
 *
 *	The application-level code in this file consists of only a finite state
 *	machine for high-level logic and a display printing function. The majority
 *	of the functional code is encapsulated in the global application objects
 *	which handle tasks commonly encountered in microcontroller development.
 *	They are generic enough to be reusable in many applications by simply
 *	changing their parameters. Detailed descriptions of each type are provided
 *	in their respective header files, and the reader is	referred there for
 *	more information. This application uses four main "components":
 *
 *		`Keypad' -  a type that encapsulates behaviors of a keypad attached to
 *					an analog GPIO input pin (see <AnalogKeypad.h>).
 *
 *		`Display'-  a type that encapsulates behaviors of an LCD display
 *					device (see <Display.h>).
 *
 *		`TaskScheduler' - an asynchronous task scheduler. Here, the
 *					scheduler is driven synchronously in the Arduino `loop()'
 *					function and so operates in a non-preemptive manner.
 *					If interrupt driven, it supports basic preemptive multi-
 *					tasking (see <TaskScheduler.h>).
 *
 *		`DigitalClock'  - a type that encapsulates behaviors of a simple
 *					digital clock. This component provides much of the
 *					application logic and services (see <DigitalClock.h>).
 *
 *		Components communicate via callbacks defined in this file.
 *
 *		There is also a separate `config.h' file that defines the hardware
 *		configuration and other application constants. This allows for
 *		support of varying display and keypad hardware with just a change of
 *		the file.
 *
 *	**************************************************************************/

 /****************
  * Dependencies *
  ****************/

#include <AnalogKeypad.h>
#include <Display.h>
#include <DigitalClock.h>
#include <TaskScheduler.h>
#include "config.h"

/*************************
 * Function Declarations *
 *************************/

 // Initializes the application.
void initialize();
// Keypad component callback.
void keypadCallback(const Keypad::Button&, Keypad::Event);
// Button longpress event handler.
void buttonLongpressEvent(const Keypad::Button&);
// Event notifier/dispatcher.
void notify(const Keypad::Button&);
// Display component callback.
void displayCallback();
// DigitalClock component callback.
void alarmCallback();

/******************************
 * Global Application Objects *
 ******************************/

 // Keypad button collection.
Keypad::Button buttons[] =
{
	Keypad::Button(ButtonTag::Right, RightButtonTriggerLevel),	// Right
	Keypad::Button(ButtonTag::Up, UpButtonTriggerLevel),		// Up 
	Keypad::Button(ButtonTag::Down, DownButtonTriggerLevel),	// Down
	Keypad::Button(ButtonTag::Left, LeftButtonTriggerLevel),	// Left
	Keypad::Button(ButtonTag::Select, SelectButtonTriggerLevel)	// Select
};

// Display field collection.
Display::Screen::Field display_fields[] =
{
	Display::Screen::Field(DayCol, DateRow),	// Day
	Display::Screen::Field(MonthCol, DateRow),	// Month
	Display::Screen::Field(YearCol, DateRow),	// Year
	Display::Screen::Field(HourCol, TimeRow),	// Hour
	Display::Screen::Field(MinuteCol, TimeRow),	// Minute
	Display::Screen::Field(SecondCol, TimeRow),	// Second
	Display::Screen::Field(AlarmCol, DateRow)	// Alarm
};
// Alarm screen.
Display::Screen alarm_screen(display_fields);
// Date/Time screen.
Display::Screen date_time_screen(std_begin(display_fields), std_end(display_fields) - 1U);

// Print formatting buffer.
char print_buf[LcdCols + 1U];
// Date cursor.
Cursor date_format(DayCol, DateRow, DateFmt, print_buf);
// Time cursor.
Cursor time_format(DayOfWeekCol, TimeRow, TimeFmt, print_buf);

// LCD device object.
LiquidCrystal lcd(LcdRs, LcdEnable, LcdD4, LcdD5, LcdD6, LcdD7);
// Keypad object.
Keypad keypad(KeypadInputPin, (Keypad::Callback(&keypadCallback)), Keypad::LongPress::Hold, KeypadLongPressInterval, buttons);
// Display object.
Display display(lcd, (Display::Callback(&displayCallback)));
// Digital clock object.
DigitalClock digital_clock((DigitalClock::Callback(&alarmCallback)));

/***************************
 * Task Scheduling Objects *
 ***************************/

 // Poll keypad command.
ClockCommand poll_keypad(keypad);
// Refresh display command.
ClockCommand refresh_display(display);
// Check alarm command.
ClockCommand check_alarm(digital_clock);
// Poll keypad task.
TaskScheduler::Task keypad_task(poll_keypad, KeypadPollingInterval, TaskScheduler::Task::State::Active);
// Refresh display task.
TaskScheduler::Task display_task(refresh_display, DisplayRefreshInterval, TaskScheduler::Task::State::Active);
// Check alarm task.
TaskScheduler::Task alarm_task(check_alarm, AlarmCheckingInterval, TaskScheduler::Task::State::Idle);
// Scheduled tasks collection.
TaskScheduler::Task* scheduled_tasks[] = { &keypad_task, &display_task, &alarm_task };
// Task scheduler object.
TaskScheduler task_scheduler(scheduled_tasks);

void setup()
{
	initialize();
}

void loop()
{
	task_scheduler.tick();
}

void initialize()
{
	lcd.begin(LcdCols, LcdRows);
	// Do any clock sync operations here.
	if (digital_clock.status() == timeNotSet)
		display.blink(DisplayBlinkInterval);
}

void keypadCallback(const Keypad::Button& button, Keypad::Event event)
{
	switch (event)
	{
	case Keypad::Event::Press:
		notify(button);
		break;
	case Keypad::Event::Longpress:
		buttonLongpressEvent(button);
		break;
	case Keypad::Event::Release:
		break;
	default:
		break;
	}
}

void buttonLongpressEvent(const Keypad::Button& button)
{
	if (digital_clock.mode() != DigitalClock::Mode::Run)
	{
		switch (button.tag_)
		{
		case ButtonTag::Select:
			digital_clock.mode(DigitalClock::Mode::SetAlarm); // Select longpress sets the alarm.
			display.screen(&alarm_screen);
			break;
		case ButtonTag::Up:
			keypad.repeat(true); // Button repeat only works for Up, ...
			break;
		case ButtonTag::Down:
			keypad.repeat(true); // ... and Down buttons.
			break;
		case ButtonTag::Left:
			break;
		case ButtonTag::Right:
			break;
		default:
			break;
		}
	}
}

void notify(const Keypad::Button& button)
{
	switch (digital_clock.mode())
	{
	case DigitalClock::Mode::Run:
		switch (button.tag_)
		{
		case ButtonTag::Select: // Select short press sets the date & time.
			digital_clock.mode(DigitalClock::Mode::SetTime);
			display.screen(&date_time_screen);
			display.mode(Display::Mode::Set);
			display.blink();
			break;
		case ButtonTag::Up:
			break;
		case ButtonTag::Down:
			break;
		case ButtonTag::Left:
			break;
		case ButtonTag::Right:
			break;
		default:
			break;
		}
		break;
	case DigitalClock::Mode::SetAlarm: // Alarm task needs update after setting the alarm. 
		if (button.tag_ == ButtonTag::Select)
			task_scheduler.state(&alarm_task, digital_clock.alarmEnabled());
	case DigitalClock::Mode::SetTime:
		switch (button.tag_)
		{
		case ButtonTag::Select:
			digital_clock.mode(DigitalClock::Mode::Run);
			display.mode(Display::Mode::Run);
			break;
		case ButtonTag::Up:
			digital_clock.inc();
			display.needsRefresh();
			break;
		case ButtonTag::Down:
			digital_clock.dec();
			display.needsRefresh();
			break;
		case ButtonTag::Left:
			digital_clock.prev();
			display.prevField();
			break;
		case ButtonTag::Right:
			digital_clock.next();
			display.nextField();
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

void displayCallback()
{
	const time_t time = digital_clock.time();
	const char* str_set = (digital_clock.mode() == DigitalClock::Mode::SetAlarm)
		? alarm_set
		: no_alarm_set;
	const char chr_alarm = digital_clock.alarmEnabled()
		? alarm_on
		: alarm_off;

	date_format(lcd, day(time), monthShortStr(month(time)), year(time), str_set);
	time_format(lcd, dayShortStr(weekday(time)), hour(time), minute(time), second(time), chr_alarm);
}

void alarmCallback()
{
	// Do something when the alarm goes off.
	//
	// Query DigitalClock::alarmActive() to see if alarm silenced.
}
