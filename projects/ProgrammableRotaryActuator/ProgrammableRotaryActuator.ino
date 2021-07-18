/*
 *	Main application file for Rotary Actuator sketch.
 *
 *	***************************************************************************
 *
 *	File: Sequencer 4.ino
 *	Date: May 31, 2021
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
 *	This application file demonstrates a Programmable Rotary Actuator device 
 *	implemented on an Arduino Uno and compatible LCD/Keypad Shield. The device 
 *	controls a servo motor and positions it according to a user-defined, 
 *	chronological sequence. It can be used to automatically control rotary 
 *	valves and other mechanisms in a predefined manner, similarly to the way 
 *	sprinkler system valves work. The device can be programmed and operated via 
 *	the LCD/Keypad Shield or flash programmed via the on-board serial port. All 
 *	settings are stored in the on-board EEPROM memory.
 * 
 *	Actuator positions are stored as a sequence of "events". Each event has 
 *	three parameters: a human-readable "name", a "duration", displayed as 
 *	hh::mm::ss, and a rotation "angle" in degrees. Events are numbered 
 *	sequentially: 1, 2, 3 ... and displayed on the LCD in the following 
 *	format:
 * 
 *	********************
 *  * Auto:01   Closed *
 *  *      0° 00:00:10 *
 *	********************
 * 
 *	  Mode:nn   "Name"
 *	     ddd° hh:mm:ss
 * 
 *	Mode:    current operating mode
 *	nn:      event number (1, 2, 3, ...)
 *	Name:    event human-readable name, upto 7 chars max.
 *  ddd:     angle (actuator position) in degrees of rotation.
 *  hh:mm:ss event duration in hours, minutes and seconds.
 * 
 *	The device has five operating "modes": Auto, Man, Pgm, Cfg and Comm. 
 *	Modes can be selected from a menu of choices by long-pressing the 
 *	<Select> key in Auto mode, which is the default mode. Scroll the cursor 
 *	with the <Left/Right> keys until the desired mode is highlighted and 
 *  press and release the <Select> key to select that mode. The device 
 *	returns to "Auto" mode from any other mode using the <Select> key.
 *
 *	"Auto" mode runs thru the current sequence of events, displaying its name, 
 *	rotation angle and time remaining. The sequence can be started, stopped 
 *	and resumed with the <Select> key. The <Up/Down> keys manually advance 
 *	thru the sequence when the device is idle. The <Right> key resets 
 *	the sequence to the beginning (the first event). A nifty "spinner" thingy 
 *	appears at the bottom left of the display to indicate when the device is 
 *	active.
 * 
 *	"Man" mode is used to manually scroll thru and execute any event in the 
 *	current sequence. Use the <Up/Down> keys to scroll thru the sequence and 
 *  press the <Left> key to execute that event (position the actuator). The 
 *	<Right> key resets the sequence to the beginning (the first event).
 * 
 *	The remaining modes are used for editting parameters. In these modes the 
 *	cursor changes to a flashing block and highlights the currently selected 
 *	field on the display.
 * 
 *	"Pgm" mode is used to change event angle and duration parameters (events 
 *	can only be added, removed and renamed by flash programming via the 
 *	serial port). The diplay format is identical to "Auto" and "Man" modes. 
 *	Use the <Up/Down> keys to scroll to the desired event when the event 
 *	number field is highlighted. Use the <Left/Right> keys to scroll to the 
 *	desired field. Then use the <Up/Down> keys to change the value. Holding 
 *	down the <Up/Down> keys for a second or longer will cause the values to 
 *	change rapidly. When done editting, Hold down the <Select> key for one 
 *	second until the device switches back to "Auto: mode. This saves any 
 *	changes. To discard changes (undo), quickly press and release the <Select> 
 *	key.
 *	
 *	"Cfg" mode configures servo rotation parameters. This allows adjustment to 
 *	the rotation speed and accomodates different servo hardware with varying 
 *	rotation capabilities. Navigation/editting is done the same way as in 
 *	"Pgm" mode. 
 * 
 *	********************
 *	* Ini:  0° Stp: 96 *
 *	* Wrap:Y  Intv: 24 *
 *	********************
 * 
 *	The "Ini" fields sets the initial rotation angle when the device is first 
 *	powered on or after a reset. The "Stp" parameter determines how far the 
 *	servo rotates on each clocking cycle (it is operated in a manner similar 
 *	to a stepper motor). The "Intv" parameter determines the amount of time 
 *	between successive steps. Increasing the step size or decreasing the 
 *	interval will cause the servo to rotate from one position to another 
 *  faster. However, most servos are limited in their rotation speeds and 
 *	optimal values may have to be found experimentally or from the 
 *	manufacturer's harware specifications. The "Wrap" parameter sets whether 
 *	the current sequence wraps around and repeats continuously or stops after 
 *	the last event. "Y" indicates wrap-around and "N" indicates no wrap-around.
 * 
 * 
 *	"Comm" mode sets the serial_remote port communications protocols. The first 
 *	parameter sets the baud rate and the second sets the number of start/stop 
 *	bits and parity. Keypad navigation/editting is done the same as the "Pgm" 
 *	and "Cfg" modes.
 * 
 *	The device can be controlled and programmed remotely via the on-board 
 *	serial port, which is automatically started on power-up. The device 
 *	responds to a set of predefined "Command" strings, some with optional 
 *	parameters. Commands are sent and received as ASCII strings using a 
 *	specific format that must be followed. Strings have the following general 
 *	format:
 * 
 *		key [params] terminator
 * 
 *	The "key" is one of the following three character strings:
 * 
 *		srt - starts the current sequence, 
 *		stp - stops the current sequence, 
 *		res - resumes the current sequence at the current event.
 *		rst - resets the current sequence to the beginning (first event).
 *		lst - lists the current sequence of events from the device.
 *		sto - stores a new sequence of events and reboots the device.
 * 
 *	All command strings must end with the "terminator" character, which is a 
 *	newline '\n' (ASCII code 10 in decimal). 
 * 
 *	The lst and sto commands send/receive event parameters in the following 
 *	format:
 * 
 *		"name",duration,angle;
 * 
 *	The name field is enclosed in double quotes ("), and the duration and angle 
 *	fields are valid numeric values in milliseconds and degrees respectively. 
 *	Events are separated by a record-separator character, which is a semi-
 *	colon ';'. The "lst" command transmits a list of current events back to the 
 *	sender using this format and the "sto" command takes a list of events as 
 *	parameters in the same format. Thus, to send a new sequence of events to the 
 *	device, the sender would transmit:
 * 
 *		sto "Closed",10000,0;"Open",2000,90;\n
 * 
 *	This would create a sequence of two events, the first named "Closed" with a 
 *	duration of 10000 ms (10 secs) and a rotation angle of 0 degrees, and the 
 *	second, named "Open" with a duration of 2000 ms and angle of 90 degrees. A 
 *	subsequent "lst" command would return:
 * 
 *		"Closed",10000,0;"Open",2000,90;\n
 * 
 *	Due to the memory limitations of the Arduino Uno, the device can only store 
 *	eight (8) events at a time. For more storage, use a Leonardo or Mega and 
 *	call me for a software update :)
 * 
 *	A short demonstration video can be viewed here: 
 *	https://www.instagram.com/p/CQFh_5AHQNW/ 
 * 
 *	**************************************************************************/

#include "config.h"
//#define NOEEPROM 1	// Rewrites corrupted/missing EEPROM data. Uncomment & upload
						// when running sketch for first time, or if EEPROM becomes 
						// corrupted and causes errors/crashes. Then comment out and 
						// reupload for normal operation.

/*
 * Function decls.
 */

void memoryInitialize();
void lcdInitialize();
void serialInitialize(const SerialRemote&);
void keypadPressEvent(const Keypad::Button&);
void keypadReleaseEvent(const Keypad::Button&);
void keypadLongpressEvent(const Keypad::Button&);
void keypadCallback(const Keypad::Button&, Keypad::Event);
void sequencerCallback(const event_type&, event_state_type);
void actuatorCallback(RotaryActuator::State);
void serialCallback(CommandTag);
void displayCallback();
void sequencerAction(Action);
void menuSelect(const Display::Field&);
void scrollField(Scroll);
void setMode(Mode);
void adjustEvent(const Display::Field&, int8_t);
void adjustDuration(const Display::Field&, int8_t);
void adjustIndex(int8_t);
void adjustAngle(int8_t);
void adjustConfig(const Display::Field&, int8_t);
void adjustStep(int8_t);
void adjustInterval(int8_t);
void adjustWrap(int8_t);
void adjustInitAngle(int8_t);
void adjustComms(const Display::Field&, int8_t);
void listEvents(const sequence_type&);
void storeEvents(const char*);
void initEvent(event_type&, char*, char*);
void loadSequence(sequence_type&);
void storeSequence(const sequence_type&);
void copySequence(const sequence_type&, sequence_type&);
void restoreSequence(sequence_type&, sequence_type&);
void createSequence(sequence_type&, sequence_type::size_type);
void loadConfig(config_t&);
void storeConfig(EEPROMStream::address_type, config_t&);
void copyConfig(config_t&);
void restoreConfig(const config_t&);
void loadComms(SerialProtocols&);
void storeComms(EEPROMStream::address_type, SerialProtocols&);
void copyComms(SerialProtocols&);
void restoreComms(SerialProtocols&);

/*
 * Global application objects.
 */

/* Keypad button objects */

const Keypad::Button right_button(ButtonTag::Right, RightButtonTriggerLevel);
const Keypad::Button up_button(ButtonTag::Up, UpButtonTriggerLevel);
const Keypad::Button down_button(ButtonTag::Down, DownButtonTriggerLevel);
const Keypad::Button left_button(ButtonTag::Left, LeftButtonTriggerLevel);
const Keypad::Button select_button(ButtonTag::Select, SelectButtonTriggerLevel);
const Keypad::Button buttons[] = { right_button, up_button, down_button, left_button, select_button };

/* Display field objects, used for keypad navigation/editting settings */

const Display::Field index_field(IndexCol, TopRow);
const Display::Field angle_field(AngleCol, BottomRow);
const Display::Field hour_field(HourCol, BottomRow);
const Display::Field minute_field(MinuteCol, BottomRow);
const Display::Field second_field(SecondCol, BottomRow);
const Display::Field field_init_angle(ColumnInitAngle, TopRow);
const Display::Field field_step_size(ColumnStepSize, TopRow);
const Display::Field field_wrap(ColumnWrap, BottomRow);
const Display::Field field_step_interval(ColumnStepInterval, BottomRow);
const Display::Field baud_field(BaudCol, TopRow);
const Display::Field proto_field(ProtoCol, TopRow);
const Display::Field auto_field(AutoCol, TopRow);
const Display::Field man_field(ManCol, TopRow);
const Display::Field pgm_field(PgmCol, BottomRow);
const Display::Field cfg_field(CfgCol, BottomRow);
const Display::Field comm_field(CommCol, BottomRow);
const Display::Field man_fields[] = { index_field };
const Display::Field pgm_fields[] = { index_field, angle_field, hour_field, minute_field, second_field };
const Display::Field cgf_fields[] = { field_init_angle, field_step_size, field_wrap, field_step_interval };
const Display::Field menu_fields[] = { auto_field, man_field, pgm_field, cfg_field, comm_field };
const Display::Field comm_fields[] = { baud_field, proto_field };

/* Display screen objects, each contains a collection of field objects. 
   Screens are changed according to the display mode. */

const Display::Screen man_screen(ManLabel, man_fields, EventPrintFmt);
const Display::Screen pgm_screen(PgmLabel, pgm_fields, EventPrintFmt);
const Display::Screen cfg_screen(LabelConfigScreen, cgf_fields, PrintFmtConfigScreen);
const Display::Screen menu_screen(MenuLabel, menu_fields, MenuPrintFmt);
const Display::Screen comm_screen(CommLabel, comm_fields, CommPrintFmt);
const Display::Screen auto_screen(AutoLabel, nullptr, nullptr, std_begin(EventPrintFmt), std_end(EventPrintFmt));

/* SerialRemote command objects, allow for remote control via serial_remote port. */

Command<void, void, CommandTag> start_cmd(&serialCallback, CommandTag::Start);
Command<void, void, CommandTag> stop_cmd(&serialCallback, CommandTag::Stop);
Command<void, void, CommandTag> resume_cmd(&serialCallback, CommandTag::Resume);
Command<void, void, CommandTag> reset_cmd(&serialCallback, CommandTag::Reset);
Command<void, void, CommandTag> list_cmd(&serialCallback, CommandTag::List);
Command<void, void, CommandTag> store_cmd(&serialCallback, CommandTag::Store);
const SerialRemote::Command serial_cmds[] =
{
	SerialRemote::Command(CommandTag::Start, SerialStartString, &start_cmd),	// Reset & start sequencer. 
	SerialRemote::Command(CommandTag::Stop, SerialStopString, &stop_cmd),		// Stop sequencer.
	SerialRemote::Command(CommandTag::Resume, SerialResumeString, &resume_cmd),	// Resume sequencer without resetting.
	SerialRemote::Command(CommandTag::Reset, SerialResetString, &reset_cmd),	// Reset sequencer.
	SerialRemote::Command(CommandTag::List, SerialListString, &list_cmd),		// List current sequence.
	SerialRemote::Command(CommandTag::Store, SerialStoreString, &store_cmd)		// Store new sequence & reboot.
};

/* Hardware objects */

char serial_buf[MaxEventRecords * MaxCharsPerRecord];
EEPROMStream eeprom;
Keypad keypad(KeypadInputPin, &keypadCallback, Keypad::LongPress::Hold, KeypadLongPressInterval, buttons);
LiquidCrystal lcd(LcdRs, LcdEnable, LcdD4, LcdD5, LcdD6, LcdD7);
Display display(lcd, &displayCallback);
Spinner spinner(SpinnerChars, SpinnerDivisor); // Way cool spinner thingy to indicate when sequencer is active.
SerialProtocols serial_protocols;
SerialRemote serial_remote(serial_buf, serial_cmds);
SweepServo<servo_hardware> servo;
angle_t servo_init_angle = ServoMinAngle;
config_t config(ServoDfltStepSize, ServoDfltStepInterval, servo_init_angle, false);
RotaryActuator actuator(servo, &actuatorCallback);
Sequencer sequencer(nullptr, nullptr, &sequencerCallback, true);

/* Task scheduling objects, each ClockCommand is executed by the TaskScheduler 
   according to its Task scheduling interval and state. */

ClockCommand keypad_clock(keypad);
ClockCommand display_clock(display);
ClockCommand sequencer_clock(sequencer);
ClockCommand actuator_clock(actuator);
ClockCommand serial_clock(serial_remote);
TaskScheduler::Task keypad_task(&keypad_clock, KeypadPollingInterval, TaskScheduler::Task::State::Active);
TaskScheduler::Task display_task(&display_clock, DisplayRefreshInterval, TaskScheduler::Task::State::Active);
TaskScheduler::Task sequencer_task(&sequencer_clock, SequencerClockingInterval, TaskScheduler::Task::State::Idle);
TaskScheduler::Task actuator_task(&actuator_clock, ServoDfltStepInterval, TaskScheduler::Task::State::Idle);
TaskScheduler::Task serial_task(&serial_clock, SerialPollingInterval, TaskScheduler::Task::State::Active);
TaskScheduler::Task* tasks[] = { &keypad_task, &display_task, &sequencer_task, &actuator_task, &serial_task };
TaskScheduler task_scheduler(tasks);

/*
 * Static memory allocators.
 */

char strings[MaxEventRecords * 2U][MaxLengthEventName];
actuator_command_type _commands[MaxEventRecords * 2U];
event_type _events[MaxEventRecords * 2U];
event_type* events[MaxEventRecords * 2U];
sequence_type tmp_events;
// Needs an MMU, of sorts, to realloc memory.

/*
 * Application state objects.
 */

Mode mode = Mode::Auto;	// The current operating mode.
bool keypad_release_enabled = true; // Kills the Select button release event after a long-press.
EEPROMStream::address_type config_address = 0U, comms_address = 0U; // EEPROM addresses for config and comms storage.

void setup()
{
	memoryInitialize();
#if defined NOEEPROM
	// Assign two events and store them in the EEPROM 
	// along with the current config and comms settings.
	events[0]->name_ = "Closed";
	events[0]->duration_ = 10000UL;
	events[1]->name_ = "Open";
	events[1]->duration_ = 2000UL;
	static_cast<actuator_command_type*>(events[1]->command_)->angle(90U);
	createSequence(sequencer.events(), 2U);
	storeSequence(sequencer.events());
	restoreConfig(config);
	storeConfig(config_address = eeprom.address(), config);
	storeComms(comms_address = eeprom.address(), serial_protocols);
#else
	// Load the sequencer events, config and comms settings from the EEPROM.
	loadSequence(sequencer.events());
	config_address = eeprom.address();
	loadConfig(config);
	comms_address = eeprom.address();
	loadComms(serial_protocols);
#endif
	// Create a copy of the sequencer events for editing/restore. 
	createSequence(tmp_events, sequencer.events().size());
	lcdInitialize();
	// Initialize operating mode to "Auto".
	setMode(Mode::Auto);
	// Attach & initialize the servo and actuator objects.
	servo.attach(ServoControlPin);
	servo.initialize(servo_init_angle);
	actuator.begin();
}

void loop() 
{
	// Execute any scheduled tasks.
	task_scheduler.tick();
}

void memoryInitialize()
{
	// Initialize the static memory allocators, cuz the Arduino dynamic memory manager kinda blows. 
	// Spent all-nighters trying to get new, delete and String types to work. Not a chance. Replaced 
	// them with these beauties:
	// 
	// `_commands' array stores concrete sequencer event command objects, which set the actuator position, 
	// `strings' array stores human-readable event names, 
	// `_events' array stores concrete sequencer event objects. 
	// `events' array stores pointers to concrete event objects. 
	// It's all in my new book: "How Not To Malloc Your Way To Oblivion In MCU Applications", 
	// now on Amazon, maybe (not).

	for (size_t i = 0; i < size_of(events); ++i) // For some reason, the C++11 range-based for loop no worky-worky here.
	{
		_commands[i].object(&actuator);
		_commands[i].method(&RotaryActuator::position);
		_events[i].name_ = strings[i];
		_events[i].command_ = &_commands[i];
		events[i] = &_events[i];
	}
}

void lcdInitialize()
{
	lcd.begin(LcdCols, LcdRows);
}

void serialInitialize(const SerialProtocols& comms)
{
	Serial.begin(comms.baud(), comms.protocol().second);
	Serial.flush();
}

void keypadPressEvent(const Keypad::Button& button)
{
	switch (button.tag_)
	{
	case ButtonTag::Down:	// Down button adjusts settings.
		switch (mode)
		{
		case Mode::Auto:
			if (sequencer.status() != Sequencer::Status::Idle)
				break;
		case Mode::Man:
			adjustEvent(index_field, Decrement);
			break;
		case Mode::Pgm:
			adjustEvent(*display.field(), Decrement);
			break;
		case Mode::Cfg:
			adjustConfig(*display.field(), Decrement);
			break;
		case Mode::Comms:
			adjustComms(*display.field(), Decrement);
			break;
		default:
			break;
		}
		break;
	case ButtonTag::Left:	// Left button executes the current sequencer event in Man mode ... 
		switch (mode)
		{
		case Mode::Auto:
			break;
		case Mode::Man:
			sequencer.event().command_->execute();
			break;
		case Mode::Pgm:		// ... else it scrolls the cursor to the previous field.
		case Mode::Cfg:
		case Mode::Menu:
		case Mode::Comms:
			scrollField(Scroll::Previous);
			break;
		default:
			break;
		}
		break;
	case ButtonTag::Right:	// Right button resets the sequencer in Auto and Man modes ... 
		switch (mode)
		{
		case Mode::Auto:
			if (sequencer.status() == Sequencer::Status::Active)
				break;
		case Mode::Man:
			sequencer.reset();
			display.print(); /* Man mode requires display update. */
			break;
		case Mode::Pgm:		// ... else it scrolls the cursor to the next field.
		case Mode::Cfg:
		case Mode::Menu:
		case Mode::Comms:
			scrollField(Scroll::Next);
			break;
		default:
			break;
		}
		break;
	case ButtonTag::Select: // Select button only responds to release & long-press.
		break;
	case ButtonTag::Up:		// Up button adjusts settings.
		switch (mode)
		{
		case Mode::Auto:
			if (sequencer.status() != Sequencer::Status::Idle)
				break;
		case Mode::Man:
			adjustEvent(index_field, Increment);
			break;
		case Mode::Pgm:
			adjustEvent(*display.field(), Increment);
			break;
		case Mode::Cfg:
			adjustConfig(*display.field(), Increment);
			break;
		case Mode::Comms:
			adjustComms(*display.field(), Increment);
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

void keypadReleaseEvent(const Keypad::Button& button)
{
	keypad.repeat(false);
	switch (button.tag_)
	{
	case ButtonTag::Down:
		break;
	case ButtonTag::Left:
		break;
	case ButtonTag::Right:
		break;
	case ButtonTag::Select:	// Select button activated when released to allow for long-press.
		switch (mode)
		{
		case Mode::Auto:
			switch (sequencer.status())
			{
			case Sequencer::Status::Active:
				sequencerAction(Action::Stop);
				break;
			case Sequencer::Status::Idle:
				sequencerAction(Action::Resume);
				break;
			case Sequencer::Status::Done:
				sequencerAction(Action::Start);
				break;
			default:
				break;
			}
			break;
		case Mode::Man:
			setMode(Mode::Auto);
			break;
		case Mode::Pgm:	// Select button release is "undo" in edit modes, restores settings to original values.
			restoreSequence(tmp_events, sequencer.events());
			setMode(Mode::Auto);
			break;
		case Mode::Cfg:
			restoreConfig(config);
			setMode(Mode::Auto);
			break;
		case Mode::Comms:
			restoreComms(serial_protocols);
			setMode(Mode::Auto);
			break;
		case Mode::Menu:
			menuSelect(*display.field());
			break;
		default:
			break;
		}
		break;
	case ButtonTag::Up:
		break;
	default:
		break;
	}
}

void keypadLongpressEvent(const Keypad::Button& button)
{
	switch (button.tag_)
	{
	case ButtonTag::Down:	// Down button repeats on long-press.
		if (mode != Mode::Auto)
			keypad.repeat(true);
		break;
	case ButtonTag::Left:
		break;
	case ButtonTag::Right:
		break;
	case ButtonTag::Select:	// Select button long-press displays menu screen in Auto mode and saves changes in edit modes.
		switch (mode)
		{
		case Mode::Auto:
			setMode(Mode::Menu);
			break;
		case Mode::Man:
			setMode(Mode::Auto);
			break;
		case Mode::Pgm:
			storeSequence(sequencer.events());
			setMode(Mode::Auto);
			break;
		case Mode::Cfg:
			storeConfig(config_address, config);
			setMode(Mode::Auto);
			break;
		case Mode::Comms:
			storeComms(comms_address, serial_protocols);
			setMode(Mode::Auto);
			break;
		case Mode::Menu:
			break;
		default:
			break;
		}
		keypad_release_enabled = false;
		break;
	case ButtonTag::Up:	// Up button repeats on long-press.
		if (mode != Mode::Auto)
			keypad.repeat(true);
		break;
	default:
		break;
	}
}

void keypadCallback(const Keypad::Button& button, Keypad::Event event)
{
	// Handle any keypad button events.
	switch (event)
	{
	case Keypad::Event::Press:
		keypadPressEvent(button);
		break;
	case Keypad::Event::Longpress:
		keypadLongpressEvent(button);
		break;
	case Keypad::Event::Release:
		if (!keypad_release_enabled)
			keypad_release_enabled = true;
		else
			keypadReleaseEvent(button);
		break;
	default:
		break;
	}
}

void sequencerCallback(const event_type& event, event_state_type state)
{
	// Kill the spinner display thingy when sequencer goes idle.
	switch (state)
	{
	case event_state_type::Begin:
		break;
	case event_state_type::End:
		spinner.visible() = sequencer.status() == Sequencer::Status::Active;
		break;
	default:
		break;
	}
}

void actuatorCallback(RotaryActuator::State state)
{
	// Set the actuator task state according to its internal state.
	switch (state)
	{
	case RotaryActuator::State::Init:
		break;
	case RotaryActuator::State::Idle:
		actuator_task.state() = TaskScheduler::Task::State::Idle;
		break;
	case RotaryActuator::State::Active:
		actuator_task.state() = TaskScheduler::Task::State::Active;
		break;
	case RotaryActuator::State::Error:
		break;
	default:
		break;
	}
}

void serialCallback(CommandTag tag)
{
	// Handle any valid serial_remote commands received.
	switch (tag)
	{
	case CommandTag::Start:
		if (sequencer.status() != Sequencer::Status::Active)
			sequencerAction(Action::Start);
		break;
	case CommandTag::Stop:
		if (sequencer.status() == Sequencer::Status::Active)
			sequencerAction(Action::Stop);
		break;
	case CommandTag::Resume:
		if (sequencer.status() == Sequencer::Status::Idle)
			sequencerAction(Action::Resume);
		break;
	case CommandTag::Reset:
		sequencer.reset();
		break;
	case CommandTag::List:
		listEvents(sequencer.events());
		break;
	case CommandTag::Store:
		storeEvents(serial_remote.buf());
		break;
	default:
		break;
	}
}

void displayCallback()
{
	// Update the LCD display.
	char buf[LcdCols + 1];
	//tmElements_t tm;
	msecs_t time = sequencer.event().duration_;
	const angle_t angle = static_cast<actuator_command_type*>(sequencer.event().command_)->angle();
	const Display::Screen screen = *display.screen();
	const char* label = screen.label();
	const char wrap = sequencer.wrap() ? WrapChar : NoWrapChar;
	uint8_t col = 0, row = 0;

	// Arduino `LiquidCrystal' API can only print one row at a time.
	// Set cursor to top row.
	lcd.setCursor(col, row);
	switch (mode)
	{
	case Mode::Auto:
		time = sequencer.event().duration_ < sequencer.elapsed()
			? 0
			: time - sequencer.elapsed();
		time += RemainingTimeOffset;
	case Mode::Pgm:
	case Mode::Man:
	{
		std_chrono_millis tm(time);
		lcd.print(screen(buf, row, label, sequencer.index(), sequencer.event().name_)); // Print top row.
		lcd.setCursor(col, ++row);		// Set cursor to bottom row.
		lcd.print(screen(buf, row, spinner.spin(), angle, DegreesSymbol, tm.hours(), tm.minutes(), tm.seconds())); // Print bottom row.
		break;
	}
	case Mode::Cfg:
		lcd.print(screen(buf, row, servo_init_angle, DegreesSymbol, servo.stepSize()));
		lcd.setCursor(col, ++row);
		lcd.print(screen(buf, row, wrap, actuator_task.interval()));
		break;
	case Mode::Menu:
		lcd.print(screen(buf, row, label, AutoLabel, ManLabel));
		lcd.setCursor(col, ++row);
		lcd.print(screen(buf, row, PgmLabel, LabelConfigScreen, CommLabel));
		break;
	case Mode::Comms:
		lcd.print(screen(buf, row, label, serial_protocols.baud(), serial_protocols.protocol().first)); // Comms screen only prints top row.
	default:
		break;
	}
}

void sequencerAction(Action action)
{
	TaskScheduler::Task::State task_state = TaskScheduler::Task::State::Active;
	bool spinner_visible = true;

	// Command the sequencer to start, stop or resume and set its task state accordingly.
	switch (action)
	{
	case Action::Start:
		sequencer.start();
		break;
	case Action::Stop:
		sequencer.stop();
		task_state = TaskScheduler::Task::State::Idle;
		spinner_visible = false;
		break;
	case Action::Resume:
		sequencer.resume();
		break;
	default:
		break;
	}
	sequencer_task.state() = task_state;
	spinner.visible() = spinner_visible;	// Spinner thingy appears on the display when sequencer is active.
}

void menuSelect(const Display::Field& field)
{
	Mode m = Mode::Auto;

	// Cycles through the various operating modes/display screens.
	if (field == man_field)
		m = Mode::Man;
	else if (field == pgm_field)
		m = Mode::Pgm;
	else if (field == cfg_field)
		m = Mode::Cfg;
	else if (field == comm_field)
		m = Mode::Comms;
	setMode(m);
}

void scrollField(Scroll dir)
{

	if (display.cursor() != Display::Cursor::Block)
		display.cursor(Display::Cursor::Block);
	// Scrolls the display cursor through the current screen's fields.
	switch (dir)
	{
	case Scroll::Previous:
		display.prev();
		break;
	case Scroll::Next:
		display.next();
		break;
	default:
		break;
	}
}

void setMode(Mode m)
{
	// Switch the display screen/cursor according to the current operating mode.
	switch (m)
	{
	case Mode::Menu:
		display.screen(&menu_screen);
		display.cursor(Display::Cursor::Block);
		break;
	case Mode::Auto:
		display.screen(&auto_screen);
		display.cursor(Display::Cursor::Normal);
		break;
	case Mode::Man:
		sequencerAction(Action::Stop);
		display.screen(&man_screen);
		display.cursor(Display::Cursor::Block);
		break;
	case Mode::Pgm:
		copySequence(sequencer.events(), tmp_events);
		display.screen(&pgm_screen);
		display.cursor(Display::Cursor::Block);
		break;
	case Mode::Cfg:
		copyConfig(config);
		display.screen(&cfg_screen);
		display.cursor(Display::Cursor::Block);
		break;
	case Mode::Comms:
		copyComms(serial_protocols);
		display.screen(&comm_screen);
		display.cursor(Display::Cursor::Block);
		break;
	default:
		break;
	}
	display.clear();
	mode = m;
}

void adjustEvent(const Display::Field& field, int8_t adjustment)
{
	switch (mode)
	{
	case Mode::Pgm:
	case Mode::Cfg:
	case Mode::Menu:
	case Mode::Comms:
		if (display.cursor() != Display::Cursor::Edit)
			display.cursor(Display::Cursor::Edit);
		break;
	default:
		break;
	}
	if (field == index_field)
		adjustIndex(adjustment);
	else if (field == angle_field)
		adjustAngle(adjustment);
	else
		adjustDuration(field, adjustment);
	display.print();
}

void adjustDuration(const Display::Field& field, int8_t adjustment)
{
	time_t adjusted = sequencer.event().duration_;

	if (field == hour_field)
		adjusted += (MillisPerHour * adjustment);
	else if (field == minute_field)
		adjusted += (MillisPerMinute * adjustment);
	else if (field == second_field)
		adjusted += (MillisPerSecond * adjustment);
	if (adjusted > MillisPerDay)
		adjusted = adjustment < 0 ? MillisPerDay : 0;
	sequencer.event().duration_ = adjusted;
}

void adjustIndex(int8_t adjustment)
{
	if (adjustment < 0)
		sequencer.prev();
	else
		sequencer.next();
}

void adjustAngle(int8_t adjustment)
{
	actuator_command_type* cmd = static_cast<actuator_command_type*>(sequencer.event().command_);
	angle_t angle = (cmd->angle());

	switch (adjustment)
	{
	case Increment:
		if (++angle == ServoMaxAngle + 1U)
			angle = ServoMinAngle;
		break;
	case Decrement:
		if (angle == ServoMinAngle)
			angle = ServoMaxAngle + 1U;
		--angle;
		break;
	default:
		break;
	}
	cmd->angle(angle);
}

void adjustConfig(const Display::Field& field, int8_t adjustment)
{
	if (display.cursor() != Display::Cursor::Edit)
		display.cursor(Display::Cursor::Edit);
	if (field == field_step_size)
		adjustStep(adjustment);
	else if (field == field_wrap)
		adjustWrap(adjustment);
	else if (field == field_init_angle)
		adjustInitAngle(adjustment);
	else
		adjustInterval(adjustment);
	display.print();
}

void adjustStep(int8_t adjustment)
{
	step_t step_size = servo.stepSize();

	switch (adjustment)
	{
	case Increment:
		if (++step_size == ServoMaxStepSize + 1UL)
			step_size = ServoMinStepSize;
		break;
	case Decrement:
		if (step_size == ServoMinStepSize)
			step_size = ServoMaxStepSize + 1UL;
		--step_size;
		break;
	default:
		break;
	}
	servo.stepSize(step_size);
}

void adjustInterval(int8_t adjustment)
{
	msecs_t intvl = actuator_task.interval();

	switch (adjustment)
	{
	case Increment:
		if (++intvl == ServoMaxStepInterval + 1UL)
			intvl = ServoMinStepInterval;
		break;
	case Decrement:
		if (intvl == ServoMinStepInterval)
			intvl = ServoMaxStepInterval + 1UL;
		--intvl;
		break;
	default:
		break;
	}
	actuator_task.interval() = intvl;
}

void adjustWrap(int8_t adjustment)
{
	sequencer.wrap(!sequencer.wrap());
}

void adjustInitAngle(int8_t adjustment)
{
	switch (adjustment)
	{
	case Increment:
		if (++servo_init_angle == ServoMaxAngle + 1U)
			servo_init_angle = ServoMinAngle;
		break;
	case Decrement:
		if (servo_init_angle == ServoMinAngle)
			servo_init_angle = ServoMaxAngle + 1U;
		--servo_init_angle;
		break;
	default:
		break;
	}
}

void adjustComms(const Display::Field& field, int8_t adjustment)
{
	SerialProtocols::Select selection = field == proto_field
		? SerialProtocols::Select::Protocol
		: SerialProtocols::Select::Baud;

	if (display.cursor() != Display::Cursor::Edit)
		display.cursor(Display::Cursor::Edit);
	serial_protocols.select(selection);
	if (adjustment == Increment)
		serial_protocols.next();
	else
		serial_protocols.prev();
	display.print();
}

void listEvents(const sequence_type& sequence)
{
	char d[9]; // itoa/ltoa conversion buffer.

	serial_remote.buf()[0] = '\0';
	// Assemble a string of event parameters and send it to the serial buffer for transmission.
	for (auto& it : sequence)
	{
		charcat(serial_remote.buf(), StringDelimiterChar);
		strcat(serial_remote.buf(), it->name_);
		charcat(serial_remote.buf(), StringDelimiterChar);
		charcat(serial_remote.buf(), GroupSeparatorChar);
		strcat(serial_remote.buf(), ltoa(it->duration_, d, DecimalRadix));
		charcat(serial_remote.buf(), GroupSeparatorChar);
		strcat(serial_remote.buf(), itoa(static_cast<actuator_command_type*>(it->command_)->angle(), d, DecimalRadix));
		charcat(serial_remote.buf(), RecordSeparatorChar);
	}
	charcat(serial_remote.buf(), SerialRemote::EndOfTextChar);
	Serial.print(serial_remote.buf());
}

void storeEvents(const char* buf)
{
	char* start = (char*)buf + strlen(SerialStoreString), * from = start, * to = nullptr;
	uint8_t n = 0;

	// Parse characters in the serial buffer and create a collection of sequencer events
	// equal to the number of event strings received.
	while ((to = strchr(from, RecordSeparatorChar)))
	{
		if (++n == MaxEventRecords)
			break;
		from = ++to;
	}

	// Store the sequence in the EEPROM
	eeprom.reset();
	eeprom << n;
	tmp_events = sequence_type(&events[0], n); // Alloc new sequence from event[0].
	from = start;
	n = 0;
	while ((to = strchr(from, RecordSeparatorChar)))
	{
		initEvent(*tmp_events[n], from, to);
		if (++n == MaxEventRecords)
			break;
		from = to + 1;
	}
	storeSequence(tmp_events);
	// Reboot the device.
	resetFunc();
}

void initEvent(event_type& e, char* first, char* last)
{
	EventGroup grp = EventGroup::Name;
	char* to = nullptr, * begin = nullptr, * end = nullptr;

	*last = '\0';
	*const_cast<char*>(e.name_) = '\0';
	e.duration_ = 0;
	static_cast<actuator_command_type*>(e.command_)->angle(ServoMinAngle);
	// Parse an event parameter string and initilize a sequencer event object from the values.
	while (first < last && (to = strchr(first, GroupSeparatorChar)))
	{
		switch (grp)
		{
		case EventGroup::Name:
			if ((begin = strchr(first, StringDelimiterChar)) && (end = strchr(++begin, StringDelimiterChar)))
			{
				if (end < to && begin < end)
				{
					size_t len = (size_t)(end - begin) < MaxLengthEventName ? (size_t)(end - begin) : MaxLengthEventName - 1U;

					strncpy(const_cast<char*>(e.name_), begin, len);
					(const_cast<char*>(e.name_))[len] = '\0';
				}
			}
			grp = EventGroup::Duration;
			break;
		case EventGroup::Duration:
			if (to < last)
			{
				*to = '\0';
				e.duration_ = atol(first);
			}
			grp = EventGroup::Angle;
			break;
		default:
			break;
		}
		first = to + 1;
	}
	if (first < last)
		static_cast<actuator_command_type*>(e.command_)->angle(atol(first) % (ServoMaxAngle + 1U));
}

void createSequence(sequence_type& sequence, sequence_type::size_type n)
{
	// Cheezy static memory allocator for event sequences.
	static uint8_t i = 0;

	sequence = sequence_type(&events[i], n);
	i += n;
}

void loadSequence(sequence_type& sequence)
{
	// Read a sequence of events from the EEPROM and assign to sequencer.
	EEPROMStream::address_type n = 0;
	sequence_t s;
	
	eeprom >> n;
	createSequence(sequence, n);
	s.events_ = sequence;
	s.deserialize(eeprom);
	sequencer.reset();
}

void storeSequence(const sequence_type& sequence)
{
	// Store the current sequencer events in the EEPROM.
	sequence_t s(sequence);

	eeprom.reset();
	eeprom << s.events_.size();
	s.serialize(eeprom);
	sequencer.reset();
}

void copySequence(const sequence_type& sequence, sequence_type& copy)
{
	// Make a copy of the current sequencer events. `std_copy' = no bueno due to shallow copy,  
	// `actuator_command_type' needs deeeeeeeep copy. Programmers also need big screens to read 
	// my 140 char lines of code, cuz that's just how I roll. Fight me!
	for (size_t i = 0; i < sequence.size(); ++i)
	{
		copy[i]->name_ = sequence[i]->name_;
		copy[i]->duration_ = sequence[i]->duration_;
		static_cast<actuator_command_type*>(copy[i]->command_)->angle(static_cast<actuator_command_type*>(sequence[i]->command_)->angle());
	}
}

void restoreSequence(sequence_type& copy, sequence_type& sequence)
{
	// Restore the sequencer events from a copy.
	uint8_t index = sequencer.index();

	copy.swap(sequence);
	while (sequencer.index() != index)
		sequencer.next();
}

void loadConfig(config_t& cfg)
{
	cfg.deserialize(eeprom);
	restoreConfig(cfg);
}

void storeConfig(EEPROMStream::address_type addr, config_t& cfg)
{
	eeprom.address() = addr;
	copyConfig(cfg);
	cfg.serialize(eeprom);
}

void copyConfig(config_t& copy)
{
	copy.step_interval_ = actuator_task.interval();
	copy.step_size_ = servo.stepSize();
	copy.init_angle_ = servo_init_angle;
	copy.wrap_ = sequencer.wrap();
}

void restoreConfig(config_t& copy)
{
	actuator_task.interval() = copy.step_interval_;
	servo.stepSize(copy.step_size_);
	servo_init_angle = copy.init_angle_;
	sequencer.wrap(copy.wrap_);
}

void loadComms(SerialProtocols& comms)
{
	comms.deserialize(eeprom);
	serialInitialize(comms);
}

void storeComms(EEPROMStream::address_type addr, SerialProtocols& comms)
{
	eeprom.address() = addr;
	comms.serialize(eeprom);
	if (Serial)
		Serial.end();
	serialInitialize(comms);
}

void copyComms(SerialProtocols& comms)
{
	comms.copy();
}

void restoreComms(SerialProtocols& comms)
{
	comms.restore();
}
