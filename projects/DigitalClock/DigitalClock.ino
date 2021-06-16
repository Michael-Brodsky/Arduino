
 /*	This file demonstrates an object-oriented solution to implementing a
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
 *		This sketch implements the bevior of a typical digital clock, 
 *		displaying the current time and date in a particular format and also
 *		features an alarm function. Both the current and alarm time are
 *		adjustable with the keypad and the alarm time is always stored in the
 *		onboard EEPROM and recalled on power up or after a reset.
 *
 *			01 Jan 1970  <--- date format is Day-Month-Year
 *			Thu 00:00:00 <--- time is 24 hour format.
 *
 *		On power up or after a hardware reset, the display shows the current
 *		date and time (if automatically synced) or flashes to indicate it
 *		needs to be manually set. Once set, it displays the current date and
 *		time steadily. The clock is toggled between "set" and "run" modes
 *		by the Select key. Short pressing the Select key adjusts the current
 *		date and time, longpressing it adjusts the alarm. When in "set" mode,
 *		the display cursor changes to a flashing block and highlights the
 *		"field" being adjusted.
 *
 *			█1 Jan 1970
 *			Thu 00:00:00 
 *
 *		The field value can be incremented or decremented with the Up or Down
 *		keys respectively. Holding down the keys causes them to "repeat" at  
 *		the same rate that the keypad is polled.
 *
 *		The Right and Left keys are used to scroll forward and backwards
 *		through the adjustable fields, one at a time. The cursor moves to the
 *		next or previous field and wraps around at the last and first field.
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
 *		Once adjusted, the time is set by short pressing the Select key again,
 *		which puts the display back into "run" mode.
 *
 *		The alarm is set by pressing and holding down the Select key until the
 *		display changes to "set" mode. The alarm is adjusted in the same way as
 *		the time, above. The alarm screen has one additional field to indicate 
 *		that the alarm is being adjusted and to toggle the alarm on and off.
 *
 *			█1 Jan 1970 ALRM <--- "ALRM" indicates alarm set mode.
 *			Thu 00:00:00
 *
 *		The alarm is toggled on and off by scrolling to the ALRM field and
 *		changing the setting with the Up or Down keys. Either key toggles the
 *		setting. An asterisk (*) below the ALRM field indicates the alarm is 
 *		on and no asterisks indicates that it's off.
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
 *	development, provides basic services and enforces const-correctness and use
 *	of appropriate data types.
 *
 *	The application-level code in this file consists of only a finite state
 *	machine for high-level logic and display printing functions. The majority
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
 *		`Display'-  a type that encapsulates behaviors of a character display
 *					device (see <Display.h>).
 *
 *		`TaskScheduler' - an asynchronous task scheduler. Here, the
 *					scheduler is driven synchronously by the Arduino `loop()'
 *					function and so operates in a non-preemptive manner.
 *					If interrupt driven, it supports basic preemptive multi-
 *					tasking (see <TaskScheduler.h>).
 *
 *		`DigitalClock'  - a type that encapsulates behaviors of a simple
 *					digital clock. This component provides much of the
 *					application logic and services (see <DigitalClock.h>).
 *
 *		Components are completely decoupled from each other and the Arduino 
 *		`LiquidCrystal' API, and communicate via callbacks defined in this 
 *		file.
 *
 *		There is also a separate `config.h' file that defines hardware
 *		configuration and other application constants. This facilitates 
 *		supporting a wide range of display and keypad hardware with a simple 
 *		file swap.
 *
 *	**************************************************************************/

 /****************
  * Dependencies *
  ****************/

//#include <EEPROMStream.h>
//#include <ISerializeable.h>
//#include <iterator.h>
//#include <TimeLib.h>
//#include <Time.h>
//#include <IClockable.h>
//#include <EEPROMStream.h>
//#include <Display.h>
//#include <IDisplay.h>
//#include <utils.h>
//#include <types.h>
//#include <tokens.h>
//#include <library.h>
#include <LiquidCrystal.h>			// Arduino lcd API.
#include <AnalogKeypad.h>			// `Keypad' type.
#include <LiquidCrystalDisplay.h>	// `LiquidCrystal' display manager.
#include <DigitalClock.h>			// `DigitalClock' type.
#include <TaskScheduler.h>			// `TaskScheduler' and `ClockCommand' types.
#include "config.h"					// Hardware and application configuration constants.

//#define NOEEPROM 1				// Uncomment to skip deserialization when EEPROM data corrupted.
									// Upload, run sketch and update alarm setting to overwrite EEPROM. 
									// Then comment out #define, recompile, upload and run normally.
/*************************
 * Function Declarations *
 *************************/

 // Initializes the application.
void initialize();
// Keypad component callback.
void keypadCallback(const Keypad::Button&, Keypad::Event);
// Button longpress event handler.
void buttonLongpressEvent(const Keypad::Button&);
// Keypad event notifier/dispatcher.
void notify(const Keypad::Button&);
// Handles events in `Run' mode.
void notifyRun(const Keypad::Button&);
// Handles events in `SetAlarm' mode.
void notifySetAlarm(const Keypad::Button&);
// Handles events in `SetTime' mode.
void notifySetTime(const Keypad::Button&);
// Display component callback.
void displayCallback();
// DigitalClock component callback.
void alarmCallback();

/******************************
 * Global Application Objects *
 ******************************/

 // Keypad buttons collection.
Keypad::Button buttons[] =
{
	Keypad::Button(ButtonTag::Right, RightButtonTriggerLevel),	// Right
	Keypad::Button(ButtonTag::Up, UpButtonTriggerLevel),		// Up 
	Keypad::Button(ButtonTag::Down, DownButtonTriggerLevel),	// Down
	Keypad::Button(ButtonTag::Left, LeftButtonTriggerLevel),	// Left
	Keypad::Button(ButtonTag::Select, SelectButtonTriggerLevel)	// Select
};

// Display fields collection.
const Display::Field display_fields[] =
{
	Display::Field(DayCol, DateRow),	// Day
	Display::Field(MonthCol, DateRow),	// Month
	Display::Field(YearCol, DateRow),	// Year
	Display::Field(HourCol, TimeRow),	// Hour
	Display::Field(MinuteCol, TimeRow),	// Minute
	Display::Field(SecondCol, TimeRow),	// Second
	Display::Field(AlarmCol, DateRow)	// Alarm
};
// Alarm screen (uses all seven display fields).
const Display::Screen alarm_screen("", display_fields, PrintFmt);
// Date/Time screen (uses only first six display fields).
const Display::Screen date_time_screen("", std_begin(display_fields), std_end(display_fields) - 1U, std_begin(PrintFmt), std_end(PrintFmt));
// `LiquidCrystal' hardware API.
LiquidCrystal lcd(LcdRs, LcdEnable, LcdD4, LcdD5, LcdD6, LcdD7);
// Keypad object.
Keypad keypad(KeypadInputPin, (Keypad::Callback(&keypadCallback)), Keypad::LongPress::Hold, KeypadLongPressInterval, buttons);
// Display object.
LiquidCrystalDisplay display(lcd, &displayCallback, &date_time_screen);
// EEPROM stream object.
EEPROMStream eeprom;
// Digital clock object.
DigitalClock digital_clock(eeprom, (DigitalClock::Callback(&alarmCallback)));

/***************************
 * Task Scheduling Objects *
 ***************************/

 // Poll keypad command.
ClockCommand poll_keypad(keypad);
// Refresh display command.
ClockCommand refresh_display(display);
// Check alarm command.
ClockCommand check_alarm(digital_clock);
// Scheduled tasks collection.
TaskScheduler::Task keypad_task(&poll_keypad, KeypadPollingInterval, TaskScheduler::Task::State::Active);
TaskScheduler::Task display_task(&refresh_display, DisplayRefreshInterval, TaskScheduler::Task::State::Active);
TaskScheduler::Task alarm_task(&check_alarm, AlarmCheckingInterval, TaskScheduler::Task::State::Idle);
TaskScheduler::Task* scheduled_tasks[] = 
{ 
	&keypad_task, & display_task, & alarm_task
};
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
#if !defined NOEEPROM
	digital_clock.deserialize(eeprom); // Retrieve saved alarm from EEPROM.
#endif // !defined NOEEPROM
	//
	// Do any clock sync operations here.
	//
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
	case Keypad::Event::Release:	// Button release events are ignored.
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
			digital_clock.mode(DigitalClock::Mode::SetAlarm); // "Select" longpress sets the alarm.
			display.screen(&alarm_screen);
			break;
		case ButtonTag::Up:
			keypad.repeat(true); // Button repeat only works for "Up", ...
			break;
		case ButtonTag::Down:
			keypad.repeat(true); // ... and "Down" buttons to allow for rapid time adjustment.
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
		notifyRun(button);
		break;
	case DigitalClock::Mode::SetAlarm: 
		notifySetAlarm(button);
	case DigitalClock::Mode::SetTime:
		notifySetTime(button);
		break;
	default:
		break;
	}
}

void notifyRun(const Keypad::Button& button)
{
	switch (button.tag_)
	{
	case ButtonTag::Select: // "Select" short press sets the date & time.
		digital_clock.mode(DigitalClock::Mode::SetTime);
		display.screen(&date_time_screen);
		display.cursor(Display::Cursor::Edit);
		display.blink(); // Stop blinking since we're going into "set" mode.
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
}

void notifySetAlarm(const Keypad::Button& button)
{
	if (button.tag_ == ButtonTag::Select) // Alarm task needs update after setting the alarm. 
		alarm_task.state() = (digital_clock.alarmEnabled())
		? TaskScheduler::Task::State::Active
		: TaskScheduler::Task::State::Idle;
		//task_scheduler.state(alarm_task, digital_clock.alarmEnabled());
}

void notifySetTime(const Keypad::Button& button)
{
	switch (button.tag_)
	{
	case ButtonTag::Select:		// Select press in either `SetTime' or `SetAlarm' mode returns us to `Run' mode.
		digital_clock.mode(DigitalClock::Mode::Run);
		display.cursor(Display::Cursor::Normal);
		break;
	case ButtonTag::Up:
		digital_clock.inc();	// Increment & display value in current field.
		display.print();
		break;
	case ButtonTag::Down:
		digital_clock.dec();	// Decrement & display value in current field.
		display.print();
		break;
	case ButtonTag::Left:
		digital_clock.prev();	// Advance to previous field.
		display.prev();
		break;
	case ButtonTag::Right:
		digital_clock.next();	// Advance to next field.
		display.next();
		break;
	default:
		break;
	}
}

void displayCallback()
{
	// Printing functions are handled here. The entire display is refreshed at once. 
	// Format buf stores one row of chars at a time and needs to be sized accordingly 
	// (columns + 1 for trailing NULL). Make sure format specifiers (in "config.h") 
	// don't overrun it. 

	char buf[LcdCols + 1U]; 
	const Display::Screen& screen = *display.screen();
	const time_t time = digital_clock.time();
	const char* str_set_alarm = (digital_clock.mode() == DigitalClock::Mode::SetAlarm)
		? alarm_set		// String is "ALRM" in `SetAlarm' mode, ... 
		: no_alarm_set;	// ... else it's "".
	const char chr_alarm_enabled = digital_clock.alarmEnabled()
		? alarm_on		// Char is '*' if alarm enabled, ... 
		: alarm_off;	// ... else it's ' '.

	// Arduino lcd API can only print a single row of chars to the harware at a time.
	// To print all rows at once, the cursor must be set to column 0 of each row,   
	// followed by a call to `LiquidCrystal::print()' with the corresponding row 
	// index passed to the `Screen' object's function call operator.

	uint8_t row = 0;
	lcd.setCursor(0, row);
	lcd.print(screen(buf, row, day(time), monthShortStr(month(time)), year(time), str_set_alarm));
	lcd.setCursor(0, ++row);
	lcd.print(screen(buf, row, dayShortStr(weekday(time)), hour(time), minute(time), second(time), chr_alarm_enabled));
}

void alarmCallback()
{
	// Do something when the alarm goes off.
	//
	// Query DigitalClock::alarmActive() to see if alarm silenced.
}
